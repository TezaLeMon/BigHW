// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_tgmw_tools.h"
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

void ball_fall_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int is_draw = 0, const int x_position = 0, const int y_position = 0, const CONSOLE_GRAPHICS_INFO *const pCGI = NULL)
{
	int i, j;
	/* 定义1-9的数字用何种形式显示在界面上（正常状态） */
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
		{1, COLOR_HBLACK, -1, "〇"},
		{2, COLOR_YELLOW, -1, "〇"},
		{3, COLOR_HGREEN, -1, "〇"},
		{4, COLOR_HCYAN, -1, "〇"},
		{5, COLOR_HRED, -1, "〇"},
		{6, COLOR_HPINK, -1, "〇"},
		{7, COLOR_HYELLOW, -1, "〇"},
		{8, COLOR_CYAN, -1, "〇"},
		{9, COLOR_WHITE, -1, "〇"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};

	for (i = row; i > 1; i--)	//最上层上方肯定无球
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 0) {
				if (is_draw) {
					int t = i - 1, k = 0;
					while (arr[t][j] == 0)
						t--;
					if (arr[t][j] == -1)
						continue;
					while (t + k != i) {
						gmw_move_block(pCGI, t + k++ - 1, j - 1, arr[t][j], 0, bdi_normal, UP_TO_DOWN, 1);
						//disp_move_single(arr, t + k++, j, t, j, 2, x_position, y_position);
					}
				}
				ball_fall(arr, i, j);
			}
}

void ball_add(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int ball_add[MAX_ROW + 2][MAX_ROW + 2])
{
	if (ball_add)
		arr_reset(ball_add);
	int i, j;
	for (i = 1; i <= 5; i++)	//至多上五层无球
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 0) {
				arr[i][j] = 1 + rand() % 9;
				if (ball_add)
					ball_add[i][j] = 1;
			}

}

void ball_remove_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2])
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
	for (i = 1; i <= row; i++) {
		n_count = 1;
		from = 1;
		for (j = 2; j <= col; j++)
			if (arr[i][j] == arr[i][from])
				n_count++;
			else
			{
				if (n_count >= 3) {
					flag = 1;
					if (eliminate_ball)
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
				if (n_count >= 3) {
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
			if (is_eliminate(arr, row, col)) {
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
			if (is_eliminate(arr, row, col)) {
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
	//	PS 文字版仍使用老代码
	CONSOLE_GRAPHICS_INFO MagicBall_CGI; //声明一个CGI变量
	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&MagicBall_CGI);
	gmw_set_color(&MagicBall_CGI, COLOR_BLACK, COLOR_HWHITE);			//整个窗口颜色
	gmw_set_rowno_switch(&MagicBall_CGI, true);							//显示行号
	gmw_set_colno_switch(&MagicBall_CGI, false);						//显示列标
	gmw_set_frame_default_linetype(&MagicBall_CGI, 2);					//游戏主区域线型：单线
	gmw_set_frame_color(&MagicBall_CGI, COLOR_HWHITE, COLOR_BLACK);		//游戏主区域颜色
	gmw_set_delay(&MagicBall_CGI, DELAY_OF_BLOCK_MOVED, 50);	//延时

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
		{1, COLOR_HBLACK, -1, "〇"},
		{2, COLOR_YELLOW, -1, "〇"},
		{3, COLOR_HGREEN, -1, "〇"},
		{4, COLOR_HCYAN, -1, "〇"},
		{5, COLOR_HRED, -1, "〇"},
		{6, COLOR_HPINK, -1, "〇"},
		{7, COLOR_HYELLOW, -1, "〇"},
		{8, COLOR_CYAN, -1, "〇"},
		{9, COLOR_WHITE, -1, "〇"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	/* 定义1-9的数字用何种形式显示在界面上（选中状态） */
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
		{1, COLOR_HBLACK, -1, "●"},
		{2, COLOR_YELLOW, -1, "●"},
		{3, COLOR_HGREEN, -1, "●"},
		{4, COLOR_HCYAN, -1, "●"},
		{5, COLOR_HRED, -1, "●"},
		{6, COLOR_HPINK, -1, "●"},
		{7, COLOR_HYELLOW, -1, "●"},
		{8, COLOR_CYAN, -1, "●"},
		{9, COLOR_WHITE, -1, "●"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	/* 定义1-9的数字用何种形式显示在界面上（可消除提示状态） */
	const BLOCK_DISPLAY_INFO bdi_prompt[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
		{1, COLOR_HBLACK, -1, "◎"},
		{2, COLOR_YELLOW, -1, "◎"},
		{3, COLOR_HGREEN, -1, "◎"},
		{4, COLOR_HCYAN, -1, "◎"},
		{5, COLOR_HRED, -1, "◎"},
		{6, COLOR_HPINK, -1, "◎"},
		{7, COLOR_HYELLOW, -1, "◎"},
		{8, COLOR_CYAN, -1, "◎"},
		{9, COLOR_WHITE, -1, "◎"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	/* 定义1-9的数字用何种形式显示在界面上（爆炸/消除状态） */
	const BLOCK_DISPLAY_INFO bdi_exploded[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //空白
		{1, COLOR_HBLACK, -1, "¤"},
		{2, COLOR_YELLOW, -1, "¤"},
		{3, COLOR_HGREEN, -1, "¤"},
		{4, COLOR_HCYAN, -1, "¤"},
		{5, COLOR_HRED, -1, "¤"},
		{6, COLOR_HPINK, -1, "¤"},
		{7, COLOR_HYELLOW, -1, "¤"},
		{8, COLOR_CYAN, -1, "¤"},
		{9, COLOR_WHITE, -1, "¤"},
		{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};
	char temp[256];

	if (mode >= '0'&&mode <= '9')
		choice = mode - '0' + 1;
	else
		choice = mode - 'A' + 1;
	cls();
	input_int("请输入行数(5-9)： ", &row, 5, 9, 0, 0);
	input_int("请输入列数(5-9)： ", &col, 5, 9, 0, 1);

	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(&MagicBall_CGI, row, col);
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
			if (choice - 4) {
				gmw_set_frame_style(&MagicBall_CGI, 2, 1, true);	//游戏主区域风格：每个色块宽2高1，无分隔线
				//draw_img_div(arr_ball, row, col, 0, 1);
			}
			else {
				gmw_set_frame_style(&MagicBall_CGI, 2, 1, false);	//游戏主区域风格：每个色块宽2高1，有分隔线
				//draw_img_no_div(arr_ball, row, col, 0, 1);
			}
			/* 显示框架 */
			gmw_draw_frame(&MagicBall_CGI);
			/* 上状态栏显示内容 */
			sprintf(temp, "窗口大小：%d行 %d列", MagicBall_CGI.lines, MagicBall_CGI.cols);
			gmw_status_line(&MagicBall_CGI, TOP_STATUS_LINE, temp);

			/* 将内部数组展现到屏幕上 */
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++) {
					gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
				}

			break;
		case 6:
			gmw_set_frame_style(&MagicBall_CGI, 2, 1, true);	//游戏主区域风格：每个色块宽2高1，有分隔线
			/* 显示框架 */
			gmw_draw_frame(&MagicBall_CGI);
			/* 上状态栏显示内容 */
			sprintf(temp, "窗口大小：%d行 %d列", MagicBall_CGI.lines, MagicBall_CGI.cols);
			gmw_status_line(&MagicBall_CGI, TOP_STATUS_LINE, temp);

			/* 将内部数组展现到屏幕上 */
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++) {
					gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
				}

			if (is_eliminate(arr_ball, row, col, arr_selected)) {
				//draw_img_div(arr_ball, row, col, 0, 1, arr_selected, COLOR_BLACK);
				for (int i = 0; i < row; i++)
					for (int j = 0; j < col; j++) {
						if (arr_selected[i + 1][j + 1])
							gmw_draw_block(&MagicBall_CGI, i, j, arr_selected[i + 1][j + 1], bdi_prompt);
					}
			}
			else
			{
				//draw_img_div(arr_ball, row, col, 0, 1);
				//showstr(14, 0, "未找到可消除项");
				/* 下状态栏显示内容 */
				gmw_status_line(&MagicBall_CGI, LOWER_STATUS_LINE, "未找到可消除项", "测试结束，"); //只是给出提示而已，如果真的想输入End，后续还需要加输入及判断

			}
			gotoxy(0, 2 * row + 3);
			break;
		case 7:
			//cls();
			gmw_set_frame_style(&MagicBall_CGI, 2, 1, true);	//游戏主区域风格：每个色块宽2高1，有分隔线
			/* 显示框架 */
			gmw_draw_frame(&MagicBall_CGI);
			/* 上状态栏显示内容 */
			sprintf(temp, "窗口大小：%d行 %d列", MagicBall_CGI.lines, MagicBall_CGI.cols);
			gmw_status_line(&MagicBall_CGI, TOP_STATUS_LINE, temp);

			/* 将内部数组展现到屏幕上 */
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++) {
					gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
				}

			if (is_eliminate(arr_ball, row, col, arr_selected)) {
				do {
					//draw_img_div(arr_ball, row, col, 0, 1, arr_selected, COLOR_BLACK);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_selected);
						}
					Sleep(50);
					//draw_img_removeball(arr_ball, row, col, 4, 3, arr_selected);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_exploded);
						}
					Sleep(50);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, 0, bdi_normal);
						}
					ball_remove_all(arr_ball, row, col, arr_selected);
					ball_fall_all(arr_ball, row, col, 1, 4, 3, &MagicBall_CGI);
					ball_add(arr_ball, row, col, arr_selected);

					//draw_ball_img_div_extra(arr_ball, row, col, 4, 3, arr_selected);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
						}

					arr_reset(arr_selected);
				} while (is_eliminate(arr_ball, row, col, arr_selected));


				if (find_notice(arr_ball, row, col, arr_selected)) {
					//draw_ball_img_div_extra(arr_ball, row, col, 4, 3, arr_selected, "●");
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_prompt);
						}

				}
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