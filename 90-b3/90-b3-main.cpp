// 1752762 ¼Æ1°à ÎºðÙ´ï
#include <iostream>
#include "../common/cmd_console_tools.h"
#include "../common/cmd_hdc_tools.h"
#include "90-b3.h"
using namespace std;

int main()
{
	setconsoleborder(80, 25, 80, 1000);
	setfontsize("Terminal", 16, 8);
	char **list = NULL;
	read_book_list(&list, "./book");
	disp_booklist(&list, 3, 3, 10, 10);
	return 0;
}