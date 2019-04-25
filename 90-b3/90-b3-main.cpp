/* 学号 姓名 班级 */
#include <iostream>
#include "../common/cmd_console_tools.h"
using namespace std;

int main()
{
	const int x = 123456789;

	//重复显示3次整数123456789
	showint(5, 10, x, COLOR_BLACK, COLOR_WHITE, 3);
	showstr(5, 11, "90-b3");

	//恢复初始颜色
	setcolor();
	cout << endl << endl;

	return 0;
}
