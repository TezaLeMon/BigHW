// 1752762 ��1�� κ�ٴ�
//	�����̷�ʽ������BUG
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/common_data.h"
#include "90-b0.h"
using namespace std;
int menu()
{
	cout << "---------------------------------------------" << endl;
	cout << "1.�ڲ����飬������ɳ�ʼ5����" << endl;
	cout << "2.�ڲ����飬�������60%����Ѱ���ƶ�·��" << endl;
	cout << "3.�ڲ����飬������" << endl;
	cout << "4.����n*n�Ŀ�ܣ��޷ָ��ߣ��������ʾ5����" << endl;
	cout << "5.����n*n�Ŀ�ܣ��зָ��ߣ��������ʾ5����" << endl;
	cout << "6.n*n�Ŀ�ܣ�60%����֧����꣬���һ���ƶ�" << endl;
	cout << "7.cmdͼ�ν���������" << endl;
	cout << "8.cmdͼ�ν��������� - ֧��ͬʱ����������ӷ֣�" << endl;
	cout << "0.�˳�" << endl;
	cout << "---------------------------------------------" << endl;
	cout << "[��ѡ��0 - 8] ";
	int ch;
	while (1)
	{
		ch = _getch();
		if (ch >= '0' && ch <= '8')
		{
			cout << ch - '0' << endl;
			to_be_continued(NULL);
			break;
		}
	}
	return ch;
}
int main()
{
	srand((unsigned)time(NULL));
	int arr[MAX_ROW + 2][MAX_COL + 2], row, col, ball[3];
	int path[MAX_ROW + 2][MAX_COL + 2] = { 0 };
	int ball_conut[8] = { 0 };
	setcursor(CURSOR_INVISIBLE);

	//	PS ���ְ���ʹ���ϴ���
	CONSOLE_GRAPHICS_INFO ColorLinez_CGI; //����һ��CGI����
	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&ColorLinez_CGI);
	gmw_set_rowno_switch(&ColorLinez_CGI, true);							//��ʾ�к�
	gmw_set_colno_switch(&ColorLinez_CGI, true);							//��ʾ�б�
	gmw_set_frame_default_linetype(&ColorLinez_CGI, 1);					//��Ϸ���������ͣ�˫��
	gmw_set_delay(&ColorLinez_CGI, DELAY_OF_BLOCK_MOVED, 50);	//��ʱ
	const BLOCK_DISPLAY_INFO bdi[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
	{1,  COLOR_HBLACK, -1, "��"},	//����״̬����1����Ӧ�Ĳ���
	{2,  COLOR_YELLOW, -1, "��"},
	{3,  COLOR_HGREEN, -1, "��"},
	{4,  COLOR_HCYAN, -1, "��"},
	{5,  COLOR_HRED, -1, "��"},
	{6,  COLOR_HPINK, -1, "��"},
	{7,  COLOR_HYELLOW, -1, "��"},
	{11, COLOR_HBLACK, -1, "��"},//ѡ��״̬����1����Ӧ�Ĳ���ͼ��
	{12, COLOR_YELLOW, -1, "��"},
	{13, COLOR_HGREEN, -1, "��"},
	{14, COLOR_HCYAN, -1, "��"},
	{15, COLOR_HRED, -1, "��"},
	{16, COLOR_HPINK, -1, "��"},
	{17, COLOR_HYELLOW, -1, "��"},
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������Ϊ-999
	};
	const int BDI_NORMAL = 0, BDI_SELECTED = 10;
	char temp[256];

	while (1)
	{
		setconsoleborder(80, 25, 80, 1000);
		setfontsize("Terminal", 16, 8);
		for (int i = 1; i < 8; i++)
			ball_conut[i] = 0;
		int choice = menu();
		if (choice == '0')
			return 0;
		cls();
		input_int("����������(7-9)�� ", &row, 7, 9, 0, 0);
		input_int("����������(7-9)�� ", &col, 7, 9, 0, 1);

		/* ��row/col��ֵ������Ϸ���������� */
		gmw_set_rowcol(&ColorLinez_CGI, row, col);
		cls();
		switch (choice)
		{
		case '1':
			array_create(arr, row, col, 5);
			cout << "��ʼ���飺";
			draw_text(arr, row, col, 0, 2);
			break;
		case '2':
			array_create(arr, row, col, int(row*col*0.6));
			cout << "��ǰ���飺";
			draw_text(arr, row, col, 0, 2);
			ball_creat_print(ball, 0);
			if (array_print_move_text(arr, path, row, col))
			{
				path_print_result_text(path, row, col);
				path_print_move_text(arr, path, row, col);
			}
			break;
		case '3':
			array_create(arr, row, col, 5);
			while (!is_over(arr, row, col))
			{
				static int ball_flag = 0;
				cout << endl << "��ǰ���飺";
				draw_text(arr, row, col, 0, 2);
				ball_creat_print(ball, ball_flag);
				int t = array_print_move_text(arr, path, row, col, 1);
				if (t == 1)
				{
					for (int i = 0; i < 3; i++)
					{
						array_add(arr, row, col, ball[i]);
						if (is_over(arr,row,col))
							i = 3;
					}
					ball_flag = 0;
				}
				else
					ball_flag = 1;
				if (t)
				{
					cls();
					cout << "�ƶ������飺";
					draw_text(arr, row, col, 0, 2);
				}
			}
			break;
		case '4':
			cout << "��ʼ���飺";
			array_create(arr, row, col, 5);
			draw_text(arr, row, col, 0, 2);
			to_be_continued(NULL);
			//setconsoleborder(26 + col, 15, 26 + col, 15);
			//setfontsize("������", 28);
			//cout << "��Ļ��15��" << 26 + col << "��" << endl;
			//draw_img_no_div(arr, row, col, 0, 1);

			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, false);					//��Ϸ��������ÿ��ɫ���2��1���޷ָ���
			/* ��ʾ��� */
			gmw_draw_frame(&ColorLinez_CGI);

			/* ��״̬����ʾ���� */
			sprintf(temp, "%s-���ڴ�С��%d�� %d��", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);

			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++) {
					gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
				}
			break;
		case '5':

			array_create(arr, row, col, 5);
			cout << "��ʼ���飺";
			draw_text(arr, row, col, 0, 2);
			to_be_continued(NULL);
			//setconsoleborder(4 * (col + 2) + 3, 2 * row + 5, 4 * (col + 2) + 2, 2 * row + 5);
			//setfontsize("������", 28);
			//cout << "��Ļ��" << 2 * row + 5 << "��" << 4 * (col + 2) + 2 << "��" << endl;
			//draw_img_div(arr, row, col, 0, 1);

			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, true);					//��Ϸ��������ÿ��ɫ���2��1���޷ָ���
			/* ��ʾ��� */
			gmw_draw_frame(&ColorLinez_CGI);

			/* ��״̬����ʾ���� */
			sprintf(temp, "%s-���ڴ�С��%d�� %d��", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);


			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++) {
					gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
				}

			break;
		case '6':
			array_create(arr, row, col, int(row*col*0.6));
			//setconsoleborder(4 * (col + 2) + 3, 2 * row + 5, 4 * (col + 2) + 2, 2 * row + 5);
			//setfontsize("������", 28);
			//cout << "��Ļ��" << 2 * row + 5 << "��" << 4 * (col + 2) + 2 << "��" << endl;
			//draw_img_div(arr, row, col, 0, 1);

			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, true);					//��Ϸ��������ÿ��ɫ���2��1���޷ָ���

			/* ��ʾ��� */
			gmw_draw_frame(&ColorLinez_CGI);
			/* ��״̬����ʾ���� */
			sprintf(temp, "%s-���ڴ�С��%d�� %d��", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);

			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++) {
					gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
				}

			array_print_move_img(arr, path, row, col, NULL, 0, &ColorLinez_CGI, bdi);
			break;
		case '7':
		case '8':
			array_create(arr, row, col, 5);
			//setconsoleborder(4 * 9 + 32, 2 * row + 5, 4 * 9 + 2, 2 * row + 5);
			//setfontsize("������", 28);

			gmw_set_ext_rowcol(&ColorLinez_CGI, 0, 0, 0, 30);
			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, true);					//��Ϸ��������ÿ��ɫ���2��1���޷ָ���

			/* ��ʾ��� */
			gmw_draw_frame(&ColorLinez_CGI);
			/* ��״̬����ʾ���� */
			sprintf(temp, "%s-���ڴ�С��%d�� %d��", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);

			while (!is_over(arr, row, col))
			{
				static int ball_flag = 0;
				//cls();
				//cout << "��Ļ��" << 2 * row + 5 << "��" << 4 * col + 2 << "�У��Ҽ��˳���" << endl;
				//draw_img_div(arr, row, col, 0, 1);

				for (int i = 1; i <= row; i++)
					for (int j = 1; j <= col; j++) {
						gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
					}

				ball_creat_print_img(ball, ball_flag);
				score_print(ball_conut);
				ball_count_print(arr, ball_conut, row, col);
				int t = array_print_move_img(arr, path, row, col, ball_conut, choice - '7', &ColorLinez_CGI, bdi);
				if (t == 1){
					for (int i = 0; i < 3; i++){
						array_add(arr, row, col, ball[i]);
						if (is_over(arr, row, col))
							i = 3;
					}
					ball_flag = 0;
				}
				else if (t == -1)
					break;
				else
					ball_flag = 1;
			}
			break;
		default:
			cout << "error" << endl;
			break;
		}
		to_be_continued("��С�����");
	}
	return 0;
}