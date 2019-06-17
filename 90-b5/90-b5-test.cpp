#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include "../common/cmd_net_tools.h"
#include "../common/md5.h"
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ѧ��+MD5(password)����security_str��λ�����γ�Ҫ���͵ġ�����������
  ���������char *reg_str				��������Ҫ���͵Ĵ������أ�
			const char *stu_no			��ѧ��
			const char *stu_password	������ҵϵͳ�еĿ�����룩�����ۿ���೤��ת��ΪMD5��Ϊ32�ֽڵ�hex��
			const char *security_str	����Server���յ��������ܴ�
  �� �� ֵ��
  ˵    ������ѧ�� : 1859999 , ���� : Password����� : f272a9b7422ee1ddec6c4b1abe758cadefc658c2 Ϊ��
			1��Password     �� => MD5 => ��dc647eb65e6711e155375218212b3964��
			2����֤��(ԭʼ) ��1859999+dc647eb65e6711e155375218212b3964
			3����֤��(ԭʼ)����򴮽��а�λ���
				   1859999+dc647eb65e6711e155375218212b3964
				   f272a9b7422ee1ddec6c4b1abe758cadefc658c2
			   ���а�λ��򣬽������֤һ����ͼ��ASCII�ַ���������תΪhex
			   ��֤��(Hex����) ��570a020b58005b1c50510451525406525006005405535450575004020d51505c5757515406015506
			   ���ͣ���1���ֽ� 1��f���� 0x31^0x66 = 0x57
					 ��2���ֽ� 8��2���� 0x38^0x32 = 0x0a
					 ...(��)
***************************************************************************/
int cmd_tcp_socket::make_register_string(char *send_regstr, const char *stu_no, const char *stu_password, const char *security_str)
{
	/* ��Ҫ�Լ�ʵ�� */
	char MD5_stu_password[33];
	MD5(MD5_stu_password, stu_password);	//�õ�MD5���ܺ�����ļ��ܴ�

	char ori_regstr[41];
	strcpy(ori_regstr, stu_no);
	strcat(ori_regstr, "-");
	strcat(ori_regstr, MD5_stu_password);	//�õ�ԭʼ��֤��

	for (int i = 0; i < 40; i++)	//���ȹ̶�Ϊ40λ "ѧ��+32���ܴ�"
		ori_regstr[i] ^= security_str[i];

	for (int i = 0; i < 40; i++)
		sprintf(send_regstr + 2 * i, "%02x", ori_regstr[i]);

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int game_progress(cmd_tcp_socket &client)
{
	char sel;
	char row, head_row, tail_row;
	int col, head_col, tail_col;
	bool recv_startgame = false;

	while (1) {
		/* �ȴ�Server�˵�gameprogress */
		string spack;
		if (client.get_gameprogress_string(spack) < 0) {
			return -1;
		}
		cout << "ServerӦ�� : " << spack << endl;
		if (spack == "StartGame")
			recv_startgame = true;

		/* û�յ�StartGameǰ����������Ϣ����Ϊ���� */
		if (!recv_startgame)
			return -1;

		if (spack == "GameOver") {
			cout << "����GameID : " << client.get_gameid() << endl;
			cout << "���ε÷�   : " << client.get_score() << endl;
			return 0;
		}

		cout << "1.����һ������" << endl;
		cout << "2.�������ܷɻ�����" << endl;
		while (1) {
			sel = _getch();
			if (sel == '1' || sel == '2')
				break;
		}
		switch (sel) {
			case '1':
				cout << "��������(A-J)��(0-9)���� : ";
				cin >> row >> col; //�˴�δ�ж���ȷ��
				if (row >= 'a' && row <= 'j')
					row -= 32;
				client.send_coordinate(row, col);
				break;
			case '2':
				cout << "�������ͷ��(A-J)��(0-9)���� : ";
				cin >> head_row >> head_col; //�˴�δ�ж���ȷ��
				if (head_row >= 'a' && head_row <= 'j')
					head_row -= 32;
				cout << "�������β������(A-J)��(0-9)���� : ";
				cin >> tail_row >> tail_col; //�˴�δ�ж���ȷ��
				if (tail_row >= 'a' && tail_row <= 'j')
					tail_row -= 32;
				client.send_plane_coordinates(head_row, head_col, tail_row, tail_col);
				break;
		}//end of switch
	}//end of while(1)
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��1 - �Ϸ���	0 - �Ƿ�
  ˵    ����������������
***************************************************************************/
int check(char **argv)
{
	if (strcmp(argv[1], "-auto") != 0 && strcmp(argv[1], "-manual") != 0)	//argv[1]�� -auto -manual �������
		return 0;

	if (strlen(argv[2]) != 7)	//ѧ��7λ
		return 0;

	return 1;
		
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������argv[1] : "-auto"/"-manual"����ʾ�Զ�/�˹���Ϸ
			argv[2] : ѧ��
			argv[3] : ����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char **argv)
{
	if (argc != 4) {
		cout << "Usage : " << argv[0] << " -auto|-manual  stu_no  stu_pwd" << endl;
		return -1;
	}

	/* ��������Լ��δ������Ҫ�Լ���
	   1��argv[1]�� -auto -manual �������
	   2��ѧ��7λ
	   3��... */
	if (!check(argv))
		return -1;

	/* auto�Զ���Ϸδʵ�� */
	if (strcmp(argv[1], "-auto") == 0) {
		cout << "δʵ���Զ���Ϸ" << endl;
		return -1;
	}

	const int sleep_ms = 5000;
	bool first = true;
	cmd_tcp_socket client;

	/* ��client������е�debug���أ�����ʱ�ɴ򿪣���ͼ�ν���ʱ��رգ� */
	client.set_debug_switch(false);

	while (1) {
		if (!first) { //���κδ�����ʱ5����������������һ�Σ�
			cout << "������������ӶϿ�!" << endl;
			Sleep(sleep_ms);
		}
		else
			first = false;

		/* ���ӷ����� */
		if (client.connect() < 0) {
			/* ����ʧ����Ϣ����û��debug_switch����ӡ */
			cout << "���ӷ�����ʧ��!" << endl;
			continue;//return -1;
		}

		/* ��ȡ���������͹����ļ��ܴ���֤��(��40���ַ�) */
		string s1;
		if (client.get_security_string(s1) < 0) {
			client.close();
			continue; // return -2;
		}
		/* ��ӡ */
		cout << "��ȡ�������ܴ� : " << s1 << endl;

		/* ��ѧ��+MD5�����������ܴ����� ���͸�Server��
		   ע�⣺
		   1������һ����80�ֽڣ���β��81
		   2��make_register_string��Ҫ�Լ�ʵ��
		   3���ɲο�tmake_register_string������ʱ�Ὣ�˺�����lib���Ƴ�   */
		char reg_str[81];
		//		client.tmake_register_string(reg_str, "1850000", "185-0*0%0@0", s1.c_str()); //Ԥ�õĲ����û�����ʧЧ��
		client.make_register_string(reg_str, argv[2], argv[3], s1.c_str()); //���Լ���ѧ�ź�����

		/* ����֤�����͹�ȥ */
		client.send_register_string(reg_str);

		/* ������Ϸ��������
		   1�������յ�Server��StartGame�������շ�����
		   2���յ�Server��GameOver�򷵻�0����Ϸ����
		   3���������������-1��������Ϣ����ȷ�ȴ��󣩣��������ٴ��ظ�	*/
		if (game_progress(client) < 0) {
			client.close();
			continue;
		}
		else {
			/* game_progressֻ���յ� GameOver �ŷ��� 0 */
			break;
		}
	};

	client.close();
	cout << "��Ϸ����" << endl;

	return 0;
}

