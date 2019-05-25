// 1752762 计1班 魏鹳达
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include "90-b0.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/cmd_gmw_tools.h"
using namespace std;

//type: 1右 2下 3左 4上
void disp_move_img(const int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_to, const int col_to, const int row_ori, const int col_ori)
{
	if (row_from == row_to && col_from == col_to)
		return;

	path[row_from][col_from] = 2;
	if (path[row_from][col_from + 1] == 1)
	{
		disp_move_single(arr, row_from, col_from, row_ori, col_ori, 1, 4, 3);
		disp_move_img(arr, path, row_from, col_from + 1, row_to, col_to, row_ori, col_ori);
	}
	else if (path[row_from + 1][col_from] == 1)
	{
		disp_move_single(arr, row_from, col_from, row_ori, col_ori, 2, 4, 3);
		disp_move_img(arr, path, row_from + 1, col_from, row_to, col_to, row_ori, col_ori);
	}
	else if (path[row_from][col_from - 1] == 1)
	{
		disp_move_single(arr, row_from, col_from, row_ori, col_ori, 3, 4, 3);
		disp_move_img(arr, path, row_from, col_from - 1, row_to, col_to, row_ori, col_ori);
	}
	else if (path[row_from - 1][col_from] == 1)
	{
		disp_move_single(arr, row_from, col_from, row_ori, col_ori, 4, 4, 3);
		disp_move_img(arr, path, row_from - 1, col_from, row_to, col_to, row_ori, col_ori);
	}
}

int array_print_move_img(int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int *ball_count)
{
	int row_from, row_to, col_from, col_to;
	int flag = 0;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			path[i][j] = 0;
	do
	{
		if (!read_position_img(row_from, col_from, row, col, 0))
			return -1;
	} while (arr[row_from][col_from] <= 0);
	showstr(4 * col_from, 1 + 2 * row_from, "◎", 7 + arr[row_from][col_from], COLOR_HWHITE);
	do
	{
		while (1)
		{
			if (!read_position_img(row_to, col_to, row, col, 0))
				return -1;
			if (arr[row_to][col_to] > 0)
			{
				showstr(4 * col_from, 1 + 2 * row_from, "〇", 7 + arr[row_from][col_from], COLOR_HWHITE);
				row_from = row_to;
				col_from = col_to;
				showstr(4 * col_from, 1 + 2 * row_from, "◎", 7 + arr[row_from][col_from], COLOR_HWHITE);
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