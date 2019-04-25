// 1752762 计1班 魏鹳达
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include "90-b0.h"
#include "../common/cmd_console_tools.h"
using namespace std;

int array_print_move_text(int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int move_flag)
{
	char from[3], to[3];
	int x, y;
	while (1)
	{
		input_str2("请以字母+数字形式[例：c2]输入要移动球的矩阵坐标：", from, 'a', 'a' + row - 1, '1', '1' + col - 1);
		if (arr[from[0] - 'a' + 1][from[1] - '1' + 1] > 0)
			break;
		getxy(x, y);
		cout << "起始位置为空，请重新输入";
		gotoxy(x, y - 1);
	}
	cout << "输入为" << char(from[0] - 'a' + 'A') << "行" << from[1] << "列" << endl;
	while (1)
	{
		input_str2("请以字母+数字形式[例：c2]输入要移动球的目的坐标：", to, 'a', 'a' + row - 1, '1', '1' + col - 1);
		if (arr[to[0] - 'a' + 1][to[1] - '1' + 1] == 0)
			break;
		getxy(x, y);
		cout << "移动位置非空，请重新输入";
		gotoxy(x, y - 1);
	}
	cout << "输入为" << char(to[0] - 'a' + 'A') << "行" << to[1] << "列" << endl;
	int flag = 0;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			path[i][j] = 0;
	if (!path_find(arr, path, from[0] - 'a' + 1, from[1] - '1' + 1, to[0] - 'a' + 1, to[1] - '1' + 1, flag))
	{
		cout << "无法找到移动路径" << endl;
		return 0;
	}
	else if (move_flag)
	{
		array_move(arr, from[0] - 'a' + 1, from[1] - '1' + 1, to[0] - 'a' + 1, to[1] - '1' + 1);
		if (remove(arr, to[0] - 'a' + 1, to[1] - '1' + 1, row, col))
			return 2;
	}
	return 1;
}

void path_print_result_text(const int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col)
{
	int i, j;
	cout << endl << "查找结果数组：" << endl << endl;
	cout << "  |";
	for (i = 1; i <= col; i++)
		cout << setw(3) << i;
	cout << endl << "--+-";
	for (i = 1; i <= col; i++)
		cout << "---";
	for (i = 1; i <= row; i++)
	{
		cout << endl;
		cout << char('A' + i - 1) << " |";
		for (j = 1; j <= col; j++)
		{
			cout << "  ";
			if (path[i][j] == 1)
				cout << "*";
			else
				cout << "0";
		}
	}
	cout << endl;
}

void path_print_move_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col)
{
	int i, j;
	cout << endl << "移动路径（不同色标识）：" << endl << endl;
	cout << "  |";
	for (i = 1; i <= col; i++)
		cout << setw(3) << i;
	cout << endl << "--+-";
	for (i = 1; i <= col; i++)
		cout << "---";
	for (i = 1; i <= row; i++)
	{
		cout << endl;
		cout << char('A' + i - 1) << " |";
		for (j = 1; j <= col; j++)
		{
			cout << "  ";
			if (path[i][j] == 1)
				setcolor(COLOR_HYELLOW, COLOR_HBLUE);
			cout << arr[i][j];
			setcolor();
		}
	}
	cout << endl;
}

int array_print_move_img(int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int *ball_count, const int can_keyboard)
{
	int row_from, row_to, col_from, col_to;
	int flag = 0;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			path[i][j] = 0;
	do
	{
		if (!read_position_img(row_from, col_from, row, col, can_keyboard))
			return -1;
	} while (arr[row_from][col_from] <= 0);
	showstr(4 * col_from - 2, 2 * row_from, "◎", 7 + arr[row_from][col_from], COLOR_HWHITE);
	do
	{
		while (1)
		{
			if (!read_position_img(row_to, col_to, row, col, can_keyboard))
				return -1;
			if (arr[row_to][col_to] > 0)
			{
				showstr(4 * col_from - 2, 2 * row_from, "〇", 7 + arr[row_from][col_from], COLOR_HWHITE);
				row_from = row_to;
				col_from = col_to;
				showstr(4 * col_from - 2, 2 * row_from, "◎", 7 + arr[row_from][col_from], COLOR_HWHITE);
				continue;
			}
			break;
		}
	} while (!path_find(arr, path, row_from, col_from, row_to, col_to, flag));
	disp_move_img(arr, path, row_from, col_from, row_to, col_to, row_from, col_from);
	gotoxy(0, 2 + 2 * row);
	array_move(arr, row_from, col_from, row_to, col_to);
	if (remove(arr, row_to, col_to, row, col, ball_count))
		return 2;
	return 1;
}