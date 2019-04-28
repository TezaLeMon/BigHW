// 1752762 计1班 魏鹳达
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include "cmd_console_tools.h"
#include "common_graphics.h"
using namespace std;

void draw_edge_text(const int row, const int col, const int x_position, const int y_position)
{
	int i;
	showstr(x_position, y_position, "  |");
	for (i = 1; i <= col; i++)
		showch(x_position + 3 * i + 2, y_position, i + '0');
	showstr(x_position, y_position + 1, "--+-");
	showstr(x_position + 4, y_position + 1, "---", 0, 7, col);
	for (i = 1; i <= row; i++) {
		showch(x_position, y_position + 1 + i, 'A' + i - 1);
		cout << " |";
	}
	cout << endl;
}

void draw_ball_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int color = 1)
{
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
		{
			if (color&&arr[i][j])
				showch(2 + x_position + 3 * (j - 1), y_position + i - 1, '0' + arr[i][j], COLOR_HYELLOW, arr[i][j]);
			else
				showch(2 + x_position + 3 * (j - 1), y_position + i - 1, '0' + arr[i][j]);
		}
	setcolor();
}

void draw_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int color)
{
	draw_edge_text(row, col, x_position, y_position);
	draw_ball_text(arr, row, col, x_position + 3, y_position + 2, color);
}

void draw_edge_img_no_div(const int row, const int col, const int x_position, const int y_position, const int is_disp_ch)
{
	int i;
	showstr(x_position + 2, 1 + y_position, "╔", COLOR_HWHITE, COLOR_HBLACK);
	showstr(x_position + 4, 1 + y_position, "═", COLOR_HWHITE, COLOR_HBLACK, col);
	showstr(x_position + 2 * (col + 1) + 2, 1 + y_position, "╗", COLOR_HWHITE, COLOR_HBLACK);
	for (i = 1; i <= row; i++) {
		showstr(x_position + 2, 1 + y_position + i, "║", COLOR_HWHITE, COLOR_HBLACK);
		showstr(x_position + 2 * (col + 1) + 2, 1 + y_position + i, "║", COLOR_HWHITE, COLOR_HBLACK);
	}
	showstr(x_position + 2, 1 + y_position + row + 1, "╚", COLOR_HWHITE, COLOR_HBLACK);
	showstr(x_position + 4, 1 + y_position + row + 1, "═", COLOR_HWHITE, COLOR_HBLACK, col);
	showstr(x_position + 2 * (col + 1) + 2, 1 + y_position + row + 1, "╝", COLOR_HWHITE, COLOR_HBLACK);
	if (is_disp_ch) {
		for (i = 1; i <= row; i++)
			showch(x_position, y_position + 1 + i, 'A' + i - 1);
		for (i = 1; i <= col; i++)
			showch(x_position + 3 + 2 * i, y_position, '0' + i);
	}
}

void draw_ball_img_no_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position)
{
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++) {
			if (arr[i][j])
				showstr(x_position + 2 * j + 2, y_position + 1 + i, "〇", 6 + arr[i][j], COLOR_BLACK);
			else
				showstr(x_position + 2 * j + 2, y_position + 1 + i, "〇", COLOR_HWHITE, COLOR_HWHITE);
		}
}

void draw_img_no_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int is_disp_ch)
{
	draw_edge_img_no_div(row, col, x_position, y_position, is_disp_ch);
	draw_ball_img_no_div(arr, row, col, x_position, y_position);
}


void draw_edge_img_div(const int row, const int col, const int x_position, const int y_position)
{
	int i;
	showstr(x_position + 2, 1 + y_position, "╔", COLOR_HWHITE, COLOR_HBLACK);
	showstr(x_position + 4, 1 + y_position, "═╦", COLOR_HWHITE, COLOR_HBLACK, col - 1);
	showstr(x_position + 4 * col, 1 + y_position, "═╗", COLOR_HWHITE, COLOR_HBLACK);
	for (i = 1; i <= row; i++) {
		showstr(x_position + 2, y_position + 2 * i, "║  ", COLOR_HWHITE, COLOR_HBLACK, col);
		cout << "║";
		if (i == row)
			break;
		showstr(x_position + 2, 1 + y_position + 2 * i, "╠═", COLOR_HWHITE, COLOR_HBLACK);
		showstr(x_position + 6, 1 + y_position + 2 * i, "╬═", COLOR_HWHITE, COLOR_HBLACK, col - 1);
		cout << "╣";
	}
	showstr(x_position + 2, y_position + 2 * row + 1, "╚", COLOR_HWHITE, COLOR_HBLACK);
	showstr(x_position + 4, y_position + 2 * row + 1, "═╩", COLOR_HWHITE, COLOR_HBLACK, col - 1);
	cout << "═╝";
	for (i = 1; i <= row; i++)
		showch(x_position, y_position + 2 * i, 'A' + i - 1);
	for (i = 1; i <= col; i++)
		showch(x_position + 4 * i, y_position, '0' + i);
}

void draw_ball_img_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_color[MAX_ROW + 2][MAX_COL + 2] = NULL, const int fg_color = COLOR_WHITE)
{
	if (arr_color == NULL)
		for (int i = 1; i <= row; i++)
			for (int j = 1; j <= col; j++)
				if (arr[i][j])
					showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "〇", 6 + arr[i][j], COLOR_BLACK);
				else
					showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "〇", COLOR_HWHITE, COLOR_HWHITE);
	else
		for (int i = 1; i <= row; i++)
			for (int j = 1; j <= col; j++)
				if (arr[i][j])
					if(arr_color[i][j])
						showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "●", 6 + arr[i][j], fg_color);
					else
						showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "〇", 6 + arr[i][j], COLOR_BLACK);
				else
					showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "〇", COLOR_HWHITE, COLOR_HWHITE);

	gotoxy(x_position, y_position + 2 * row + 2);
	setcolor();
}

void draw_ball_img_div_extra(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_selected[MAX_ROW + 2][MAX_COL + 2], const char *s)
{
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			if (arr_selected[i][j]){
				Sleep(100);
				showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, s, 6 + arr[i][j], COLOR_BLACK);
			}
	gotoxy(x_position, y_position + 2 * row + 2);
	setcolor();
}


void draw_img_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_color[MAX_ROW + 2][MAX_COL + 2], const int fg_color)
{
	draw_edge_img_div(row, col, x_position, y_position);
	draw_ball_img_div(arr, row, col, x_position + 4, y_position + 2, arr_color, fg_color);
}

void draw_img_removeball(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int eliminate[MAX_ROW + 2][MAX_COL + 2])
{
	int i, j;
	for (i = 1; i <= row; i++)
		for (j = 1; j <= col; j++)
			if(eliminate[i][j]){
				Sleep(100);
				showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "〇", COLOR_BLACK, arr[i][j] + 6);
				Sleep(100);
				showstr(x_position + 4 * j - 4, y_position + 2 * i - 2, "〇", COLOR_HWHITE, COLOR_HWHITE);
			}
}

void disp_move_single(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_ori, const int col_ori, const int type, const int x_position, const int y_position)
{
	if (type % 2)	//水平
	{
		int t = 2 - type;
		showstr(x_position - 4 + 4 * col_from, y_position - 2 + 2 * row_from, "〇", COLOR_HWHITE, COLOR_HWHITE);
		Sleep(50);
		showstr(x_position - 4 + 4 * col_from + 2 * t, y_position - 2 + 2 * row_from, "〇", 6 + arr[row_ori][col_ori], COLOR_BLACK);
		Sleep(50);
		showstr(x_position - 4 + 4 * col_from + 2 * t, y_position - 2 + 2 * row_from, "║", COLOR_HWHITE, COLOR_HBLACK);
		Sleep(50);
		showstr(x_position - 4 + 4 * col_from + 4 * t, y_position - 2 + 2 * row_from, "〇", 6 + arr[row_ori][col_ori], COLOR_BLACK);
		Sleep(50);
	}
	else
	{
		int t = 3 - type;
		showstr(x_position - 4 + 4 * col_from, y_position - 2 + 2 * row_from, "〇", COLOR_HWHITE, COLOR_HWHITE);
		Sleep(50);
		showstr(x_position - 4 + 4 * col_from, y_position - 2 + 2 * row_from + t, "〇", 6 + arr[row_ori][col_ori], COLOR_BLACK);
		Sleep(50);
		showstr(x_position - 4 + 4 * col_from, y_position - 2 + 2 * row_from + t, "═", COLOR_HWHITE, COLOR_HBLACK);
		Sleep(50);
		showstr(x_position - 4 + 4 * col_from, y_position - 2 + 2 * row_from + 2 * t, "〇", 6 + arr[row_ori][col_ori], COLOR_BLACK);
		Sleep(50);
	}
	setcolor();
}
