// 1752762 ��1�� κ�ٴ�
#include "common_settings.h"

//�ı� ���Ʊ߿򼰲���
void draw_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int color = 1);

//ͼ�� ���Ʊ߿򼰲����޷ָ��ߣ�
void draw_img_no_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position);

//ͼ�� ���Ʊ߿򼰲����зָ��ߣ�
void draw_img_div(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_color[MAX_ROW + 2][MAX_COL + 2] = NULL, const int fg_color = COLOR_WHITE);
/*ͼ�� �ش�ӡ�������*/
void draw_ball_img_div_extra(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int arr_selected[MAX_ROW + 2][MAX_COL + 2], const char *s = "��");

/*ͼ�� �Ƴ�����*/
void draw_img_removeball(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int x_position, const int y_position, const int eliminate[MAX_ROW + 2][MAX_COL + 2]);

/*ͼ�� �����ƶ����� type: 1�� 2�� 3�� 4��*/
void disp_move_single(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_ori, const int col_ori, const int type, const int x_position, const int y_position);
/*ͼ�� �ƶ��������� type: 1�� 2�� 3�� 4��*/
//void draw_img_moveball(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_to, const int col_to, const int x_position, const int y_position);
