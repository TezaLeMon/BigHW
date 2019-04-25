// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
using namespace std;


void arr_reset(int arr[MAX_ROW + 2][MAX_COL + 2])
{
	for (int i = 0; i < MAX_ROW + 2; i++)
		for (int j = 0; j < MAX_COL + 2; j++)
			arr[i][j] = 0;
}

void array_create(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col)
{
	int i, j;
	//初始化 边界为-1 内部全为0
	for (i = 0; i <= col + 1; i++)
	{
		arr[0][i] = -1;
		arr[row + 1][i] = -1;
	}
	for (i = 1; i <= row; i++)
	{
		arr[i][0] = -1;
		arr[i][col + 1] = -1;
		for (j = 1; j <= col; j++)
			arr[i][j] = 1 + rand() % 7;
	}
}

void ball_remove(int arr[MAX_ROW + 2][MAX_COL + 2], const int row_t, const int col_t, int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2])
{
	eliminate_BALL[row_t][col_t] = 0;

	if (eliminate_BALL[row_t + 1][col_t] && arr[row_t + 1][col_t] == arr[row_t][col_t])	//下
		ball_remove(arr, row_t + 1, col_t, eliminate_BALL);
	if (eliminate_BALL[row_t][col_t + 1] && arr[row_t][col_t + 1] == arr[row_t][col_t])	//右
		ball_remove(arr, row_t, col_t + 1, eliminate_BALL);
	if (eliminate_BALL[row_t - 1][col_t] && arr[row_t - 1][col_t] == arr[row_t][col_t])	//上
		ball_remove(arr, row_t - 1, col_t, eliminate_BALL);
	if (eliminate_BALL[row_t][col_t - 1] && arr[row_t][col_t - 1] == arr[row_t][col_t])	//左
		ball_remove(arr, row_t, col_t - 1, eliminate_BALL);

	arr[row_t][col_t] = 0;
}

void ball_fall(int arr[MAX_ROW + 2][MAX_COL + 2], const int row_foot, const int col_foot)
{
	int t = 1;
	while (arr[row_foot - t][col_foot] == 0)
		t++;
	if (arr[row_foot - t][col_foot] == -1)	//上方无球（循环到边界
		return;
	arr[row_foot][col_foot] = arr[row_foot - t][col_foot];
	arr[row_foot - t][col_foot] = 0;
}

void ball_fall_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int is_draw, const int x_position, const int y_position)
{
	int i, j;
	for (i = row; i > 1; i--)	//最上层上方肯定无球
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 0){
				if (is_draw) {
					int t = i - 1, k = 0;
					while (arr[t][j] == 0)
						t--;
					if (arr[t][j] == -1)
						continue;
					while (t + k != i)
						disp_move_single(arr, t + k++, j, t, j, 2, x_position, y_position);
				}
				ball_fall(arr, i, j);
			}
}

void ball_add(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int ball_add[MAX_ROW + 2][MAX_ROW + 2])
{
	if(ball_add)
		arr_reset(ball_add);
	int i, j;
	for (i = 1; i <= 5; i++)	//至多上五层无球
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 0){
				arr[i][j] = 1 + rand() % 9;
				if(ball_add)
					ball_add[i][j] = 1;
			}

}

void ball_remove_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col,int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2])
{
	int i, j;
	for (i = 1; i <= row; i++)
		for (j = 1; j <= col; j++)
			if (eliminate_BALL[i][j])
				ball_remove(arr, i, j, eliminate_BALL);
}

//检查是否可以消除 可/否消除返回1/0 将可消除球位置标记存放在eliminate_ball数组中
int is_eliminate(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_ball[MAX_ROW + 2][MAX_COL + 2] = NULL)
{
	int i, j, from, n_count, flag = 0;
	//横向
	for (i = 1; i <= row; i++){
		n_count = 1;
		from = 1;
		for (j = 2; j <= col; j++)
			if (arr[i][j] == arr[i][from])
				n_count++;
			else
			{
				if (n_count >= 3){
					flag = 1;
					if(eliminate_ball)
						for (int t = from; t < j; t++)
							eliminate_ball[i][t] = 1;	//将可消除球的位置做标记
				}
				n_count = 1;
				from = j;
			}
	}

	//竖向
	for (i = 1; i <= col; i++) {
		n_count = 1;
		from = 1;
		for (j = 2; j <= row; j++)
			if (arr[j][i] == arr[from][i])
				n_count++;
			else
			{
				if (n_count >= 3){
					flag = 1;
					if (eliminate_ball)
						for (int t = from; t < j; t++)
							eliminate_ball[t][i] = 1;	//横竖均有消除的在消除函数中判断
				}
				n_count = 1;
				from = j;
			}
	}
	return flag;
}

void ball_exchange(int arr[MAX_ROW + 2][MAX_COL + 2], const int row1, const int col1, const int row2, const int col2)
{
	int t = arr[row1][col1];
	arr[row1][col1] = arr[row2][col2];
	arr[row2][col2] = t;
}


int find_notice(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_notice[MAX_ROW + 2][MAX_COL + 2])
{
	int i, j, flag = 0;
	//查找横向移动后是否可消除
	for (i = 1; i <= row; i++)
		for (j = 1; j < col; j++)
		{
			ball_exchange(arr, i, j, i, j + 1);
			if (is_eliminate(arr, row, col)){
				flag = 1;
				eliminate_notice[i][j] = 1;
				eliminate_notice[i][j + 1] = 1;
			}
			ball_exchange(arr, i, j, i, j + 1);
		}

	//查找竖向移动后是否可消除
	for (i = 1; i < row; i++)
		for (j = 1; j <= col; j++)
		{
			ball_exchange(arr, i, j, i + 1, j);
			if (is_eliminate(arr, row, col)){
				flag = 1;
				eliminate_notice[i][j] = 1;
				eliminate_notice[i + 1][j] = 1;
			}
			ball_exchange(arr, i, j, i + 1, j);
		}
	return flag;
}

void play(char mode)
{
	int choice, row, col, arr_ball[MAX_ROW + 2][MAX_COL + 2], arr_selected[MAX_ROW + 2][MAX_COL + 2];
	if (mode >= '0'&&mode <= '9')
		choice = mode - '0' + 1;
	else
		choice = mode - 'A' + 1;
	cls();
	input_int("请输入行数(5-9)： ", &row, 5, 9, 0, 0);
	input_int("请输入列数(5-9)： ", &col, 5, 9, 0, 1);
	array_create(arr_ball, row, col);
	arr_reset(arr_selected);
	switch (choice)
	{
		case 1:
			cout << "\n初始数组：";
			draw_text(arr_ball, row, col, 0, 4, 0);
			to_be_continued("进行寻找初始可消除项的操作");
			if (is_eliminate(arr_ball, row, col, arr_selected))
				print_result_text(arr_ball, arr_selected, row, col);
			else
				cout << "初始已无可消除项" << endl;
			break;
		case 2:
			cout << "\n初始数组：";
			draw_text(arr_ball, row, col, 0, 4, 0);
			to_be_continued("进行寻找初始可消除项的操作");
			while (is_eliminate(arr_ball, row, col, arr_selected)) {
				print_result_text(arr_ball, arr_selected, row, col);
				to_be_continued("按回车键进行数组下落及填充操作");
				ball_remove_all(arr_ball, row, col, arr_selected);
				ball_fall_all(arr_ball, row, col);
				ball_add(arr_ball, row, col);
				cout << "消除及填充后数组：" << endl;
				print_text(arr_ball, row, col);
				to_be_continued("继续进行寻找初始可消除项的操作");
			}
			cout << "初始已无可消除项" << endl;
			break;
		case 3:
			cout << "\n初始数组：";
			draw_text(arr_ball, row, col, 0, 4, 0);
			to_be_continued("进行寻找初始可消除项的操作");
			while (is_eliminate(arr_ball, row, col, arr_selected)) {
				print_result_text(arr_ball, arr_selected, row, col);
				to_be_continued("按回车键进行数组下落及填充操作");
				ball_remove_all(arr_ball, row, col, arr_selected);
				ball_fall_all(arr_ball, row, col);
				ball_add(arr_ball, row, col);
				cout << "消除及填充后数组：" << endl;
				print_text(arr_ball, row, col);
				to_be_continued("继续进行寻找初始可消除项的操作");
			}
			cout << "初始已无可消除项" << endl;
			if (find_notice(arr_ball, row, col, arr_selected)) {
				cout << "\n可选择的消除提示：" << endl;
				print_result_text(arr_ball, arr_selected, row, col);
			}
			else
				cout << "无可选择的消除提示，Game Over" << endl;
			break;
		case 4:
		case 5:
			cout << "\n初始数组：";
			draw_text(arr_ball, row, col, 0, 4, 0);
			while (is_eliminate(arr_ball, row, col, arr_selected)) {
				to_be_continued("进行寻找初始可消除项的操作");
				print_result_text(arr_ball, arr_selected, row, col);
				to_be_continued("按回车键进行数组下落及填充操作");
				ball_remove_all(arr_ball, row, col, arr_selected);
				ball_fall_all(arr_ball, row, col);
				ball_add(arr_ball, row, col);
				cout << "消除及填充后数组：" << endl;
				print_text(arr_ball, row, col);
			}
			to_be_continued(NULL);
			cls();
			if (choice - 4)
				draw_img_div(arr_ball, row, col, 0, 1);
			else
				draw_img_no_div(arr_ball, row, col, 0, 1);
			break;
		case 6:
			cls();
			if (is_eliminate(arr_ball, row, col, arr_selected))
				draw_img_div(arr_ball, row, col, 0, 1, arr_selected, COLOR_BLACK);
			else
			{
				draw_img_div(arr_ball, row, col, 0, 1);
				showstr(14, 0, "未找到可消除项");
			}
			gotoxy(0, 2 * row + 3);
			break;
		case 7:
			cls();
			if (is_eliminate(arr_ball, row, col, arr_selected)){
				do {
					draw_img_div(arr_ball, row, col, 0, 1, arr_selected, COLOR_BLACK);
					draw_img_removeball(arr_ball, row, col, 4, 3, arr_selected);
					ball_remove_all(arr_ball, row, col, arr_selected);
					ball_fall_all(arr_ball, row, col, 1, 4, 3);
					ball_add(arr_ball, row, col, arr_selected);
					draw_ball_img_div_extra(arr_ball, row, col, 4, 3, arr_selected);
					arr_reset(arr_selected);
				} while (is_eliminate(arr_ball, row, col, arr_selected));

				if (find_notice(arr_ball, row, col, arr_selected))
					draw_ball_img_div_extra(arr_ball, row, col, 4, 3, arr_selected, "●");
				else
				{
					gotoxy(0, 2 * row + 3);
					to_be_continued("无可选择的消除提示，Game Over");
					return;
				}
			}
			else
			{
				draw_img_div(arr_ball, row, col, 0, 1);
				showstr(14, 0, "未找到可消除项");
			}
			gotoxy(0, 2 * row + 3);
			break;
		case 8:
			break;
		//case 9:
		//	ifstream fin;
		//	int np[MAX_ROW + 2][MAX_COL + 2] = { 0 };
		//	char s[12];
		//	cout << "输入学号：";
		//	cin >> s;
		//	strcat(s, ".dat");
		//	fin.open(s, ios::in);
		//	fin >> row >> col;
		//	for (int i = 1; i <= row; i++)
		//		for (int j = 1; j < col; j++)
		//		{
		//			fin >> arr_ball[i][j];
		//			if (arr_ball[i][j] >= 90)
		//			{
		//				arr_ball[i][j] -= 90;
		//				np[i][j] = 1;
		//			}
		//		}
		//	fin.close();
		//	cls();
		//	cout << "\n初始数组：";
		//	draw_text(arr_ball, row, col, 0, 4, 0);
		//	to_be_continued("进行寻找初始可消除项的操作");
		//	if (is_eliminate(arr_ball, row, col, arr_selected))
		//		print_result_text(arr_ball, arr_selected, row, col);
		//	else
		//		cout << "初始已无可消除项" << endl;

		//	cout << "\n验证数组：";
		//	print_result_text(arr_ball, np, row, col);
		//	break;
		default:
			break;
	}
	to_be_continued(NULL);
}