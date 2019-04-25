// 1752762 计1班 魏鹳达
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
using namespace std;

void print_result_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int arr_need_color[MAX_ROW + 2][MAX_COL + 2], const int row, const int col)
{
	int i, j;
	cout << endl << "初始可消除项：" << endl << endl;
	cout << "  |";
	for (i = 1; i <= col; i++)
		cout << setw(3) << i;
	cout << endl << "--+-";
	for (i = 1; i <= col; i++)
		cout << "---";
	for (i = 1; i <= row; i++)
	{
		cout << endl;
		cout << char('A' + i - 1) << " |";
		for (j = 1; j <= col; j++)
		{
			setcolor();
			cout << "  ";
			if (arr_need_color[i][j])
				setcolor(COLOR_HYELLOW, COLOR_HBLUE);
			else
				setcolor();
			cout << arr[i][j];
		}
		setcolor();
	}
	cout << endl;
}

void print_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col)
{
	int i, j;
	setcolor();
	cout << "\n\n  |";
	for (i = 1; i <= col; i++)
		cout << setw(3) << i;
	cout << endl << "--+-";
	for (i = 1; i <= col; i++)
		cout << "---";
	for (i = 1; i <= row; i++)
	{
		cout << endl;
		cout << char('A' + i - 1) << " |";
		for (j = 1; j <= col; j++){
			cout << "  ";
			cout << arr[i][j];
		}
	}
	cout << endl;
}
