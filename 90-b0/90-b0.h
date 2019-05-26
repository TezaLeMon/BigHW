// 1752762 ��1�� κ�ٴ�
#include "../common/common_settings.h"
#include "../common/cmd_gmw_tools.h"

//�ڲ���������
void array_create(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int n_balls);
//�����
void array_add(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int ball);
//�ƶ���
void array_move(int arr[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_to, const int col_to);
//Ѱ· ��·����1 ��·����0  ����path�о�����·��Ϊ1������Ϊ0
int path_find(const int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_to, const int col_to, int &flag_find);
//�ж���Ϸ�Ƿ����
int is_over(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);
//�ж��Ƿ������򲢽�����Ӧ���� ��������1 ���򷵻�0
int remove(int arr[MAX_ROW + 2][MAX_COL + 2], const int row_target, const int col_target, const int row, const int col, int *ball_count = NULL);



//�ı���ʽ�ƶ��ڲ����� δ�ƶ�����0 �ƶ���δ��������1 �ƶ�������������2
int array_print_move_text(int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int move_flag = 0);
//�ı���ӡ·�����ҽ��
void path_print_result_text(const int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);
void path_print_move_text(const int arr[MAX_ROW + 2][MAX_COL + 2], const int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col);

//ͼ���ƶ��ڲ����� �ƶ���δ��������1 �ƶ�������������2
int array_print_move_img(int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int *ball_count = NULL, const int can_keyboard = 0, CONSOLE_GRAPHICS_INFO *const pCGI = NULL, const BLOCK_DISPLAY_INFO *const bdi = NULL);



//��������
void input_int(const char *s, int *target, const int low, const int high, const int x, const int y);
void input_str2(const char *s, char *target, const int row_low, const int row_high, const int col_low, const int col_high);
//�����򲢴�ӡ��Ϣ
void ball_creat_print(int ball[3], const int again_flag);
void ball_creat_print_img(int ball[3], const int again_flag);
//ͼ�ν��� ��ӡ�÷�
void score_print(const int ball_conut[8]);
//ͼ�ν��� ��ӡ�������
void ball_count_print(const int arr[MAX_ROW + 2][MAX_COL + 2], const int removeball_conut[8], const int row, const int col);

void disp_move_img(const int arr[MAX_ROW + 2][MAX_COL + 2], int path[MAX_ROW + 2][MAX_COL + 2], const int row_from, const int col_from, const int row_to, const int col_to, const int row_ori, const int col_ori, const CONSOLE_GRAPHICS_INFO *const pCGI, const BLOCK_DISPLAY_INFO *const bdi);