// 1752762 计1班 魏鹳达
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/common_data.h"
using namespace std;

int main()
{
	srand((unsigned)time(NULL));
	setcursor(CURSOR_INVISIBLE);
	char serial_number[] = { "ABCDEFGHIQ" };
	char prompt[][52] = {	"内部数组，生成初始状态，寻找是否有初始可消除项",
							"内部数组，消除初始可消除项后非0项下落并用0填充",
							"内部数组，消除初始可消除项后查找消除提示",
							"n*n的框架(无分隔线)，显示初始状态",
							"n*n的框架(有分隔线)，显示初始状态",
							"n*n的框架(有分隔线)，显示初始状态及初始可消除项",
							"n*n的框架(有分隔线)，消除初始可消除项后显示消除提示",
							"cmd图形界面完整版",
							"从文件中读取数据以验证查找消除提示的算法的正确性",
							"退出" };
	char t_menu;
	while (1){
		t_menu = menu(serial_number, prompt);
		if (t_menu == serial_number[strlen(serial_number) - 1])
			break;
		play(t_menu);
	}
	return 0;
}