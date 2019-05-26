// 1752762 ��1�� κ�ٴ�
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_console_tools.h"
#include "../common/cmd_gmw_tools.h"
#include "../common/cmd_tgmw_tools.h"
#include "90-b4.h"
using namespace std;

#define N_EVERY 2	//ÿ������ɫ��ĸ���

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾ������������Ϣ
  ���������
  �� �� ֵ��
  ˵    �������� CONSOLE_GRAPHICS_INFO �ṹ����λ����״̬��(x,y)��(0,y+2)λ��
***************************************************************************/
static void to_be_continued(const char *prompt, const CONSOLE_GRAPHICS_INFO *const bgi)
{
	setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //�ָ���ʼ��ɫ

	gotoxy(0, bgi->SLI.lower_start_y + 2);//����赽ָ��λ��
	cout << setw(bgi->cols - 1) << ' '; //���ÿո��������

	gotoxy(0, bgi->SLI.lower_start_y + 2);//����赽ָ��λ��
	if (prompt)
		cout << prompt << "�����س�������...";
	else
		cout << "���س�������...";

	while (_getch() != '\r')
		;

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾ������������Ϣ
  ���������
  �� �� ֵ��
  ˵    �������� CONSOLE_GRAPHICS_INFO �ṹ����λ����״̬��(x,y)��(0,y+2)λ��
***************************************************************************/
static void to_be_continued(const char *game_name, const char *prompt, const CONSOLE_GRAPHICS_INFO *const bgi)
{
	setcolor(bgi->area_bgcolor, bgi->area_fgcolor); //�ָ���ʼ��ɫ

	gotoxy(0, bgi->SLI.lower_start_y + 2);//����赽ָ��λ��
	cout << setw(bgi->cols - 1) << ' '; //���ÿո��������

	gotoxy(0, bgi->SLI.lower_start_y + 2);//����赽ָ��λ��
	cout << game_name << '-' << (bgi->CFI.separator ? "��" : "��") << "�ָ���-" << prompt << "�����س�������...";

	while (_getch() != '\r')
		;

	return;
}

//����-1 ��Ϸʧ��	����1 ��Ϸ�ɹ�	����0 ��Ϸ����
int is_over(const int score, const int ROW, const int COL, const int arr[MAX_ROW][MAX_COL]) {
	//Ӯ����Ϸ
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			if (arr[i][j] == score)
				return 1;

	//�пհ�
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			if (!arr[i][j])
				return 0;

	//����
	for (int i = 0; i < ROW; ++i)
		for (int j = 0; j < COL - 1; ++j)
			if (arr[i][j] && arr[i][j] == arr[i][j + 1])
				return 0;

	//����
	for (int j = 0; j < COL; ++j)
		for (int i = 0; i < ROW - 1; ++i)
			if (arr[i][j] && arr[i][j] == arr[i + 1][j])
				return 0;

	//�Ѿ���·����
	return -1;
}

//����0 �����˳�	����1���¿�ʼ	����-1 �쳣�˳�
int my_2048() {
	srand((unsigned int)time(NULL));
	CONSOLE_GRAPHICS_INFO G2048_CGI; //����һ��CGI����
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{BDI_VALUE_BLANK, -1, -1, NULL},  //0����ʾ���ÿո���伴��
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
	{131072, COLOR_HPINK, COLOR_BLACK, NULL},  //������ģ������Լ�����
	{BDI_VALUE_END, -1, -1, NULL} //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};

	setcursor(CURSOR_INVISIBLE);
	gmw_init(&G2048_CGI);
	gmw_set_color(&G2048_CGI, COLOR_BLACK, COLOR_WHITE);		//����������ɫ
	gmw_set_font(&G2048_CGI, "������", 16);						//����
	gmw_set_frame_style(&G2048_CGI, 10, 5, false);				//��Ϸ��������ÿ��ɫ���10��5���޷ָ��ߡ�����ɫ����߿򣬿��Ϊ10(�����6λ����)���߶�Ϊ5(Ϊ�˱���ɫ��Ϊ����)��
	gmw_set_frame_color(&G2048_CGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
	gmw_set_block_border_switch(&G2048_CGI, true);				//Сɫ����߿�
	gmw_set_rowno_switch(&G2048_CGI, false);					//����ʾ�к�
	gmw_set_colno_switch(&G2048_CGI, false);					//����ʾ�б�

	while (1) {
		int arr2048[MAX_ROW][MAX_COL] = { 0 };
		int row = 8, col = 8, target_score = 8192;
		char temp[256];
		setcolor(G2048_CGI.area_bgcolor, G2048_CGI.area_fgcolor);
		setconsoleborder(80, 25, 80, 1000);
		//û�п�����ʾ����
		cout << "��Ϸ2048�������ã�" << endl;
		while (1) {
			cout << "����������[4-8]" << endl;
			cin >> row;
			cin.clear();
			cin.ignore(1024, '\n');
			if (row >= 4 && row <= 8)
				break;
			cout << "������� ����������" << endl;
		}
		while (1) {
			cout << "����������[4-10]" << endl;
			cin >> col;
			cin.clear();
			cin.ignore(1024, '\n');
			if (col >= 4 && col <= 10)
				break;
			cout << "������� ����������" << endl;
		}
		while (1) {
			cout << "������Ŀ�����[8192/16384/32768/65536]" << endl;
			cin >> target_score;
			cin.clear();
			cin.ignore(1024, '\n');
			if (target_score == 8192 || target_score == 16384 || target_score == 32768 || target_score == 65536)
				break;
			cout << "������� ����������" << endl;
		}

		/* ��row/col��ֵ������Ϸ���������� */
		gmw_set_rowcol(&G2048_CGI, row, col);

		/* ��ʾ��� */
		gmw_draw_frame(&G2048_CGI);


		//��ʼ N_EVERY��ɫ��
		for (int n_eve = 0; n_eve < N_EVERY; n_eve++)
			create_single(row, col, arr2048);

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++) {
				gmw_draw_block(&G2048_CGI, i, j, arr2048[i][j], bdi_normal);
			}

		int loop = 1, is_move = 0;
		int maction, mrow, mcol;
		int keycode1, keycode2;
		int ret;
		while (1) {	

			/* ��״̬����ʾ���� */
			sprintf(temp, "Ŀ�꣺%d ��R������ Q���˳���", target_score);
			gmw_status_line(&G2048_CGI, TOP_STATUS_LINE, temp);

			loop = 1;
			while (loop) {
				ret = gmw_read_keyboard_and_mouse(&G2048_CGI, maction, mrow, mcol, keycode1, keycode2, false);

				if (ret == CCT_MOUSE_EVENT) {
					continue;
				}
				else { //CCT_KEYBOARD_EVENT - ������뺬��ȥ�� cmd_console_tools.cpp
					switch (keycode1) {
						case 'R':
						case 'r':
							return 1;	//����
							break;
						case 'q':
						case 'Q':
							return 0;
							break;
						case 0xE0: //224
							switch (keycode2) {
								case KB_ARROW_UP:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, DOWN_TO_UP);
									break;
								case KB_ARROW_DOWN:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, UP_TO_DOWN);
									break;
								case KB_ARROW_LEFT:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, RIGHT_TO_LEFT);
									break;
								case KB_ARROW_RIGHT:
									loop = 0;
									is_move = arr_move_merge(row, col, arr2048, &G2048_CGI, 0, bdi_normal, LEFT_TO_RIGHT);
									break;
							}
							break;
						default:
							break;
					}//end of switch

				}//end of else

			}//end of while

			int row_new, col_new;
			if (is_move) {
				if (is_over(target_score, row, col, arr2048) != 0)	//��Ϸ������ʧ��/�ɹ���
					return 0;
				//��Ϸ��δ����
				//ÿ��������������
				for (int n_eve = 0; n_eve < N_EVERY; n_eve++) {
					create_single(row, col, arr2048, &row_new, &col_new);
					gmw_draw_block(&G2048_CGI, row_new, col_new, arr2048[row_new][col_new], bdi_normal);
				}
			}

		}
	}
	return 0;
}