// 1752762 计1班 魏鹳达
#include "common_settings.h"

//文本 绘制边框及彩球
void draw_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int color = 1);

//图形 绘制边框及彩球（无分隔线）
void draw_img_no_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position);

//图形 绘制边框及彩球（有分隔线）
void draw_img_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_color[MAX_ROW + 2][MAX_COL + 2] = NULL, const int fg_color = COLOR_WHITE);
/*图形 重打印额外彩球*/
void draw_ball_img_div_extra(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_selected[MAX_ROW + 2][MAX_COL + 2], const char *s = "〇");

/*图形 移除彩球*/
void draw_img_removeball(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int eliminate[MAX_ROW + 2][MAX_COL + 2]);

/*图形 单步移动彩球 type: 1右 2下 3左 4上*/
void disp_move_single(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_ori, const int col_ori, const int type, const int x_position, const int y_position);
/*图形 移动单个彩球 type: 1右 2下 3左 4上*/
//void draw_img_moveball(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_to, const int col_to, const int x_position, const int y_position);
