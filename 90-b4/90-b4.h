// 1752762 ��1�� κ�ٴ�
#define MAX_ROW 8
#define MAX_COL 10
#include "../common/cmd_gmw_tools.h"

//��Ϸ����
//����0 �����˳�	����1���¿�ʼ	����-1 �쳣�˳�
int my_2048();

/*����һ�����ɫ�� 1/3����Ϊ4 2/3����Ϊ2 ��Ҫ���б�֤����δ��*/
int create_single(const int ROW, const int COL, int arr[MAX_ROW][MAX_COL], int* new_row = NULL, int* new_col = NULL);

/*�ƶ���ϲ�ɫ�� �ƶ�����1 δ�ƶ�����0*/
int arr_move_merge(const int ROW, const int COL, int arr[MAX_ROW][MAX_COL], CONSOLE_GRAPHICS_INFO *const pCGI, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction);