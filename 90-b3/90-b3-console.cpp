// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <string.h>
#include "90-b3.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
using namespace std;

void disp_booklist(char ***book_list, const int x_position, const int y_position, const int row, const int col)
{
	showstr(x_position, y_position, "请选择文件");
	draw_edge_img_no_div(row, col, x_position - 2, y_position);
	for (int i = 0; i < row&&(*book_list)[i]; i++) {
		int len = strlen((*book_list)[i]) - 3;	// + 1 - strlen(".txt")
		char *temp = new char[len];
		strncpy(temp, (*book_list)[i], len - 1);
		temp[len] = '\0';
		showstr(x_position + 2, y_position + 2 + i, temp);
		cout.clear();
		cout << endl;
	}
}