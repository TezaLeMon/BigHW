// 1752762 计1班 魏鹳达
// 格式转化仍有BUG	|自己态度问题 没有花够时间
#include <iostream>
#include <conio.h>
#include "../common/cmd_console_tools.h"
#include "../common/cmd_hdc_tools.h"
#include "90-b3.h"
using namespace std;

int main()
{
	while (1) {
		setcursor(CURSOR_INVISIBLE);
		setconsoleborder(80, 25, 80, 1000);
		setfontsize("Terminal", 16, 8);
		char **list = NULL;
		read_book_list(&list, "./book");
		if (list == NULL)		//error
			return -1;
		char **book;
		book = select_disp_booklist(&list, 10, 3, 10, 8);
		if (book == NULL)		//error
			return -1;
		char **pos_inf = NULL;
		int flag_is_new = 0;
		streamoff pos_beg;
		hdc_init(COLOR_BLACK, COLOR_WHITE, 1300, 1300);		//用(背景色，前景色，宽度，高度）初始化窗口
		hdc_triangle(0, 0, 0, 200, 200, 0, 1, 2, RGB(179, 146, 119));
		hdc_triangle(0, 1300, 0, 1100, 200, 1300, 1, 2, RGB(179, 146, 119));
		hdc_triangle(1300, 0, 1100, 0, 1300, 200, 1, 2, RGB(179, 146, 119));
		hdc_triangle(1300, 1300, 1300, 1100, 1100, 1300, 1, 2, RGB(179, 146, 119));

		pos_beg = read_pos(&pos_inf, *book, "./book");
		if (pos_beg == -1)		//error
			return -1;
		if (pos_beg == -2) {
			pos_beg = 0;
			flag_is_new = 1;
		}
		int is_close = disp_book_words(*book, "./book", 60, 62, 18, 8, pos_beg);
		save_pos(&pos_inf, *book, "./book", pos_beg, flag_is_new);
		if (is_close != 1)
			break;

		hdc_release();
	}

	return 0;
}