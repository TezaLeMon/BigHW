// 1752762 ��1�� κ�ٴ�
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "90-b1.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_tgmw_tools.h"
using namespace std;


void arr_reset(int arr[MAX_ROW + 2][MAX_COL + 2])
{
	for (int i = 0; i < MAX_ROW + 2; i++)
		for (int j = 0; j < MAX_COL + 2; j++)
			arr[i][j] = 0;
}

void array_create(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col)
{
	int i, j;
	//��ʼ�� �߽�Ϊ-1 �ڲ�ȫΪ0
	for (i = 0; i <= col + 1; i++)
	{
		arr[0][i] = -1;
		arr[row + 1][i] = -1;
	}
	for (i = 1; i <= row; i++)
	{
		arr[i][0] = -1;
		arr[i][col + 1] = -1;
		for (j = 1; j <= col; j++)
			arr[i][j] = 1 + rand() % 7;
	}
}

void ball_remove(int arr[MAX_ROW + 2][MAX_COL + 2], const int row_t, const int col_t, int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2])
{
	eliminate_BALL[row_t][col_t] = 0;

	if (eliminate_BALL[row_t + 1][col_t] && arr[row_t + 1][col_t] == arr[row_t][col_t])	//��
		ball_remove(arr, row_t + 1, col_t, eliminate_BALL);
	if (eliminate_BALL[row_t][col_t + 1] && arr[row_t][col_t + 1] == arr[row_t][col_t])	//��
		ball_remove(arr, row_t, col_t + 1, eliminate_BALL);
	if (eliminate_BALL[row_t - 1][col_t] && arr[row_t - 1][col_t] == arr[row_t][col_t])	//��
		ball_remove(arr, row_t - 1, col_t, eliminate_BALL);
	if (eliminate_BALL[row_t][col_t - 1] && arr[row_t][col_t - 1] == arr[row_t][col_t])	//��
		ball_remove(arr, row_t, col_t - 1, eliminate_BALL);

	arr[row_t][col_t] = 0;
}

void ball_fall(int arr[MAX_ROW + 2][MAX_COL + 2], const int row_foot, const int col_foot)
{
	int t = 1;
	while (arr[row_foot - t][col_foot] == 0)
		t++;
	if (arr[row_foot - t][col_foot] == -1)	//�Ϸ�����ѭ�����߽�
		return;
	arr[row_foot][col_foot] = arr[row_foot - t][col_foot];
	arr[row_foot - t][col_foot] = 0;
}

void ball_fall_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, const int is_draw = 0, const int x_position = 0, const int y_position = 0, const CONSOLE_GRAPHICS_INFO *const pCGI = NULL)
{
	int i, j;
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�����״̬�� */
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
		{1, COLOR_HBLACK, -1, "��"},
		{2, COLOR_YELLOW, -1, "��"},
		{3, COLOR_HGREEN, -1, "��"},
		{4, COLOR_HCYAN, -1, "��"},
		{5, COLOR_HRED, -1, "��"},
		{6, COLOR_HPINK, -1, "��"},
		{7, COLOR_HYELLOW, -1, "��"},
		{8, COLOR_CYAN, -1, "��"},
		{9, COLOR_WHITE, -1, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};

	for (i = row; i > 1; i--)	//���ϲ��Ϸ��϶�����
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 0) {
				if (is_draw) {
					int t = i - 1, k = 0;
					while (arr[t][j] == 0)
						t--;
					if (arr[t][j] == -1)
						continue;
					while (t + k != i) {
						gmw_move_block(pCGI, t + k++ - 1, j - 1, arr[t][j], 0, bdi_normal, UP_TO_DOWN, 1);
						//disp_move_single(arr, t + k++, j, t, j, 2, x_position, y_position);
					}
				}
				ball_fall(arr, i, j);
			}
}

void ball_add(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int ball_add[MAX_ROW + 2][MAX_ROW + 2])
{
	if (ball_add)
		arr_reset(ball_add);
	int i, j;
	for (i = 1; i <= 5; i++)	//�������������
		for (j = 1; j <= col; j++)
			if (arr[i][j] == 0) {
				arr[i][j] = 1 + rand() % 9;
				if (ball_add)
					ball_add[i][j] = 1;
			}

}

void ball_remove_all(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_BALL[MAX_ROW + 2][MAX_COL + 2])
{
	int i, j;
	for (i = 1; i <= row; i++)
		for (j = 1; j <= col; j++)
			if (eliminate_BALL[i][j])
				ball_remove(arr, i, j, eliminate_BALL);
}

//����Ƿ�������� ��/����������1/0 ����������λ�ñ�Ǵ����eliminate_ball������
int is_eliminate(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_ball[MAX_ROW + 2][MAX_COL + 2] = NULL)
{
	int i, j, from, n_count, flag = 0;
	//����
	for (i = 1; i <= row; i++) {
		n_count = 1;
		from = 1;
		for (j = 2; j <= col; j++)
			if (arr[i][j] == arr[i][from])
				n_count++;
			else
			{
				if (n_count >= 3) {
					flag = 1;
					if (eliminate_ball)
						for (int t = from; t < j; t++)
							eliminate_ball[i][t] = 1;	//�����������λ�������
				}
				n_count = 1;
				from = j;
			}
	}

	//����
	for (i = 1; i <= col; i++) {
		n_count = 1;
		from = 1;
		for (j = 2; j <= row; j++)
			if (arr[j][i] == arr[from][i])
				n_count++;
			else
			{
				if (n_count >= 3) {
					flag = 1;
					if (eliminate_ball)
						for (int t = from; t < j; t++)
							eliminate_ball[t][i] = 1;	//���������������������������ж�
				}
				n_count = 1;
				from = j;
			}
	}
	return flag;
}

void ball_exchange(int arr[MAX_ROW + 2][MAX_COL + 2], const int row1, const int col1, const int row2, const int col2)
{
	int t = arr[row1][col1];
	arr[row1][col1] = arr[row2][col2];
	arr[row2][col2] = t;
}


int find_notice(int arr[MAX_ROW + 2][MAX_COL + 2], const int row, const int col, int eliminate_notice[MAX_ROW + 2][MAX_COL + 2])
{
	int i, j, flag = 0;
	//���Һ����ƶ����Ƿ������
	for (i = 1; i <= row; i++)
		for (j = 1; j < col; j++)
		{
			ball_exchange(arr, i, j, i, j + 1);
			if (is_eliminate(arr, row, col)) {
				flag = 1;
				eliminate_notice[i][j] = 1;
				eliminate_notice[i][j + 1] = 1;
			}
			ball_exchange(arr, i, j, i, j + 1);
		}

	//���������ƶ����Ƿ������
	for (i = 1; i < row; i++)
		for (j = 1; j <= col; j++)
		{
			ball_exchange(arr, i, j, i + 1, j);
			if (is_eliminate(arr, row, col)) {
				flag = 1;
				eliminate_notice[i][j] = 1;
				eliminate_notice[i + 1][j] = 1;
			}
			ball_exchange(arr, i, j, i + 1, j);
		}
	return flag;
}

void play(char mode)
{
	int choice, row, col, arr_ball[MAX_ROW + 2][MAX_COL + 2], arr_selected[MAX_ROW + 2][MAX_COL + 2];
	//	PS ���ְ���ʹ���ϴ���
	CONSOLE_GRAPHICS_INFO MagicBall_CGI; //����һ��CGI����
	/*��ȱʡֵ��ʼ�������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ�ԣ�*/
	gmw_init(&MagicBall_CGI);
	gmw_set_color(&MagicBall_CGI, COLOR_BLACK, COLOR_HWHITE);			//����������ɫ
	gmw_set_rowno_switch(&MagicBall_CGI, true);							//��ʾ�к�
	gmw_set_colno_switch(&MagicBall_CGI, false);						//��ʾ�б�
	gmw_set_frame_default_linetype(&MagicBall_CGI, 2);					//��Ϸ���������ͣ�����
	gmw_set_frame_color(&MagicBall_CGI, COLOR_HWHITE, COLOR_BLACK);		//��Ϸ��������ɫ
	gmw_set_delay(&MagicBall_CGI, DELAY_OF_BLOCK_MOVED, 50);	//��ʱ

	const BLOCK_DISPLAY_INFO bdi_normal[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //0����ʾ���ÿո���伴��
		{1, COLOR_HBLACK, -1, "��"},
		{2, COLOR_YELLOW, -1, "��"},
		{3, COLOR_HGREEN, -1, "��"},
		{4, COLOR_HCYAN, -1, "��"},
		{5, COLOR_HRED, -1, "��"},
		{6, COLOR_HPINK, -1, "��"},
		{7, COLOR_HYELLOW, -1, "��"},
		{8, COLOR_CYAN, -1, "��"},
		{9, COLOR_WHITE, -1, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ�ѡ��״̬�� */
	const BLOCK_DISPLAY_INFO bdi_selected[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
		{1, COLOR_HBLACK, -1, "��"},
		{2, COLOR_YELLOW, -1, "��"},
		{3, COLOR_HGREEN, -1, "��"},
		{4, COLOR_HCYAN, -1, "��"},
		{5, COLOR_HRED, -1, "��"},
		{6, COLOR_HPINK, -1, "��"},
		{7, COLOR_HYELLOW, -1, "��"},
		{8, COLOR_CYAN, -1, "��"},
		{9, COLOR_WHITE, -1, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���������ʾ״̬�� */
	const BLOCK_DISPLAY_INFO bdi_prompt[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
		{1, COLOR_HBLACK, -1, "��"},
		{2, COLOR_YELLOW, -1, "��"},
		{3, COLOR_HGREEN, -1, "��"},
		{4, COLOR_HCYAN, -1, "��"},
		{5, COLOR_HRED, -1, "��"},
		{6, COLOR_HPINK, -1, "��"},
		{7, COLOR_HYELLOW, -1, "��"},
		{8, COLOR_CYAN, -1, "��"},
		{9, COLOR_WHITE, -1, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	/* ����1-9�������ú�����ʽ��ʾ�ڽ����ϣ���ը/����״̬�� */
	const BLOCK_DISPLAY_INFO bdi_exploded[] = {
		{BDI_VALUE_BLANK, -1, -1, "  "},  //�հ�
		{1, COLOR_HBLACK, -1, "��"},
		{2, COLOR_YELLOW, -1, "��"},
		{3, COLOR_HGREEN, -1, "��"},
		{4, COLOR_HCYAN, -1, "��"},
		{5, COLOR_HRED, -1, "��"},
		{6, COLOR_HPINK, -1, "��"},
		{7, COLOR_HYELLOW, -1, "��"},
		{8, COLOR_CYAN, -1, "��"},
		{9, COLOR_WHITE, -1, "��"},
		{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};
	char temp[256];

	if (mode >= '0'&&mode <= '9')
		choice = mode - '0' + 1;
	else
		choice = mode - 'A' + 1;
	cls();
	input_int("����������(5-9)�� ", &row, 5, 9, 0, 0);
	input_int("����������(5-9)�� ", &col, 5, 9, 0, 1);

	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(&MagicBall_CGI, row, col);
	array_create(arr_ball, row, col);
	arr_reset(arr_selected);
	switch (choice)
	{
		case 1:
			cout << "\n��ʼ���飺";
			draw_text(arr_ball, row, col, 0, 4, 0);
			to_be_continued("����Ѱ�ҳ�ʼ��������Ĳ���");
			if (is_eliminate(arr_ball, row, col, arr_selected))
				print_result_text(arr_ball, arr_selected, row, col);
			else
				cout << "��ʼ���޿�������" << endl;
			break;
		case 2:
			cout << "\n��ʼ���飺";
			draw_text(arr_ball, row, col, 0, 4, 0);
			to_be_continued("����Ѱ�ҳ�ʼ��������Ĳ���");
			while (is_eliminate(arr_ball, row, col, arr_selected)) {
				print_result_text(arr_ball, arr_selected, row, col);
				to_be_continued("���س��������������估������");
				ball_remove_all(arr_ball, row, col, arr_selected);
				ball_fall_all(arr_ball, row, col);
				ball_add(arr_ball, row, col);
				cout << "�������������飺" << endl;
				print_text(arr_ball, row, col);
				to_be_continued("��������Ѱ�ҳ�ʼ��������Ĳ���");
			}
			cout << "��ʼ���޿�������" << endl;
			break;
		case 3:
			cout << "\n��ʼ���飺";
			draw_text(arr_ball, row, col, 0, 4, 0);
			to_be_continued("����Ѱ�ҳ�ʼ��������Ĳ���");
			while (is_eliminate(arr_ball, row, col, arr_selected)) {
				print_result_text(arr_ball, arr_selected, row, col);
				to_be_continued("���س��������������估������");
				ball_remove_all(arr_ball, row, col, arr_selected);
				ball_fall_all(arr_ball, row, col);
				ball_add(arr_ball, row, col);
				cout << "�������������飺" << endl;
				print_text(arr_ball, row, col);
				to_be_continued("��������Ѱ�ҳ�ʼ��������Ĳ���");
			}
			cout << "��ʼ���޿�������" << endl;
			if (find_notice(arr_ball, row, col, arr_selected)) {
				cout << "\n��ѡ���������ʾ��" << endl;
				print_result_text(arr_ball, arr_selected, row, col);
			}
			else
				cout << "�޿�ѡ���������ʾ��Game Over" << endl;
			break;
		case 4:
		case 5:
			cout << "\n��ʼ���飺";
			draw_text(arr_ball, row, col, 0, 4, 0);
			while (is_eliminate(arr_ball, row, col, arr_selected)) {
				to_be_continued("����Ѱ�ҳ�ʼ��������Ĳ���");
				print_result_text(arr_ball, arr_selected, row, col);
				to_be_continued("���س��������������估������");
				ball_remove_all(arr_ball, row, col, arr_selected);
				ball_fall_all(arr_ball, row, col);
				ball_add(arr_ball, row, col);
				cout << "�������������飺" << endl;
				print_text(arr_ball, row, col);
			}
			to_be_continued(NULL);
			if (choice - 4) {
				gmw_set_frame_style(&MagicBall_CGI, 2, 1, true);	//��Ϸ��������ÿ��ɫ���2��1���޷ָ���
				//draw_img_div(arr_ball, row, col, 0, 1);
			}
			else {
				gmw_set_frame_style(&MagicBall_CGI, 2, 1, false);	//��Ϸ��������ÿ��ɫ���2��1���зָ���
				//draw_img_no_div(arr_ball, row, col, 0, 1);
			}
			/* ��ʾ��� */
			gmw_draw_frame(&MagicBall_CGI);
			/* ��״̬����ʾ���� */
			sprintf(temp, "���ڴ�С��%d�� %d��", MagicBall_CGI.lines, MagicBall_CGI.cols);
			gmw_status_line(&MagicBall_CGI, TOP_STATUS_LINE, temp);

			/* ���ڲ�����չ�ֵ���Ļ�� */
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++) {
					gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
				}

			break;
		case 6:
			gmw_set_frame_style(&MagicBall_CGI, 2, 1, true);	//��Ϸ��������ÿ��ɫ���2��1���зָ���
			/* ��ʾ��� */
			gmw_draw_frame(&MagicBall_CGI);
			/* ��״̬����ʾ���� */
			sprintf(temp, "���ڴ�С��%d�� %d��", MagicBall_CGI.lines, MagicBall_CGI.cols);
			gmw_status_line(&MagicBall_CGI, TOP_STATUS_LINE, temp);

			/* ���ڲ�����չ�ֵ���Ļ�� */
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++) {
					gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
				}

			if (is_eliminate(arr_ball, row, col, arr_selected)) {
				//draw_img_div(arr_ball, row, col, 0, 1, arr_selected, COLOR_BLACK);
				for (int i = 0; i < row; i++)
					for (int j = 0; j < col; j++) {
						if (arr_selected[i + 1][j + 1])
							gmw_draw_block(&MagicBall_CGI, i, j, arr_selected[i + 1][j + 1], bdi_prompt);
					}
			}
			else
			{
				//draw_img_div(arr_ball, row, col, 0, 1);
				//showstr(14, 0, "δ�ҵ���������");
				/* ��״̬����ʾ���� */
				gmw_status_line(&MagicBall_CGI, LOWER_STATUS_LINE, "δ�ҵ���������", "���Խ�����"); //ֻ�Ǹ�����ʾ���ѣ�������������End����������Ҫ�����뼰�ж�

			}
			gotoxy(0, 2 * row + 3);
			break;
		case 7:
			//cls();
			gmw_set_frame_style(&MagicBall_CGI, 2, 1, true);	//��Ϸ��������ÿ��ɫ���2��1���зָ���
			/* ��ʾ��� */
			gmw_draw_frame(&MagicBall_CGI);
			/* ��״̬����ʾ���� */
			sprintf(temp, "���ڴ�С��%d�� %d��", MagicBall_CGI.lines, MagicBall_CGI.cols);
			gmw_status_line(&MagicBall_CGI, TOP_STATUS_LINE, temp);

			/* ���ڲ�����չ�ֵ���Ļ�� */
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++) {
					gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
				}

			if (is_eliminate(arr_ball, row, col, arr_selected)) {
				do {
					//draw_img_div(arr_ball, row, col, 0, 1, arr_selected, COLOR_BLACK);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_selected);
						}
					Sleep(50);
					//draw_img_removeball(arr_ball, row, col, 4, 3, arr_selected);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_exploded);
						}
					Sleep(50);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, 0, bdi_normal);
						}
					ball_remove_all(arr_ball, row, col, arr_selected);
					ball_fall_all(arr_ball, row, col, 1, 4, 3, &MagicBall_CGI);
					ball_add(arr_ball, row, col, arr_selected);

					//draw_ball_img_div_extra(arr_ball, row, col, 4, 3, arr_selected);
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_normal);
						}

					arr_reset(arr_selected);
				} while (is_eliminate(arr_ball, row, col, arr_selected));


				if (find_notice(arr_ball, row, col, arr_selected)) {
					//draw_ball_img_div_extra(arr_ball, row, col, 4, 3, arr_selected, "��");
					for (int i = 0; i < row; i++)
						for (int j = 0; j < col; j++) {
							if (arr_selected[i + 1][j + 1])
								gmw_draw_block(&MagicBall_CGI, i, j, arr_ball[i + 1][j + 1], bdi_prompt);
						}

				}
				else
				{
					gotoxy(0, 2 * row + 3);
					to_be_continued("�޿�ѡ���������ʾ��Game Over");
					return;
				}
			}
			else
			{
				draw_img_div(arr_ball, row, col, 0, 1);
				showstr(14, 0, "δ�ҵ���������");
			}
			gotoxy(0, 2 * row + 3);
			break;
		case 8:
			break;
			//case 9:
			//	ifstream fin;
			//	int np[MAX_ROW + 2][MAX_COL + 2] = { 0 };
			//	char s[12];
			//	cout << "����ѧ�ţ�";
			//	cin >> s;
			//	strcat(s, ".dat");
			//	fin.open(s, ios::in);
			//	fin >> row >> col;
			//	for (int i = 1; i <= row; i++)
			//		for (int j = 1; j < col; j++)
			//		{
			//			fin >> arr_ball[i][j];
			//			if (arr_ball[i][j] >= 90)
			//			{
			//				arr_ball[i][j] -= 90;
			//				np[i][j] = 1;
			//			}
			//		}
			//	fin.close();
			//	cls();
			//	cout << "\n��ʼ���飺";
			//	draw_text(arr_ball, row, col, 0, 4, 0);
			//	to_be_continued("����Ѱ�ҳ�ʼ��������Ĳ���");
			//	if (is_eliminate(arr_ball, row, col, arr_selected))
			//		print_result_text(arr_ball, arr_selected, row, col);
			//	else
			//		cout << "��ʼ���޿�������" << endl;

			//	cout << "\n��֤���飺";
			//	print_result_text(arr_ball, np, row, col);
			//	break;
		default:
			break;
	}
	to_be_continued(NULL);
}