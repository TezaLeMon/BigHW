// 1752762 计1班 魏鹳达
#include <iostream>
#include <conio.h>
#include <string.h>
#include <iomanip>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
using namespace std; 

void input_int(const char *s, int *target, const int low, const int high, const int x, const int y)
{
	while (1) {
		showstr(x, y, s);
		cin >> *target;
		showstr(x, y + 1, "                          ");
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (*target >= low && *target <= high)
			break;
		showstr(x + strlen(s), y, "                      ");
		showstr(x, y + 1, "输入不合法，请重新输入");
	}
}

void input_str2(const char *s, char *target, const int row_low, const int row_high, const int col_low, const int col_high)
{
	int x, y;
	getxy(x, y);
	cout << s;
	while (1) {
		showstr(strlen(s), y, "                      ");
		gotoxy(strlen(s), y);
		cin >> target[0];
		cin >> target[1];
		showstr(0, y + 1, "                          ");
		target[2] = '\0';
		cin.ignore(1024, '\n');
		if (target[0] >= 'A' && target[0] <= 'Z')
			target[0] = target[0] - 'A' + 'a';
		if (target[0] >= row_low && target[0] <= row_high && target[1] >= col_low && target[1] <= col_high)
			break;
		showstr(0, y + 1, "输入不合法，请重新输入");
	}
	gotoxy(0, y + 1);
}
