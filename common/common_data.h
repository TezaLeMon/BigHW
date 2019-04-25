// 1752762 计1班 魏鹳达
#include "common_settings.h"
#include <iostream>
#include <iomanip>
using namespace std;

//菜单函数 serial_number存放序号 prompt存放提示语
template <int size>
char menu(char *serial_number, char(*prompt)[size])
{
	cls();
	int i;
	gotoxy(0, 1);
	for (i = 0; serial_number[i] != '\0'; i++)
	{
		cout << setw(2) << serial_number[i] << '.' << prompt[i] << endl;	//未检查提示语不合法的情况
		int t = strlen(prompt[i]);
	}
	showch(0, 0, '-', 0, 7, size + 3);
	showch(0, strlen(serial_number) + 1, '-', 0, 7, size + 3);
	cout << endl << "[请选择] ";
	char ch;
	while (1)
	{
		ch = (char)_getch();
		for (i = 0; serial_number[i] != '\0'; i++)
		{
			if (serial_number[i] == ch)
				break;
			else if (ch >= 'a'&&ch <= 'z' && (ch - 'a' + 'A') == serial_number[i])
			{
				ch = ch - 'a' + 'A';
				break;
			}
			else if (ch >= 'A'&&ch <= 'Z' && (ch - 'A' + 'a') == serial_number[i])
				break;
		}
		if (serial_number[i] != '\0') {
			cout << ch << endl;
			to_be_continued(NULL);
			break;
		}
	}
	return ch;
}
