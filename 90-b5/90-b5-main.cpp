/* ѧ�� ���� �༶ */
#include <iostream>
#include "../common/cmd_console_tools.h"
using namespace std;

int main()
{
	const int MAX_X = 50;
	const int Y = 2;
	const int bg_color = COLOR_HYELLOW;	//����Ϊ����ɫ
	const int fg_color = COLOR_HBLUE;		//ǰ��Ϊ����ɫ
	const char ch = '*';        //�ַ�Ϊ*(ע�⣺����ǿո���ǰ��ɫ�޷���ʾ)
	const int LENGTH = 10;
	int x;

	/* ��һ���ַ����������ƶ� */
	for (x = 0; x < MAX_X; x++) {
		/* ������(x,2)λ�ô�������ӡ10���ַ� */
		showch(x, Y, ch, bg_color, fg_color, LENGTH);

		/* ��ʱ0.1�� */
		Sleep(100);

		if (x < MAX_X - 1) {
			/* �����ʾ(���һ�α���)���������Ϊ��������ɫ+�ո��ػ�һ��ղŵ�λ�� */
			showch(x, Y, ' ', COLOR_BLACK, COLOR_WHITE, LENGTH);
		}
	} //end of for

	//�ָ���ʼ��ɫ
	setcolor();
	cout << endl << "90-b5" << endl << endl;

	return 0;
}
