// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_console_tools.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_tgmw_tools.h"
#include "90-b4.h"
using namespace std;

#define N_EVERY 2	//每次生成色块的个数

/***************************************************************************
  函数名称：
  功    能：显示按键继续的信息
  输入参数：
  返 回 值：
  说    明：借用 CONSOLE_GRAPHICS_INFO 结构，定位在下状态栏(x,y)的(0,y+2)位置
***************************************************************************/
static void to_be_continued(const char *prompt, const CONSOLE_GRAPHICS_INFO *const bgi)
{
	setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //恢复初始颜色

	gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
	cout << setw(bgi->cols - 1) << ' '; //先用空格清空整行

	gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
	if (prompt)
		cout << prompt << "，按回车键继续...";
	else
		cout << "按回车键继续...";

	while (_getch() != '\r')
		;

	return;
}

/***************************************************************************
  函数名称：
  功    能：显示按键继续的信息
  输入参数：
  返 回 值：
  说    明：借用 CONSOLE_GRAPHICS_INFO 结构，定位在下状态栏(x,y)的(0,y+2)位置
***************************************************************************/
static void to_be_continued(const char *game_name, const char *prompt, const CONSOLE_GRAPHICS_INFO *const bgi)
{
	setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //恢复初始颜色

	gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
	cout << setw(bgi->cols - 1) << ' '; //先用空格清空整行

	gotoxy(0, bgi->SLI.lower_start_y + 2);//光标设到指定位置
	cout << game_name << '-' << (bgi->CFI.separator ? "有" : "无") << "分隔线-" << prompt << "，按回车键继续...";

	while (_getch() != '\r')
		;

	return;
}

//返回-1 游戏失败	返回1 游戏成功	返回0 游戏继续
int is_over(const int score, const int ROW, const int COL, const int arr[MAX_ROW][MAX_COL]) {
	//赢得游戏
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			if (arr[i][j] == score)
				return 1;

	//有空白
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			if (!arr[i][j])
				return 0;

	//横向
	for (int i = 0; i < ROW; ++i)
		for (int j = 0; j < COL - 1; ++j)
			if (arr[i][j] && arr[i][j] == arr[i][j + 1])
				return 0;

	//纵向
	for (int j = 0; j < COL; ++j)
		for (int i = 0; i < ROW - 1; ++i)
			if (arr[i][j] && arr[i][j] == arr[i + 1][j])
				return 0;

	//已经无路可走
	return -1;
}

//返回0 正常退出	返回1重新开始	返回-1 异常退出
int my_2048() {
	srand((unsigned int)time(NULL));
	CONSOLE_GRAPHICS_INFO G2048_CGI; //声明一个CGI变量
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  //0不显示，用空格填充即可
	{2, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{4, COLOR_HRED, COLOR_BLACK, NULL},
	{8, COLOR_GREEN, COLOR_BLACK, NULL},
	{16, COLOR_RED, COLOR_BLACK, NULL},
	{32, COLOR_HGREEN, COLOR_BLACK, NULL},
	{64, COLOR_HYELLOW, COLOR_BLACK, NULL},
	{128, COLOR_HRED, COLOR_BLACK, NULL},
	{256, COLOR_HWHITE, COLOR_BLACK, NULL},
	{512, COLOR_HBLACK, COLOR_BLACK, NULL},
	{1024, COLOR_HPINK, COLOR_BLACK, NULL},
	{2048, COLOR_WHITE, COLOR_BLACK, NULL},
	{4096, COLOR_YELLOW, COLOR_BLACK, NULL},
	{8192, COLOR_PINK, COLOR_BLACK, NULL},
	{16384, COLOR_HBLUE, COLOR_BLACK, NULL},
	{32768, COLOR_HCYAN, COLOR_BLACK, NULL},
	{65536, COLOR_HGREEN, COLOR_BLACK, NULL},
	{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //如果开心，还可以继续加
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为content为NULL，前面-999无所谓
	};

	setcursor(CURSOR_INVISIBLE);
	gmw_init(&G2048_CGI);
	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);		//整个窗口颜色
	gmw_set_font(&G2048_CGI, "新宋体", 16);						//字体
	gmw_set_frame_style(&G2048_CGI, 10, 5, false);				//游戏主区域风格：每个色块宽10高5，无分隔线【数字色块带边框，宽度为10(放最多6位数字)，高度为5(为了保持色块为方形)】
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_block_border_switch(&G2048_CGI, true);				//小色块带边框
	gmw_set_rowno_switch(&G2048_CGI, false);					//不显示行号
	gmw_set_colno_switch(&G2048_CGI, false);					//不显示列标

	while (1) {
		int arr2048[MAX_ROW][MAX_COL] = { 0 };
		int row = 8, col = 8, target_score = 8192;
		char temp[256];
		setcolor(G2048_CGI.area_bgcolor, G2048_CGI.area_fgcolor);
		setconsoleborder(80, 25, 80, 1000);
		//没有考虑显示问题
		cout << "游戏2048参数设置：" << endl;
		while (1) {
			cout << "请输入行数[4-8]" << endl;
			cin >> row;
			cin.clear();
			cin.ignore(1024, '\n');
			if (row >= 4 && row <= 8)
				break;
			cout << "输入错误 请重新输入" << endl;
		}
		while (1) {
			cout << "请输入列数[4-10]" << endl;
			cin >> col;
			cin.clear();
			cin.ignore(1024, '\n');
			if (col >= 4 && col <= 10)
				break;
			cout << "输入错误 请重新输入" << endl;
		}
		while (1) {
			cout << "请输入目标分数[8192/16384/32768/65536]" << endl;
			cin >> target_score;
			cin.clear();
			cin.ignore(1024, '\n');
			if (target_score == 8192 || target_score == 16384 || target_score == 32768 || target_score == 65536)
				break;
			cout << "输入错误 请重新输入" << endl;
		}

		/* 按row/col的值重设游戏主区域行列 */
		gmw_set_rowcol(&G2048_CGI, row, col);

		/* 显示框架 */
		gmw_draw_frame(&G2048_CGI);


		//初始 N_EVERY个色块
		for (int n_eve = 0; n_eve < N_EVERY; n_eve++)
			create_single(row, col, arr2048);

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++) {
				gmw_draw_block(&G2048_CGI, i, j, arr2048[i][j], bdi_normal);
			}

		int loop = 1, is_move = 0;
		int maction, mrow, mcol;
		int keycode1, keycode2;
		int ret;
		while (1) {	

			/* 上状态栏显示内容 */
			sprintf(temp, "目标：%d （R：重玩 Q：退出）", target_score);
			gmw_status_line(&G2048_CGI, TOP_STATUS_LINE, temp);

			loop = 1;
			while (loop) {
				ret = gmw_read_keyboard_and_mouse(&G2048_CGI, maction, mrow, mcol, keycode1, keycode2, false);

				if (ret == CCT_MOUSE_EVENT) {
					continue;
				}
				else { //CCT_KEYBOARD_EVENT - 具体键码含义去读 cmd_console_tools.cpp
					switch (keycode1) {
						case 'R':
						case 'r':
							return 1;	//重玩
							break;
						case 'q':
						case 'Q':
							return 0;
							break;
						case 0xE0: //224
							switch (keycode2) {
								case KB_ARROW_UP:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, DOWN_TO_UP);
									break;
								case KB_ARROW_DOWN:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, UP_TO_DOWN);
									break;
								case KB_ARROW_LEFT:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, RIGHT_TO_LEFT);
									break;
								case KB_ARROW_RIGHT:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, LEFT_TO_RIGHT);
									break;
							}
							break;
						default:
							break;
					}//end of switch

				}//end of else

			}//end of while

			int row_new, col_new;
			if (is_move) {
				if (is_over(target_score, row, col, arr2048) != 0)	//游戏结束（失败/成功）
					return 0;
				//游戏还未结束
				//每次生成两个方块
				for (int n_eve = 0; n_eve < N_EVERY; n_eve++) {
					create_single(row, col, arr2048, &row_new, &col_new);
					gmw_draw_block(&G2048_CGI, row_new, col_new, arr2048[row_new][col_new], bdi_normal);
				}
			}

		}
	}
	return 0;
}