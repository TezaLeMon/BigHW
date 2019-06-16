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
#include "90-b5-hit_plane.h"
using namespace std;

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

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：1 - 重玩	0 - 退出
  说    明：手动游戏模式
***************************************************************************/
int play_game_manual() {
	CONSOLE_GRAPHICS_INFO Ghitplane_CGI;	//声明一个CGI变量
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{ BDI_VALUE_BLANK, -1, -1, NULL },	//0不显示，用空格填充即可
	{ 1, COLOR_HYELLOW, COLOR_BLACK,"〇" },	//HitFail 所选坐标为空
	{ 2, COLOR_HRED, COLOR_BLACK, "×" },	//HitBody 所选坐标为机身
	{ 3, COLOR_GREEN, COLOR_BLACK, "×" },	//HitHead 所选坐标为机头
	{ BDI_VALUE_END, -1, -1, NULL } //判断结束条件为content为NULL，前面-999无所谓
	};

	char temp[256];

	/* 隐藏光标 */
	setcursor(CURSOR_INVISIBLE);

	/* 初始化GCGI结构体 */
	gmw_init(&Ghitplane_CGI);

	gmw_set_color(&Ghitplane_CGI, COLOR_BLACK, COLOR_WHITE);		//整个窗口颜色
	gmw_set_font(&Ghitplane_CGI, "新宋体", 16);						//字体
	gmw_set_frame_style(&Ghitplane_CGI, 6, 3);						//游戏主区域风格：每个色块宽10高5，分隔线【数字色块带边框，宽度为10(放最多6位数字)，高度为5(为了保持色块为方形)】
	gmw_set_frame_color(&Ghitplane_CGI, COLOR_WHITE, COLOR_BLACK);	//游戏主区域颜色
	gmw_set_frame_default_linetype(&Ghitplane_CGI, 2);				//游戏区域分隔线风格-全单线
	gmw_set_rowno_switch(&Ghitplane_CGI, false);					//不显示行号
	gmw_set_colno_switch(&Ghitplane_CGI, false);					//不显示列标

	/* 按row/col的值重设游戏主区域行列 */
	gmw_set_rowcol(&Ghitplane_CGI, 10, 10);		//写死了10*10

	/* 显示框架 */
	gmw_draw_frame(&Ghitplane_CGI);

	//	游戏部分
	int loop = 1;
	int maction, mrow, mcol;
	int keycode1, keycode2;
	int ret;
	int score = 0;

	while (1)
	{
		/* 上状态栏显示内容 */
		sprintf(temp, "当前分数：%d （R：重玩 Q：退出）", score);
		gmw_status_line(&Ghitplane_CGI, TOP_STATUS_LINE, temp);

		loop = 1;
		while (loop) {
			ret = gmw_read_keyboard_and_mouse(&Ghitplane_CGI, maction, mrow, mcol, keycode1, keycode2, false);

			if (ret == CCT_MOUSE_EVENT) {



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
					default:

						break;
				}//end of switch
			}//end of else
		}//end of while

	}
}

#if 1
int main()
{
	play_game_manual();
}
#endif