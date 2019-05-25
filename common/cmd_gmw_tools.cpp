// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		此处可以给出需要的静态全局变量（尽可能少，最好没有）、静态全局只读变量/宏定义（个数不限）等
   -------------------------------------------------- */


   /* --------------------------------------------------
		   此处可以给出需要的内部辅助工具函数
		   1、函数名不限，建议为 gmw_inner_*
		   2、个数不限
		   3、必须是static函数，确保只在本源文件中使用
	  -------------------------------------------------- */


	  /* -----------------------------------------------
			  实现下面给出的函数（函数声明不准动）
		 ----------------------------------------------- */
		 /***************************************************************************
		   函数名称：
		   功    能：设置游戏主框架的行列数
		   输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
					 const int row						：行数(错误则为0，不设上限，人为保证正确性)
					 const int col						：列数(错误则为0，不设上限，人为保证正确性)
		   返 回 值：
		   说    明：1、指消除类游戏的矩形区域的行列值
					 2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
		 ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	//主框架部分
	int t = row <= 0 ? 10 : row;
	pCGI->row_num = t;	//主框架包含的色块的行数
	t = col <= 0 ? 10 : col;
	pCGI->col_num = t;	//主框架包含的色块的列数

	//整个cmd窗口大小
	//为了给中文输入法提示行及运行结束的提示信息留空间，要求在计算得到的结果基础上
	//（上额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4（1中文输入法提示行+3预留空行）
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_row_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no * 2 + 1;

	//下状态栏位置
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->draw_frame_with_row_no;
	//下状态栏宽度
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->draw_frame_with_col_no * 2;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置整个窗口（含游戏区、附加区在内的整个cmd窗口）的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int bg_color					：前景色（缺省COLOR_BLACK）
		   const int fg_color					：背景色（缺省COLOR_WHITE）
		   const bool cascade					：是否级联（缺省为true-级联）
  返 回 值：
  说    明：1、cascade = true时
				同步修改游戏主区域的颜色
				同步修改上下状态栏的正常文本的背景色和前景色，醒目文本的背景色（前景色不变）
			2、不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15
					前景色背景色的值一致导致无法看到内容
					前景色正好是状态栏醒目前景色，导致无法看到醒目提示
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	//主框架区域背景色
	//int t = (bgcolor < 0 || bgcolor>15) ? COLOR_BLACK : bgcolor;  //不检查？
	pCGI->area_bgcolor = bgcolor;
	//主框架区域前景色
	//t = (fgcolor < 0 || fgcolor>15) < 0 ? COLOR_WHITE : fgcolor;
	pCGI->area_fgcolor = fgcolor;

	if (cascade) {
		//主区域部分
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		//状态栏部分
		pCGI->SLI.top_normal_bgcolor = pCGI->area_bgcolor;	//正常文本颜色同窗口一致
		pCGI->SLI.top_normal_fgcolor = pCGI->area_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = pCGI->area_bgcolor;	//醒目文本背景颜色同窗口一致
		pCGI->SLI.lower_normal_bgcolor = pCGI->area_bgcolor;	//正常文本颜色同窗口一致
		pCGI->SLI.lower_normal_fgcolor = pCGI->area_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = pCGI->area_bgcolor;	//醒目文本背景颜色同窗口一致
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置窗口的字体
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *fontname					：字体名称（只能是"Terminal"和"新宋体"两种，错误则返回-1，不改变字体）
		   const int fs_high					：字体高度（缺省及错误为16，不设其它限制，人为保证）
		   const int fs_width					：字体高度（缺省及错误为8，不设其它限制，人为保证）
  返 回 值：
  说    明：1、与cmd_console_tools中的setfontsize相似，目前只支持“点阵字体”和“新宋体”
			2、若设置其它字体则直接返回，保持原字体设置不变
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	//字体部分
	strcpy(pCGI->CFT.font_type, fontname);	//默认点阵8*16
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置延时
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type						：延时的类型（目前为3种）
		   const int delay_ms					：以ms为单位的延时
			   画边框的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   画色块的延时：0 ~ 不设上限，人为保证正确（<0则置0）
			   色块移动的延时：BLOCK_MOVED_DELAY_MS ~ 不设上限，人为保证正确（ <BLOCK_MOVED_DELAY_MS 则置 BLOCK_MOVED_DELAY_MS）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	int dm = delay_ms < 0 ? 0 : delay_ms;

	if (type == DELAY_OF_BLOCK_MOVED)
		pCGI->delay_of_block_moved = dm;
	else if (type == DELAY_OF_DRAW_BLOCK)
		pCGI->delay_of_draw_block = dm;
	else if (type == DELAY_OF_DRAW_FRAME)
		pCGI->delay_of_draw_frame = dm;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  输入参数：设置游戏主框架结构之外需要保留的额外区域
  功    能：CONSOLE_GRAPHICS_INFO *const pCGI	：
		   const int up_lines					：上部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int down_lines				：下部额外的行（缺省及错误为0，不设上限，人为保证）
		   const int left_cols					：左边额外的列（缺省及错误为0，不设上限，人为保证）
		   const int right_cols				：右边额外的列（缺省及错误为0，不设上限，人为保证）
  返 回 值：
  说    明：额外行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	//额外行列更变
	pCGI->extern_up_lines = up_lines < 0 ? 0 : up_lines;
	pCGI->extern_down_lines = down_lines < 0 ? 0 : down_lines;
	pCGI->extern_left_cols = left_cols < 0 ? 0 : left_cols;
	pCGI->extern_right_cols = right_cols < 0 ? 0 : right_cols;

	//其他成员值更变
	//主框架区域参考坐标起始位置
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->start_y = pCGI->extern_up_lines;

	//状态栏部分
	pCGI->SLI.top_start_x = pCGI->start_x;
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->draw_frame_with_row_no;
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->draw_frame_with_col_no * 2;

	//整个cmd窗口大小
	//为了给中文输入法提示行及运行结束的提示信息留空间，要求在计算得到的结果基础上
	//（上额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4（1中文输入法提示行+3预留空行）
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + (2 + pCGI->CFI.separator*(pCGI->row_num - 1)) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_row_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no * 2 + 1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	//主框架区域组成元素的形状
	if (type == 1) {
		strcpy(pCGI->CFI.top_left, "╔");
		strcpy(pCGI->CFI.lower_left, "╚");
		strcpy(pCGI->CFI.top_right, "╗");
		strcpy(pCGI->CFI.lower_right, "╝");
		strcpy(pCGI->CFI.h_normal, "═");
		strcpy(pCGI->CFI.v_normal, "║");
		strcpy(pCGI->CFI.h_top_separator, "╦");
		strcpy(pCGI->CFI.h_lower_separator, "╩");
		strcpy(pCGI->CFI.v_left_separator, "╠");
		strcpy(pCGI->CFI.v_right_separator, "╣");
		strcpy(pCGI->CFI.mid_separator, "╬");
	}
	else if (type == 2) {
		strcpy(pCGI->CFI.top_left, "┏");
		strcpy(pCGI->CFI.lower_left, "┗");
		strcpy(pCGI->CFI.top_right, "┓");
		strcpy(pCGI->CFI.lower_right, "┛");
		strcpy(pCGI->CFI.h_normal, "━");
		strcpy(pCGI->CFI.v_normal, "┃");
		strcpy(pCGI->CFI.h_top_separator, "┳");
		strcpy(pCGI->CFI.h_lower_separator, "┻");
		strcpy(pCGI->CFI.v_left_separator, "┣");
		strcpy(pCGI->CFI.v_right_separator, "┫");
		strcpy(pCGI->CFI.mid_separator, "╋");
	}
	else if (type == 3) {
		strcpy(pCGI->CFI.top_left, "╒");
		strcpy(pCGI->CFI.lower_left, "╘");
		strcpy(pCGI->CFI.top_right, "╕");
		strcpy(pCGI->CFI.lower_right, "╛");
		strcpy(pCGI->CFI.h_normal, "═");
		strcpy(pCGI->CFI.v_normal, "┃");
		strcpy(pCGI->CFI.h_top_separator, "╤");
		strcpy(pCGI->CFI.h_lower_separator, "╧");
		strcpy(pCGI->CFI.v_left_separator, "╞");
		strcpy(pCGI->CFI.v_right_separator, "╡");
		strcpy(pCGI->CFI.mid_separator, "╪");
	}
	else if (type == 4)
	{
		strcpy(pCGI->CFI.top_left, "╓");
		strcpy(pCGI->CFI.lower_left, "╙");
		strcpy(pCGI->CFI.top_right, "╖");
		strcpy(pCGI->CFI.lower_right, "╜");
		strcpy(pCGI->CFI.h_normal, "━");
		strcpy(pCGI->CFI.v_normal, "║");
		strcpy(pCGI->CFI.h_top_separator, "╥");
		strcpy(pCGI->CFI.h_lower_separator, "╨");
		strcpy(pCGI->CFI.v_left_separator, "╟");
		strcpy(pCGI->CFI.v_right_separator, "╢");
		strcpy(pCGI->CFI.mid_separator, "╫");
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的11种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const char *...						：共11种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	char temp[5] = { '\0' };

	strncpy(temp, top_left ? top_left : "  ", 2);
	temp[top_left ? strlen(top_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.top_left, temp, 2);

	strncpy(temp, lower_left ? lower_left : "  ", 2);
	temp[lower_left ? strlen(lower_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.lower_left, temp, 2);

	strncpy(temp, top_right ? top_right : "  ", 2);
	temp[top_right ? strlen(top_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.top_right, temp, 2);

	strncpy(temp, lower_right ? lower_right : "  ", 2);
	temp[lower_right ? strlen(lower_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.lower_right, temp, 2);

	strncpy(temp, h_normal ? h_normal : "  ", 2);
	temp[h_normal ? strlen(h_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.h_normal, temp, 2);

	strncpy(temp, v_normal ? v_normal : "  ", 2);
	temp[v_normal ? strlen(v_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.v_normal, temp, 2);

	strncpy(temp, h_top_separator ? h_top_separator : "  ", 2);
	temp[h_top_separator ? strlen(h_top_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.h_top_separator, temp, 2);

	strncpy(temp, h_lower_separator ? h_lower_separator : "  ", 2);
	temp[h_lower_separator ? strlen(h_lower_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.h_lower_separator, temp, 2);

	strncpy(temp, v_left_separator ? v_left_separator : "  ", 2);
	temp[v_left_separator ? strlen(v_left_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.v_left_separator, temp, 2);

	strncpy(temp, v_right_separator ? v_right_separator : "  ", 2);
	temp[v_right_separator ? strlen(v_right_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.v_right_separator, temp, 2);

	strncpy(temp, mid_separator ? mid_separator : "  ", 2);
	temp[mid_separator ? strlen(mid_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.mid_separator, temp, 2);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_FRAME_TYPE 结构中的色块数量大小、是否需要分隔线等
  输入参数：输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int block_width						：宽度（错误及缺省2，因为约定表格线为中文制表符，如果给出奇数，要+1）
			const int block_high						：高度（错误及缺省1）
			const bool separator						：是否需要分隔线（缺省为true，需要分隔线）
  返 回 值：
  说    明：框架大小/是否需要分隔线等的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	if (block_width <= 0)
		pCGI->CFI.block_width = 2;
	else if (block_width % 2)
		pCGI->CFI.block_width = block_width + 1;
	else
		pCGI->CFI.block_width = block_width;

	if (block_high <= 0)
		pCGI->CFI.block_high = 1;
	else
		pCGI->CFI.block_high = block_high;

	pCGI->CFI.separator = separator;

	pCGI->CFI.block_width_ext = 2 * pCGI->CFI.separator;
	pCGI->CFI.block_high_ext = 1 * pCGI->CFI.separator;
	//主框架区域色块 每行/每列总宽度（含分隔线）
	pCGI->CFI.bwidth = pCGI->CFI.block_width + 2 * pCGI->CFI.separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.separator;

	//状态栏部分
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line;
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1));

	//整个cmd窗口大小
	//为了给中文输入法提示行及运行结束的提示信息留空间，要求在计算得到的结果基础上
	//（上额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4（1中文输入法提示行+3预留空行）
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_row_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no * 2 + 1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BORDER_TYPE 结构中的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int bg_color					：背景色（缺省 -1表示用窗口背景色）
			const int fg_color					：前景色（缺省 -1表示用窗口前景色）
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	pCGI->CFI.bgcolor = bgcolor == -1 ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = fgcolor == -1 ? pCGI->area_fgcolor : fgcolor;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型（缺省4种）
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：1 - 全双线 2 - 全单线 3 - 横双竖单 4 - 横单竖双
  返 回 值：
  说    明：
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	if (type == 1) {
		strcpy(pCGI->CBI.top_left, "╔");
		strcpy(pCGI->CBI.lower_left, "╚");
		strcpy(pCGI->CBI.top_right, "╗");
		strcpy(pCGI->CBI.lower_right, "╝");
		strcpy(pCGI->CBI.h_normal, "═");
		strcpy(pCGI->CBI.v_normal, "║");
	}
	else if (type == 2) {
		strcpy(pCGI->CBI.top_left, "┏");
		strcpy(pCGI->CBI.lower_left, "┗");
		strcpy(pCGI->CBI.top_right, "┓");
		strcpy(pCGI->CBI.lower_right, "┛");
		strcpy(pCGI->CBI.h_normal, "━");
		strcpy(pCGI->CBI.v_normal, "┃");
	}
	else if (type == 3) {
		strcpy(pCGI->CBI.top_left, "╒");
		strcpy(pCGI->CBI.lower_left, "╘");
		strcpy(pCGI->CBI.top_right, "╕");
		strcpy(pCGI->CBI.lower_right, "╛");
		strcpy(pCGI->CBI.h_normal, "═");
		strcpy(pCGI->CBI.v_normal, "┃");
	}
	else if (type == 4)
	{
		strcpy(pCGI->CBI.top_left, "╓");
		strcpy(pCGI->CBI.lower_left, "╙");
		strcpy(pCGI->CBI.top_right, "╖");
		strcpy(pCGI->CBI.lower_right, "╜");
		strcpy(pCGI->CBI.h_normal, "━");
		strcpy(pCGI->CBI.v_normal, "║");
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：填充 CONSOLE_BLOCK_INFO 结构中的6种线型
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const char *...					：共6种，具体见.h，此处略
  返 回 值：
  说    明：约定为一个中文制表符，可以使用其它内容，人为保证2字节
			1、超过2字节则只取前2字节
			2、如果给NULL，用两个空格替代
			3、如果给1字节，则补一个空格，如果因此而导致显示乱，不算错
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left,
	const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	char temp[5] = { '\0' };

	strncpy(temp, top_left ? top_left : "  ", 2);
	temp[top_left ? strlen(top_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.top_left, temp, 2);

	strncpy(temp, lower_left ? lower_left : "  ", 2);
	temp[lower_left ? strlen(lower_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.lower_left, temp, 2);

	strncpy(temp, top_right ? top_right : "  ", 2);
	temp[top_right ? strlen(top_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.top_right, temp, 2);

	strncpy(temp, lower_right ? lower_right : "  ", 2);
	temp[lower_right ? strlen(lower_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.lower_right, temp, 2);

	strncpy(temp, h_normal ? h_normal : "  ", 2);
	temp[h_normal ? strlen(h_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.h_normal, temp, 2);

	strncpy(temp, v_normal ? v_normal : "  ", 2);
	temp[v_normal ? strlen(v_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.v_normal, temp, 2);

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置每个游戏色块(彩球)是否需要小边框
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const bool on_off					：true - 需要 flase - 不需要（缺省false）
  返 回 值：
  说    明：边框约定为中文制表符，双线
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示上下状态栏
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const bool on_off					：true - 需要 flase - 不需要（缺省true）
  返 回 值：
  说    明：1、状态栏的相关约定如下：
			   1.1、上状态栏只能一行，在主区域最上方框线/列标的上面，为主区域的最开始一行（主区域的左上角坐标就是上状态栏的坐标）
			   1.2、下状态栏只能一行，在主区域最下方框线的下面
			   1.3、状态栏的宽度为主区域宽度，如果信息过长则截断
		   2、行列的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	if (type == TOP_STATUS_LINE) {
		pCGI->top_status_line = on_off;
		pCGI->SLI.is_top_status_line = on_off;
	}
	else if (type == LOWER_STATUS_LINE) {
		pCGI->lower_status_line = on_off;
		pCGI->SLI.is_lower_status_line = on_off;
	}
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no;
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置上下状态栏的颜色
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const int type						：状态栏类型（上/下）
			const int normal_bgcolor			：正常文本背景色（缺省 -1表示使用窗口背景色）
			const int normal_fgcolor			：正常文本前景色（缺省 -1表示使用窗口前景色）
			const int catchy_bgcolor			：醒目文本背景色（缺省 -1表示使用窗口背景色）
			const int catchy_fgcolor			：醒目文本前景色（缺省 -1表示使用亮黄色）
  输入参数：
  返 回 值：
  说    明：不检查颜色值错误及冲突，需要人为保证
				例：颜色非0-15，前景色背景色的值一致导致无法看到内容等
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	//状态栏部分
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;	//缺省 颜色同窗口一致
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;	//缺省 醒目文本背景颜色同窗口一致
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;	//缺省 醒目文本前景为亮黄
	}
	else if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;	//缺省 颜色同窗口一致
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;	//缺省 醒目文本背景颜色同窗口一致
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;	//缺省 醒目文本前景为亮黄
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示行号
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、行号约定为字母A开始连续排列（如果超过26，则从a开始，超过52的统一为*，实际应用不可能）
			2、是否显示行号的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_row_no = on_off;

	pCGI->SLI.width = 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1));

	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no * 2 + 1;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：设置是否显示列标
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
			const bool on_off					：true - 显示 flase - 不显示（缺省false）
  返 回 值：
  说    明：1、列标约定为数字0开始连续排列（数字0-99，超过99统一为**，实际应用不可能）
			2、是否显示列标的变化会导致CONSOLE_GRAPHICS_INFO结构体中其它成员值的变化，要处理
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_col_no = on_off;

	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line;

	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：打印 CONSOLE_GRAPHICS_INFO 结构体中的各成员值
  输入参数：
  返 回 值：
  说    明：1、仅供调试用，打印格式自定义
			2、本函数测试用例中未调用过，可以不实现
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：将 CONSOLE_GRAPHICS_INFO 结构体用缺省值进行初始化
  输入参数：CONSOLE_GRAPHICS_INFO *const pCGI：整体结构指针
		   const int row					：游戏区域色块行数（缺省10）
		   const int col					：游戏区域色块列数（缺省10）
		   const int bgcolor				：整个窗口背景色（缺省 COLOR_BLACK）
		   const int fgcolor				：整个窗口背景色（缺省 COLOR_WHITE）
  返 回 值：
  说    明：窗口背景黑/前景白，点阵16*8，上下左右无额外行列，上下状态栏均有，无行号/列标，框架线型为双线，色块宽度2/高度1/无小边框，颜色略
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	//主框架部分
	int t = row <= 0 ? 10 : row;
	pCGI->row_num = t;	//主框架包含的色块的行数
	t = col <= 0 ? 10 : col;
	pCGI->col_num = t;	//主框架包含的色块的列数

	//主框架区域参考坐标起始位置
	pCGI->start_x = 0;
	pCGI->start_y = 0;

	//主框架区域组成元素的形状
	strcpy(pCGI->CFI.top_left, "╔");
	strcpy(pCGI->CFI.lower_left, "╚");
	strcpy(pCGI->CFI.top_right, "╗");
	strcpy(pCGI->CFI.lower_right, "╝");
	strcpy(pCGI->CFI.h_normal, "═");
	strcpy(pCGI->CFI.v_normal, "║");
	strcpy(pCGI->CFI.h_top_separator, "╦");
	strcpy(pCGI->CFI.h_lower_separator, "╩");
	strcpy(pCGI->CFI.v_left_separator, "╠");
	strcpy(pCGI->CFI.v_right_separator, "╣");
	strcpy(pCGI->CFI.mid_separator, "╬");

	//主框架区域背景色
	t = col < 0 ? COLOR_BLACK : bgcolor;
	pCGI->CFI.bgcolor = t;
	pCGI->area_bgcolor = t;
	//主框架区域前景色
	t = col < 0 ? COLOR_WHITE : fgcolor;
	pCGI->CFI.fgcolor = t;
	pCGI->area_fgcolor = t;

	//主框架区域色块 宽度、高度
	pCGI->CFI.block_width = 2;
	pCGI->CFI.block_high = 1;
	//主框架区域色块 分割线（默认有）
	pCGI->CFI.separator = true;
	//每个色块附加的宽度和高度
	pCGI->CFI.block_width_ext = 2 * pCGI->CFI.separator;
	pCGI->CFI.block_high_ext = 1 * pCGI->CFI.separator;
	//主框架区域色块 每行/每列总宽度（含分隔线）
	pCGI->CFI.bwidth = pCGI->CFI.block_width + 2 * pCGI->CFI.separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.separator;

	//CFI填充部分置0
	memset(pCGI->CFI.pad1, '\0', sizeof(pCGI->CFI.pad1));
	memset(pCGI->CFI.pad2, '\0', sizeof(pCGI->CFI.pad2));
	memset(pCGI->CFI.pad, '\0', sizeof(pCGI->CFI.pad));

	//色块信息部分
	strcpy(pCGI->CBI.top_left, "╔");
	strcpy(pCGI->CBI.lower_left, "╚");
	strcpy(pCGI->CBI.top_right, "╗");
	strcpy(pCGI->CBI.lower_right, "╝");
	strcpy(pCGI->CBI.h_normal, "═");
	strcpy(pCGI->CBI.v_normal, "║");
	pCGI->CBI.block_border = false;		//默认色块无边框

	//CBI填充部分置0
	memset(&pCGI->CBI.pad1, '\0', sizeof(pCGI->CBI.pad1));
	memset(pCGI->CBI.pad, '\0', sizeof(pCGI->CBI.pad));

	//状态栏部分
	pCGI->top_status_line = true;	//默认开启上状态栏
	pCGI->SLI.is_top_status_line = true;
	pCGI->SLI.top_start_x = pCGI->start_x;	//位置（0，0）
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.top_normal_bgcolor = pCGI->area_bgcolor;	//正常文本颜色同窗口一致
	pCGI->SLI.top_normal_fgcolor = pCGI->area_fgcolor;
	pCGI->SLI.top_catchy_bgcolor = pCGI->area_bgcolor;	//醒目文本背景颜色同窗口一致
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;	//醒目文本前景为亮黄
	pCGI->lower_status_line = true;	//默认开启下状态栏
	pCGI->SLI.is_lower_status_line = true;
	pCGI->SLI.lower_start_x = pCGI->start_x;	//位置（0，...）
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	pCGI->SLI.lower_normal_bgcolor = pCGI->area_bgcolor;	//正常文本颜色同窗口一致
	pCGI->SLI.lower_normal_fgcolor = pCGI->area_fgcolor;
	pCGI->SLI.lower_catchy_bgcolor = pCGI->area_bgcolor;	//醒目文本背景颜色同窗口一致
	pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;	//醒目文本前景为亮黄
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1));

	//SLI填充部分置0
	memset(pCGI->SLI.pad1, '\0', sizeof(pCGI->SLI.pad1));
	memset(pCGI->SLI.pad, '\0', sizeof(pCGI->SLI.pad));

	//字体部分
	strcpy(pCGI->CFT.font_type, "Terminal");	//默认点阵8*16
	pCGI->CFT.font_size_high = 16;
	pCGI->CFT.font_size_width = 8;

	//默认不显示行号及列标
	pCGI->draw_frame_with_row_no = false;
	pCGI->draw_frame_with_col_no = false;

	pCGI->delay_of_draw_frame = 0;	//上下左右辅助区域全部为0
	pCGI->delay_of_draw_block = 0;	//画边框及色块时无延时
	pCGI->delay_of_block_moved = 3;	//色块移动时延时3ms

	//整个cmd窗口大小
	//为了给中文输入法提示行及运行结束的提示信息留空间，要求在计算得到的结果基础上
	//（上额外空间+上状态栏+列标显示+主区域+下状态栏）+ 4（1中文输入法提示行+3预留空行）
	pCGI->extern_up_lines = 0;
	pCGI->extern_down_lines = 0;
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;

	pCGI->extern_left_cols = 0;
	pCGI->extern_right_cols = 0;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no * 2 + 1;

	//主结构体填充置0
	memset(pCGI->pad, '\0', sizeof(pCGI->pad));

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：画主游戏框架
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
  返 回 值：
  说    明：具体可参考demo的效果
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	int i, j;
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cls();
	setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);

	if (pCGI->draw_frame_with_col_no) {
		setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
		for (j = 0; j < pCGI->col_num; j++) {
			//由于一些细节问题（以及懒...） 未使用showstr/showint/showch
			gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no + 2 + (pCGI->CFI.block_width / 2) / 2 * 2 + j * pCGI->CFI.bwidth, pCGI->start_y + pCGI->top_status_line);
			if (j <= 99)
				cout << j;
			else
				cout << "*";
		}
	}
	//第一行
	gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
	cout << pCGI->CFI.top_left;
	for (j = 1; j < pCGI->col_num; j++) {
		for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
			cout << pCGI->CFI.h_normal;
		if (pCGI->CFI.separator)
			cout << pCGI->CFI.h_top_separator;
	}
	for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
		cout << pCGI->CFI.h_normal;
	cout << pCGI->CFI.top_right;
	Sleep(pCGI->delay_of_draw_frame);
	//中间部分
	for (i = 1; i < pCGI->row_num; i++) {
		for (int h = 0; h < pCGI->CFI.block_high; h++) {
			if (pCGI->draw_frame_with_row_no&&h == (pCGI->CFI.block_high - 1) / 2) {
				char ch = i - 1 > 'Z' - 'A' ? '*' : 'A' + i - 1;
				showch(pCGI->start_x, h + pCGI->CFI.bhigh * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, ch, pCGI->area_bgcolor, pCGI->area_fgcolor);
				setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + pCGI->CFI.bhigh * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.v_normal;
			for (j = 1; j < pCGI->col_num; j++) {
				for (int t = 0; t < pCGI->CFI.block_width; t++)
					cout << " ";
				if (pCGI->CFI.separator)
					cout << pCGI->CFI.v_normal;
			}
			for (int t = 0; t < pCGI->CFI.block_width; t++)
				cout << " ";
			cout << pCGI->CFI.v_normal;
			Sleep(pCGI->delay_of_draw_frame);
		}
		if (pCGI->CFI.separator) {
			gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + (pCGI->CFI.bhigh) * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.v_left_separator;
			for (j = 1; j < pCGI->col_num; j++) {
				for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
					cout << pCGI->CFI.h_normal;
				cout << pCGI->CFI.mid_separator;
			}
			for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
				cout << pCGI->CFI.h_normal;
			cout << pCGI->CFI.v_right_separator;
		}
		Sleep(pCGI->delay_of_draw_frame);
	}
	//最后两行
	for (int h = 0; h < pCGI->CFI.block_high; h++) {
		if (pCGI->draw_frame_with_row_no&&h == (pCGI->CFI.block_high - 1) / 2) {
			char ch = i - 1 > 'Z' - 'A' ? '*' : 'A' + i - 1;
			showch(pCGI->start_x, pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + h + pCGI->CFI.bhigh * (pCGI->row_num - 1), ch, pCGI->area_bgcolor, pCGI->area_fgcolor);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
		}
		gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + (pCGI->CFI.bhigh) * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
		cout << pCGI->CFI.v_normal;
		for (j = 1; j < pCGI->col_num; j++) {
			for (int t = 0; t < pCGI->CFI.block_width; t++)
				cout << " ";
			if (pCGI->CFI.separator)
				cout << pCGI->CFI.v_normal;
		}
		for (int t = 0; t < pCGI->CFI.block_width; t++)
			cout << " ";
		cout << pCGI->CFI.v_normal;
		Sleep(pCGI->delay_of_draw_frame);
	}
	gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + (pCGI->CFI.bhigh) * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
	cout << pCGI->CFI.lower_left;
	for (j = 1; j < pCGI->col_num; j++) {
		for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
			cout << pCGI->CFI.h_normal;
		if (pCGI->CFI.separator)
			cout << pCGI->CFI.h_lower_separator;
	}
	for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
		cout << pCGI->CFI.h_normal;
	cout << pCGI->CFI.lower_right;
	Sleep(pCGI->delay_of_draw_frame);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：在状态栏上显示信息
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int type							：指定是上/下状态栏
		   const char *msg						：正常信息
		   const char *catchy_msg					：需要特别标注的信息（在正常信息前显示）
  返 回 值：
  说    明：1、输出宽度限定为主框架的宽度（含行号列标位置），超出则截去
			2、如果最后一个字符是某汉字的前半个，会导致后面乱码，要处理
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	if (type == TOP_STATUS_LINE && pCGI->top_status_line) {
		showstr(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y, " ", pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.width);
		gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
		if (catchy_msg) {
			setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
			cout << catchy_msg;
		}
		setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		cout << msg;
	}
	else if (type == LOWER_STATUS_LINE && pCGI->lower_status_line) {
		showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, " ", pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.width);
		gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		if (catchy_msg) {
			setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
			cout << catchy_msg;
		}
		setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		cout << msg;
	}
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：显示某一个色块(内容为字符串，坐标为row/col)
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const BLOCK_DISPLAY_INFO *const bdi		：存放该值对应的显示信息的结构体数组
  返 回 值：
  说    明：1、BLOCK_DISPLAY_INFO 的含义见头文件，用法参考测试样例
			2、bdi_value为 BDI_VALUE_BLANK 表示空白块，要特殊处理
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{

	const BLOCK_DISPLAY_INFO *p;
	int fg, bg;
	for (p = bdi; p->value != BDI_VALUE_END; p++) {
		if (p->value == bdi_value) {
			fg = p->fgcolor == -1 ? pCGI->CFI.fgcolor : p->fgcolor;
			bg = p->bgcolor == -1 ? pCGI->CFI.bgcolor : p->bgcolor;
			setcolor(bg, fg);
			break;
		}
	}

	if (pCGI->CBI.block_border) {
		//第一行
		if (bdi_value == BDI_VALUE_BLANK)
			showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, " ", bg, fg, pCGI->CFI.block_width);
		else {
			gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.top_left;
			for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
				cout << pCGI->CFI.h_normal;
			}
			cout << pCGI->CFI.top_right;
		}
		//中间行
		for (int h = 1; h < pCGI->CFI.block_high - 1; h++) {
			if (bdi_value == BDI_VALUE_BLANK)
				showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, " ", bg, fg, pCGI->CFI.block_width);
			else {
				gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				cout << pCGI->CFI.v_normal;
				for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
					cout << "  ";
				}
				cout << pCGI->CFI.v_normal;
			}
		}
		//最后一行
		if (bdi_value == BDI_VALUE_BLANK)
			showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, " ", bg, fg, pCGI->CFI.block_width);
		else {
			gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.lower_left;
			for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
				cout << pCGI->CFI.h_normal;
			}
			cout << pCGI->CFI.lower_right;
		}
	}

	//输出值
	gotoxy(2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
	if (p->content)
		cout << p->content;
	else if (p->value)
		cout << p->value;
	else
		cout << "  ";

	Sleep(pCGI->delay_of_draw_block);
	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：移动某一个色块
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   const int row_no						：行号（从0开始，人为保证正确性，程序不检查）
		   const int col_no						：列号（从0开始，人为保证正确性，程序不检查）
		   const int bdi_value						：需要显示的值
		   const int blank_bdi_value				：移动过程中用于动画效果显示时用于表示空白的值（一般为0，此处做为参数代入，是考虑到可能出现的特殊情况）
		   const BLOCK_DISPLAY_INFO *const bdi		：存放显示值/空白值对应的显示信息的结构体数组
		   const int direction						：移动方向，一共四种，具体见cmd_gmw_tools.h
		   const int distance						：移动距离（从1开始，人为保证正确性，程序不检查）
  返 回 值：
  说    明：
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	const BLOCK_DISPLAY_INFO *p;
	int fg, bg;
	for (p = bdi; p->value != BDI_VALUE_END; p++) {
		if (p->value == bdi_value) {
			fg = p->fgcolor == -1 ? pCGI->CFI.fgcolor : p->fgcolor;
			bg = p->bgcolor == -1 ? pCGI->CFI.bgcolor : p->bgcolor;
			break;
		}
	}
	if (p->value == BDI_VALUE_END)
		return -1;

	const BLOCK_DISPLAY_INFO *p_blank;
	int fg_blank, bg_blank;
	for (p_blank = bdi; p_blank->value != BDI_VALUE_END; p_blank++) {
		if (p_blank->value == blank_bdi_value) {
			fg_blank = p_blank->fgcolor == -1 ? pCGI->CFI.fgcolor : p_blank->fgcolor;
			bg_blank = p_blank->bgcolor == -1 ? pCGI->CFI.bgcolor : p_blank->bgcolor;
			break;
		}
	}
	if (p_blank->value == BDI_VALUE_END)
		return -1;

	for (int d = 0; d < distance; d++) {
		//每次移动一格
		if (direction == DOWN_TO_UP || direction == UP_TO_DOWN) {
			int t = direction * 2 - 1;	//朝上 t=-1		朝下 t=1
			for (int h = 1; h <= pCGI->CFI.bhigh; h++) {
				Sleep(pCGI->delay_of_block_moved);
				//抹去值
				gotoxy(2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + (h - 1)*t + pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				setcolor(bg_blank, fg_blank);
				if (p_blank->content)
					cout << p_blank->content;
				else if (p_blank->value)
					cout << p_blank->value;
				else
					cout << "  ";

				if (pCGI->CBI.block_border) {
					//先抹去最远一行
					setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + (h - 1)* t + direction + pCGI->CFI.bhigh* (1 - direction) + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					for (int w = 0; w < pCGI->CFI.block_width; w++)
						cout << " ";

					setcolor(bg, fg);
					//第一行
					gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h*t + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.top_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.top_right;
					//中间行
					for (int bh = 1; bh < pCGI->CFI.block_high - 1; bh++) {
						gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h*t + bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
						cout << pCGI->CFI.v_normal;
						for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
							cout << "  ";
						}
						cout << pCGI->CFI.v_normal;
					}
					//最后一行
					gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h*t + pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.lower_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.lower_right;
				}

				//输出值
				setcolor(bg, fg);
				gotoxy(2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h * t + pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				if (p->content)
					cout << p->content;
				else if (p->value)
					cout << p->value;
				else
					cout << "  ";
			}
			//补上被覆盖掉的边框
			if (pCGI->CFI.separator) {
				showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + direction * pCGI->CFI.bhigh + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.block_width / 2);
			}
		}
		else if (direction == LEFT_TO_RIGHT || direction == RIGHT_TO_LEFT) {
			int t = direction * 2 - 5;	//朝左 t=-1		朝右 t=1
			for (int h = 1; h <= pCGI->CFI.bwidth / 2; h++) {
				Sleep(pCGI->delay_of_block_moved);
				//抹去值
				gotoxy(t*d*pCGI->CFI.bwidth + (h - 1) * 2 * t + 2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				setcolor(bg_blank, fg_blank);
				if (p_blank->content)
					cout << p_blank->content;
				else if (p_blank->value)
					cout << p_blank->value;
				else
					cout << "  ";

				if (pCGI->CBI.block_border) {
					//先抹去最远一列(2列)
					for (int bh = 0; bh < pCGI->CFI.block_high; bh++) {
						showstr(t * d*pCGI->CFI.bwidth + pCGI->CFI.bwidth* (3 - direction) + 2 * (h - 1) * t + 2 * (direction - 2) + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no, bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, "  ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}

					setcolor(bg, fg);
					//第一行
					gotoxy(t*d*pCGI->CFI.bwidth + 2 * h * t + col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.top_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.top_right;
					//中间行
					for (int bh = 1; bh < pCGI->CFI.block_high - 1; bh++) {
						gotoxy(t*d*pCGI->CFI.bwidth + 2 * h * t + col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
						cout << pCGI->CFI.v_normal;
						for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
							cout << "  ";
						}
						cout << pCGI->CFI.v_normal;
					}
					//最后一行
					gotoxy(t*d*pCGI->CFI.bwidth + 2 * h * t + col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.lower_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.lower_right;
				}

				//输出值
				setcolor(bg, fg);
				gotoxy(t*d*pCGI->CFI.bwidth + h * 2 * t + 2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				if (p->content)
					cout << p->content;
				else if (p->value)
					cout << p->value;
				else
					cout << "  ";
			}
			//补上被覆盖掉的边框
			if (pCGI->CFI.separator) {
				setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				for (int bh = 0; bh < pCGI->CFI.block_high; bh++) {
					gotoxy(t*d*pCGI->CFI.bwidth + (direction - 2) * pCGI->CFI.bwidth + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.v_normal;
				}
			}
		}
	}

	return 0; //此句可根据需要修改
}

/***************************************************************************
  函数名称：
  功    能：读键盘或鼠标
  输入参数：const CONSOLE_GRAPHICS_INFO *const pCGI	：整体结构指针
		   int &MAction							：如果返回 CCT_MOUSE_EVENT，则此值有效，为 MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK 两者之一
													   如果返回 CCT_KEYBOARD_EVENT，则此值无效
		   int &MRow								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的行号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &MCol								：如果返回 CCT_MOUSE_EVENT 且 MAction = MOUSE_LEFT_BUTTON_CLICK，则此值有效，表示左键选择的游戏区域的列号（从0开始）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode1							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第一个）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   int &KeyCode2							：如果返回 CCT_KEYBOARD_EVENT，则此值有效，为读到的键码（如果双键码，则为第二个，如果是单键码，则为0）
												  其余情况此值无效（如果访问无效值导致错误，不是本函数的错!!!）
		   const bool update_lower_status_line		：鼠标移动时，是否要在本函数中显示"[当前光标] *行*列"的信息（true=显示，false=不显示，缺省为true）
  返 回 值：函数返回约定
			1、如果是鼠标移动，不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] *行*列"
		   2、如果是按下鼠标左键，且当前鼠标指针停留在主游戏区域的*行*列上，则返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow 为行号，MCol 为列标
								  且当前鼠标指针停留在非法区域（非游戏区域，游戏区域中的分隔线），则不返回，根据 update_lower_status_line 的设置在下状态栏显示"[当前光标] 位置非法"
		   3、如果是按下鼠标右键，则不判断鼠标指针停留区域是否合法，直接返回 CCT_MOUSE_EVENT ，MAction 为 MOUSE_LEFT_BUTTON_CLICK, MRow、MCol不可信
		   4、如果按下键盘上的某键（含双键码按键），则直接返回 CCT_KEYBOARD_EVENT，KeyCode1/KeyCode2中为对应的键码值
 说    明：通过调用 cmd_console_tools.cpp 中的 read_keyboard_and_mouse 函数实现
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	int X = 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no, Y = 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no;	//给定初始值 对应0行0列
	int ret;
	char temp[256];
	enable_mouse();
	setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	while (1) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = read_keyboard_and_mouse(X, Y, MAction, KeyCode1, KeyCode2);
		if (ret == CCT_MOUSE_EVENT) {
			MRow = (Y - (1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no)) / pCGI->CFI.bhigh;
			MCol = (X - (2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no)) / pCGI->CFI.bwidth;
			if (X - (2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no) < 0 || Y - (1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no) < 0
				|| (Y - (1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no)) % pCGI->CFI.bhigh >= pCGI->CFI.block_high
				|| (X - (2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no)) % pCGI->CFI.bwidth >= pCGI->CFI.block_width	//在边框上
				|| MRow < 0 || MRow >= pCGI->row_num || MCol < 0 || MCol >= pCGI->col_num) {	//超出范围
				sprintf(temp, "[当前光标] 位置非法");
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				continue;
			}
			else {
				sprintf(temp, "[当前光标] %c行 %d列 ", 'A' + MRow, MCol);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				switch (MAction) {
					case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
						return CCT_MOUSE_EVENT;
					case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
						return CCT_MOUSE_EVENT;
					default:
						break;
				}
			}
		}
		else if (ret == CCT_KEYBOARD_EVENT) {
			return CCT_KEYBOARD_EVENT;
		}
	}
	disable_mouse();	//禁用鼠标

	return 0; //此句可根据需要修改
}
