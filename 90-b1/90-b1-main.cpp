// 1752762 ��1�� κ�ٴ�
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/common_data.h"
using namespace std;

int main()
{
	srand((unsigned)time(NULL));
	setcursor(CURSOR_INVISIBLE);
	char serial_number[] = { "ABCDEFGHIQ" };
	char prompt[][52] = {	"�ڲ����飬���ɳ�ʼ״̬��Ѱ���Ƿ��г�ʼ��������",
							"�ڲ����飬������ʼ����������0�����䲢��0���",
							"�ڲ����飬������ʼ������������������ʾ",
							"n*n�Ŀ��(�޷ָ���)����ʾ��ʼ״̬",
							"n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬",
							"n*n�Ŀ��(�зָ���)����ʾ��ʼ״̬����ʼ��������",
							"n*n�Ŀ��(�зָ���)��������ʼ�����������ʾ������ʾ",
							"cmdͼ�ν���������",
							"���ļ��ж�ȡ��������֤����������ʾ���㷨����ȷ��",
							"�˳�" };
	char t_menu;
	while (1){
		t_menu = menu(serial_number, prompt);
		if (t_menu == serial_number[strlen(serial_number) - 1])
			break;
		play(t_menu);
	}
	return 0;
}