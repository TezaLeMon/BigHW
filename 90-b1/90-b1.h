// 1752762 ��1�� κ�ٴ�
#include "../common/common_settings.h"

/*����ѡ��˵�������Ϸ*/
void play(char mode);

/*��������*/
void input_int(const char *s, int *target, const int low, const int high, const int x, const int y);
void input_str2(const char *s, char *target, const int row_low, const int row_high, const int col_low, const int col_high);

/*���ɳ�ʼ����*/
void array_create(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);
/*�������������*/
void ball_remove_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2]);
//void ball_fall_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int is_draw = 0, const int x_positon = 0, const int y_position = 0, const CONSOLE_GRAPHICS_INFO *const pCGI = NULL);
void ball_add(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int ball_add[MAX_ROW + 2][MAX_ROW + 2] = NULL);

/*Ѱ��������ʾ ����������1 �Ѳ�����������0 �豣֤����ǰ���޿�������*/
int find_notice(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_ball[MAX_ROW + 2][MAX_COL + 2]);

/*�ı� ��ӡ�������򣨲�ɫ*/
void print_result_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int arr_need_color[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);
/*�ı� ��ӡ�ڲ����飨�ڰ�*/
void print_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);

/*ͼ�� �ٿ�*/
int manipulate_play();