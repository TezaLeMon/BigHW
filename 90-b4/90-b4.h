// 1752762 计1班 魏鹳达
#define MAX_ROW 8
#define MAX_COL 10
#include "../common/cmd_gmw_tools.h"

//游戏本体
//返回0 正常退出	返回1重新开始	返回-1 异常退出
int my_2048();

/*生成一个随机色块 1/3概率为4 2/3概率为2 需要自行保证数组未满*/
int create_single(const int ROW, const int COL, int arr[MAX_ROW][MAX_COL], int* new_row = NULL, int* new_col = NULL);

/*移动与合并色块 移动返回1 未移动返回0*/
int arr_move_merge(const int ROW, const int COL, int arr[MAX_ROW][MAX_COL], CONSOLE_GRAPHICS_INFO *const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction);