// 1752762 计1班 魏鹳达
#include <cstdlib>
#include "90-b4.h"
#include "../common/cmd_gmw_tools.h"

int create_single(const int ROW, const int COL, int arr[MAX_ROW][MAX_COL], int* new_row, int* new_col)
{
	int trow, tcol;
	//	2/3的概率生成2，1/3的概率生成4
	int t = rand() % 3;
	do
	{
		trow = rand() % ROW;
		tcol = rand() % COL;
	} while (arr[trow][tcol] != 0);	//假定传入数组不满 数组将满时会导致速度下降

	if (t)
		arr[trow][tcol] = 4;
	else
		arr[trow][tcol] = 2;

	if (new_row)
		*new_row = trow;
	if (new_col)
		*new_col = tcol;

	return 0;
}


int arr_move_merge(const int ROW, const int COL, int arr[MAX_ROW][MAX_COL], CONSOLE_GRAPHICS_INFO *const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction)
{
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
	int last_row = -1, last_col = -1;
	int count = 0, flag = 0, flag_move = 0;
	int i, j;
	switch (direction)
	{
		case DOWN_TO_UP:
			for (j = 0; j < COL; j++) {
				count = 0;
				flag = 0;
				for (i = 0; i < ROW; i++) {
					if (arr[i][j]) {
						flag = 1;
						if (last_row == -1 || last_col == -1) {
							last_row = i;
							last_col = j;
							continue;
						}

						if (arr[i][j] == arr[last_row][last_col]) {	//如果和之前一个有值色块相等
							if (i - count) {
								gmw_move_block(pCGI, i, j, arr[i][j], 0, bdi_normal, DOWN_TO_UP, i - count);
								flag_move = 1;
								//移动到重合位置后，用新数字的颜色重画
								gmw_draw_block(pCGI, count, j, 2 * arr[i][j], bdi_normal);

								arr[last_row][last_col] = 0;
								arr[count][j] = 2 * arr[i][j];	//移动之后修改值
								arr[i][j] = 0;
							}
							count++;	//计数器+1
							last_row = -1;
							last_col = -1;
						}
						else {	//如果不和上方一个有值色块相等
							if (last_row - count) {
								gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, DOWN_TO_UP, last_row - count);
								flag_move = 1;
								//移动前一个色块 后一个色块留待判断
								arr[count][j] = arr[last_row][last_col];
								arr[last_row][last_col] = 0;
							}
							last_row = i;
							last_col = j;
							count++;
						}
					}
				}

				if (flag&&last_row != -1 && last_col != -1) {	//有待判断的色块
					if (last_row - count) {
						gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, DOWN_TO_UP, last_row - count);
						flag_move = 1;
						arr[count][j] = arr[last_row][last_col];
						arr[last_row][last_col] = 0;
					}
					last_row = -1;
					last_col = -1;
				}
			}
			break;
		case UP_TO_DOWN:
			for (j = 0; j < COL; j++) {
				count = 0;
				flag = 0;
				for (i = ROW - 1; i >= 0; i--) {
					if (arr[i][j]) {
						flag = 1;
						if (last_row == -1 || last_col == -1) {
							last_row = i;
							last_col = j;
							continue;
						}

						if (arr[i][j] == arr[last_row][last_col]) {	//如果和之前一个有值色块相等
							if (ROW - i - count - 1) {
								gmw_move_block(pCGI, i, j, arr[i][j], 0, bdi_normal, UP_TO_DOWN, ROW - i - count - 1);
								flag_move = 1;
								//移动到重合位置后，用新数字的颜色重画
								gmw_draw_block(pCGI, ROW - count - 1, j, 2 * arr[i][j], bdi_normal);

								arr[last_row][last_col] = 0;
								arr[ROW - 1 - count][j] = 2 * arr[i][j];	//移动之后修改值
								arr[i][j] = 0;
							}
							count++;	//计数器+1
							last_row = -1;
							last_col = -1;
						}
						else {	//如果不和下方一个有值色块相等
							if (ROW - last_row - count - 1) {
								gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, UP_TO_DOWN, ROW - last_row - count - 1);
								flag_move = 1;
								//移动前一个色块 后一个色块留待判断
								arr[ROW - 1 - count][j] = arr[last_row][last_col];
								arr[last_row][last_col] = 0;
							}
							last_row = i;
							last_col = j;
							count++;
						}
					}
				}

				if (flag&&last_row != -1 && last_col != -1) {	//有待判断的色块
					if (ROW - last_row - count - 1) {
						gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, UP_TO_DOWN, ROW - last_row - count - 1);
						flag_move = 1;
						arr[ROW - 1 - count][j] = arr[last_row][last_col];
						arr[last_row][last_col] = 0;
					}
					last_row = -1;
					last_col = -1;
				}
			}
			break;
		case RIGHT_TO_LEFT:
			for (i = 0; i < ROW; i++) {
				count = 0;
				flag = 0;
				for (j = 0; j < COL; j++) {
					if (arr[i][j]) {
						flag = 1;
						if (last_row == -1 || last_col == -1) {
							last_row = i;
							last_col = j;
							continue;
						}

						if (arr[i][j] == arr[last_row][last_col]) {	//如果和之前一个有值色块相等
							if (j - count) {
								gmw_move_block(pCGI, i, j, arr[i][j], 0, bdi_normal, RIGHT_TO_LEFT, j - count);
								flag_move = 1;
								//移动到重合位置后，用新数字的颜色重画
								gmw_draw_block(pCGI, i, count, 2 * arr[i][j], bdi_normal);

								arr[last_row][last_col] = 0;
								arr[i][count] = 2 * arr[i][j];	//移动之后修改值
								arr[i][j] = 0;
							}
							count++;	//计数器+1
							last_row = -1;
							last_col = -1;
						}
						else {	//如果不和上方一个有值色块相等
							if (last_col - count) {
								gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, RIGHT_TO_LEFT, last_col - count);
								flag_move = 1;
								//移动前一个色块 后一个色块留待判断
								arr[i][count] = arr[last_row][last_col];
								arr[last_row][last_col] = 0;
							}
							last_row = i;
							last_col = j;
							count++;
						}
					}
				}

				if (flag&&last_row != -1 && last_col != -1) {	//有待判断的色块
					if (last_col - count) {
						gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, RIGHT_TO_LEFT, last_col - count);
						flag_move = 1;
						arr[i][count] = arr[last_row][last_col];
						arr[last_row][last_col] = 0;
					}
					last_row = -1;
					last_col = -1;
				}
			}
			break;
		case LEFT_TO_RIGHT:
			for (i = 0; i < ROW; i++) {
				count = 0;
				flag = 0;
				for (j = COL - 1; j >= 0; j--) {
					if (arr[i][j]) {
						flag = 1;
						if (last_row == -1 || last_col == -1) {
							last_row = i;
							last_col = j;
							continue;
						}

						if (arr[i][j] == arr[last_row][last_col]) {	//如果和之前一个有值色块相等
							if (COL - j - count - 1) {
								gmw_move_block(pCGI, i, j, arr[i][j], 0, bdi_normal, LEFT_TO_RIGHT, COL - j - count - 1);
								flag_move = 1;
								//移动到重合位置后，用新数字的颜色重画(0,1)
								gmw_draw_block(pCGI, i, COL - count - 1, 2 * arr[i][j], bdi_normal);

								arr[last_row][last_col] = 0;
								arr[i][COL - 1 - count] = 2 * arr[i][j];	//移动之后修改值
								arr[i][j] = 0;
							}
							count++;	//计数器+1
							last_row = -1;
							last_col = -1;
						}
						else {	//如果不和右方一个有值色块相等
							if (COL - last_col - count - 1) {
								gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, LEFT_TO_RIGHT, COL - last_col - count - 1);
								flag_move = 1;
								//移动前一个色块 后一个色块留待判断
								arr[i][COL - 1 - count] = arr[last_row][last_col];
								arr[last_row][last_col] = 0;
							}
							last_row = i;
							last_col = j;
							count++;
						}
					}
				}

				if (flag&&last_row != -1 && last_col != -1) {	//有待判断的色块
					if (COL - last_col - count - 1) {
						gmw_move_block(pCGI, last_row, last_col, arr[last_row][last_col], 0, bdi_normal, LEFT_TO_RIGHT, COL - last_col - count - 1);
						flag_move = 1;
						arr[i][COL - 1 - count] = arr[last_row][last_col];
						arr[last_row][last_col] = 0;
					}
					last_row = -1;
					last_col = -1;
				}
			}
			break;
	}
	return flag_move;
}