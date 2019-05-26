// 1752762 计1班 魏鹳达
#include "../common/common_settings.h"

/*根据选择菜单调动游戏*/
void play(char mode);

/*输入数据*/
void input_int(const char *s, int *target, const int low, const int high, const int x, const int y);
void input_str2(const char *s, char *target, const int row_low, const int row_high, const int col_low, const int col_high);

/*生成初始彩球*/
void array_create(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);
/*彩球消除与添加*/
void ball_remove_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2]);
//void ball_fall_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int is_draw = 0, const int x_positon = 0, const int y_position = 0, const CONSOLE_GRAPHICS_INFO *const pCGI = NULL);
void ball_add(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int ball_add[MAX_ROW + 2][MAX_ROW + 2] = NULL);

/*寻找消除提示 可消除返回1 已不可消除返回0 需保证进行前已无可消除球*/
int find_notice(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_ball[MAX_ROW + 2][MAX_COL + 2]);

/*文本 打印消除彩球（彩色*/
void print_result_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int arr_need_color[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);
/*文本 打印内部数组（黑白*/
void print_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);

/*图形 操控*/
int manipulate_play();