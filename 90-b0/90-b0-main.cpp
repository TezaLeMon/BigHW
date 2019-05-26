// 1752762 计1班 魏鹳达
//	读键盘方式还存在BUG
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/common_data.h"
#include "90-b0.h"
using namespace std;
int menu()
{
	cout << "---------------------------------------------" << endl;
	cout << "1.内部数组，随机生成初始5个球" << endl;
	cout << "2.内部数组，随机生成60%的球，寻找移动路径" << endl;
	cout << "3.内部数组，完整版" << endl;
	cout << "4.画出n*n的框架（无分隔线），随机显示5个球" << endl;
	cout << "5.画出n*n的框架（有分隔线），随机显示5个球" << endl;
	cout << "6.n*n的框架，60%的球，支持鼠标，完成一次移动" << endl;
	cout << "7.cmd图形界面完整版" << endl;
	cout << "8.cmd图形界面完整版 - 支持同时读键（额外加分）" << endl;
	cout << "0.退出" << endl;
	cout << "---------------------------------------------" << endl;
	cout << "[请选择0 - 8] ";
	int ch;
	while (1)
	{
		ch = _getch();
		if (ch >= '0' && ch <= '8')
		{
			cout << ch - '0' << endl;
			to_be_continued(NULL);
			break;
		}
	}
	return ch;
}
int main()
{
	srand((unsigned)time(NULL));
	int arr[MAX_ROW + 2][MAX_COL + 2], row, col, ball[3];
	int path[MAX_ROW + 2][MAX_COL + 2] = { 0 };
	int ball_conut[8] = { 0 };
	setcursor(CURSOR_INVISIBLE);

	//	PS 文字版仍使用老代码
	CONSOLE_GRAPHICS_INFO ColorLinez_CGI; //声明一个CGI变量
	/*用缺省值初始化（窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略）*/
	gmw_init(&ColorLinez_CGI);
	gmw_set_rowno_switch(&ColorLinez_CGI, true);							//显示行号
	gmw_set_colno_switch(&ColorLinez_CGI, true);							//显示列标
	gmw_set_frame_default_linetype(&ColorLinez_CGI, 1);					//游戏主区域线型：双线
	gmw_set_delay(&ColorLinez_CGI, DELAY_OF_BLOCK_MOVED, 50);	//延时
	const BLOCK_DISPLAY_INFO bdi[] = {
	{BDI_VALUE_BLANK, -1, -1, "  "},  //0不显示，用空格填充即可
	{1,  COLOR_HBLACK, -1, "〇"},	//正常状态数字1所对应的彩球
	{2,  COLOR_YELLOW, -1, "〇"},
	{3,  COLOR_HGREEN, -1, "〇"},
	{4,  COLOR_HCYAN, -1, "〇"},
	{5,  COLOR_HRED, -1, "〇"},
	{6,  COLOR_HPINK, -1, "〇"},
	{7,  COLOR_HYELLOW, -1, "〇"},
	{11, COLOR_HBLACK, -1, "◎"},//选中状态数字1所对应的彩球图案
	{12, COLOR_YELLOW, -1, "◎"},
	{13, COLOR_HGREEN, -1, "◎"},
	{14, COLOR_HCYAN, -1, "◎"},
	{15, COLOR_HRED, -1, "◎"},
	{16, COLOR_HPINK, -1, "◎"},
	{17, COLOR_HYELLOW, -1, "◎"},
	{BDI_VALUE_END, -1, -1, NULL} //判断结束条件为-999
	};
	const int BDI_NORMAL = 0, BDI_SELECTED = 10;
	char temp[256];

	while (1)
	{
		setconsoleborder(80, 25, 80, 1000);
		setfontsize("Terminal", 16, 8);
		for (int i = 1; i < 8; i++)
			ball_conut[i] = 0;
		int choice = menu();
		if (choice == '0')
			return 0;
		cls();
		input_int("请输入行数(7-9)： ", &row, 7, 9, 0, 0);
		input_int("请输入列数(7-9)： ", &col, 7, 9, 0, 1);

		/* 按row/col的值重设游戏主区域行列 */
		gmw_set_rowcol(&ColorLinez_CGI, row, col);
		cls();
		switch (choice)
		{
		case '1':
			array_create(arr, row, col, 5);
			cout << "初始数组：";
			draw_text(arr, row, col, 0, 2);
			break;
		case '2':
			array_create(arr, row, col, int(row*col*0.6));
			cout << "当前数组：";
			draw_text(arr, row, col, 0, 2);
			ball_creat_print(ball, 0);
			if (array_print_move_text(arr, path, row, col))
			{
				path_print_result_text(path, row, col);
				path_print_move_text(arr, path, row, col);
			}
			break;
		case '3':
			array_create(arr, row, col, 5);
			while (!is_over(arr, row, col))
			{
				static int ball_flag = 0;
				cout << endl << "当前数组：";
				draw_text(arr, row, col, 0, 2);
				ball_creat_print(ball, ball_flag);
				int t = array_print_move_text(arr, path, row, col, 1);
				if (t == 1)
				{
					for (int i = 0; i < 3; i++)
					{
						array_add(arr, row, col, ball[i]);
						if (is_over(arr,row,col))
							i = 3;
					}
					ball_flag = 0;
				}
				else
					ball_flag = 1;
				if (t)
				{
					cls();
					cout << "移动后数组：";
					draw_text(arr, row, col, 0, 2);
				}
			}
			break;
		case '4':
			cout << "初始数组：";
			array_create(arr, row, col, 5);
			draw_text(arr, row, col, 0, 2);
			to_be_continued(NULL);
			//setconsoleborder(26 + col, 15, 26 + col, 15);
			//setfontsize("新宋体", 28);
			//cout << "屏幕：15行" << 26 + col << "列" << endl;
			//draw_img_no_div(arr, row, col, 0, 1);

			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, false);					//游戏主区域风格：每个色块宽2高1，无分隔线
			/* 显示框架 */
			gmw_draw_frame(&ColorLinez_CGI);

			/* 上状态栏显示内容 */
			sprintf(temp, "%s-窗口大小：%d行 %d列", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);

			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++) {
					gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
				}
			break;
		case '5':

			array_create(arr, row, col, 5);
			cout << "初始数组：";
			draw_text(arr, row, col, 0, 2);
			to_be_continued(NULL);
			//setconsoleborder(4 * (col + 2) + 3, 2 * row + 5, 4 * (col + 2) + 2, 2 * row + 5);
			//setfontsize("新宋体", 28);
			//cout << "屏幕：" << 2 * row + 5 << "行" << 4 * (col + 2) + 2 << "列" << endl;
			//draw_img_div(arr, row, col, 0, 1);

			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, true);					//游戏主区域风格：每个色块宽2高1，无分隔线
			/* 显示框架 */
			gmw_draw_frame(&ColorLinez_CGI);

			/* 上状态栏显示内容 */
			sprintf(temp, "%s-窗口大小：%d行 %d列", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);


			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++) {
					gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
				}

			break;
		case '6':
			array_create(arr, row, col, int(row*col*0.6));
			//setconsoleborder(4 * (col + 2) + 3, 2 * row + 5, 4 * (col + 2) + 2, 2 * row + 5);
			//setfontsize("新宋体", 28);
			//cout << "屏幕：" << 2 * row + 5 << "行" << 4 * (col + 2) + 2 << "列" << endl;
			//draw_img_div(arr, row, col, 0, 1);

			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, true);					//游戏主区域风格：每个色块宽2高1，无分隔线

			/* 显示框架 */
			gmw_draw_frame(&ColorLinez_CGI);
			/* 上状态栏显示内容 */
			sprintf(temp, "%s-窗口大小：%d行 %d列", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);

			for (int i = 1; i <= row; i++)
				for (int j = 1; j <= col; j++) {
					gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
				}

			array_print_move_img(arr, path, row, col, NULL, 0, &ColorLinez_CGI, bdi);
			break;
		case '7':
		case '8':
			array_create(arr, row, col, 5);
			//setconsoleborder(4 * 9 + 32, 2 * row + 5, 4 * 9 + 2, 2 * row + 5);
			//setfontsize("新宋体", 28);

			gmw_set_ext_rowcol(&ColorLinez_CGI, 0, 0, 0, 30);
			gmw_set_frame_style(&ColorLinez_CGI, 2, 1, true);					//游戏主区域风格：每个色块宽2高1，无分隔线

			/* 显示框架 */
			gmw_draw_frame(&ColorLinez_CGI);
			/* 上状态栏显示内容 */
			sprintf(temp, "%s-窗口大小：%d行 %d列", "ColorLinez", ColorLinez_CGI.lines, ColorLinez_CGI.cols);
			gmw_status_line(&ColorLinez_CGI, TOP_STATUS_LINE, temp);

			while (!is_over(arr, row, col))
			{
				static int ball_flag = 0;
				//cls();
				//cout << "屏幕：" << 2 * row + 5 << "行" << 4 * col + 2 << "列（右键退出）" << endl;
				//draw_img_div(arr, row, col, 0, 1);

				for (int i = 1; i <= row; i++)
					for (int j = 1; j <= col; j++) {
						gmw_draw_block(&ColorLinez_CGI, i - 1, j - 1, BDI_NORMAL + arr[i][j], bdi);
					}

				ball_creat_print_img(ball, ball_flag);
				score_print(ball_conut);
				ball_count_print(arr, ball_conut, row, col);
				int t = array_print_move_img(arr, path, row, col, ball_conut, choice - '7', &ColorLinez_CGI, bdi);
				if (t == 1){
					for (int i = 0; i < 3; i++){
						array_add(arr, row, col, ball[i]);
						if (is_over(arr, row, col))
							i = 3;
					}
					ball_flag = 0;
				}
				else if (t == -1)
					break;
				else
					ball_flag = 1;
			}
			break;
		default:
			cout << "error" << endl;
			break;
		}
		to_be_continued("本小题结束");
	}
	return 0;
}