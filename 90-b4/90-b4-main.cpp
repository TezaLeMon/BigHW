/* ѧ�� ���� �༶ */
#include <iostream>
#include "../common/cmd_console_tools.h"
using namespace std;

int main()
{
	int x, y;

	/* ��ʾ16*16����ɫ��ÿ���ַ�֮���һ���ո� */
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++)
			showch(x * 2, y, 'x', y, x, 2);	//��xyλ����ʾx����x�����ֵ��ǰ��ɫ��y�����ֵ������ɫ����ʾ����x��Ϊ��ʹ����Э�������Ի���һ��x��۲�Ч����
		cout << endl; //16��ǰ��ɫ�任��ɺ󣬻�һ��
		Sleep(100);  //��ʱ0.1�루�Ժ���Ϊ��λ��1000=1�룩
	} //end of for

	//�ָ���ʼ��ɫ
	setcolor();
	cout << endl << "90-b4" << endl << endl;

	return 0;
}
