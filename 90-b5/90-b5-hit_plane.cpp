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
#include "90-b5-hit_plane.h"
using namespace std;

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

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��1 - ����	0 - �˳�
  ˵    �����ֶ���Ϸģʽ
***************************************************************************/
int play_game_manual() {
	CONSOLE_GRAPHICS_INFO Ghitplane_CGI;	//����һ��CGI����
	const BLOCK_DISPLAY_INFO bdi_normal[] = {
	{ BDI_VALUE_BLANK, -1, -1, NULL },	//0����ʾ���ÿո���伴��
	{ 1, COLOR_HYELLOW, COLOR_BLACK,"��" },	//HitFail ��ѡ����Ϊ��
	{ 2, COLOR_HRED, COLOR_BLACK, "��" },	//HitBody ��ѡ����Ϊ����
	{ 3, COLOR_GREEN, COLOR_BLACK, "��" },	//HitHead ��ѡ����Ϊ��ͷ
	{ BDI_VALUE_END, -1, -1, NULL } //�жϽ�������ΪcontentΪNULL��ǰ��-999����ν
	};

	char temp[256];

	/* ���ع�� */
	setcursor(CURSOR_INVISIBLE);

	/* ��ʼ��GCGI�ṹ�� */
	gmw_init(&Ghitplane_CGI);

	gmw_set_color(&Ghitplane_CGI, COLOR_BLACK, COLOR_WHITE);		//����������ɫ
	gmw_set_font(&Ghitplane_CGI, "������", 16);						//����
	gmw_set_frame_style(&Ghitplane_CGI, 6, 3);						//��Ϸ��������ÿ��ɫ���10��5���ָ��ߡ�����ɫ����߿򣬿��Ϊ10(�����6λ����)���߶�Ϊ5(Ϊ�˱���ɫ��Ϊ����)��
	gmw_set_frame_color(&Ghitplane_CGI, COLOR_WHITE, COLOR_BLACK);	//��Ϸ��������ɫ
	gmw_set_frame_default_linetype(&Ghitplane_CGI, 2);				//��Ϸ����ָ��߷��-ȫ����
	gmw_set_rowno_switch(&Ghitplane_CGI, false);					//����ʾ�к�
	gmw_set_colno_switch(&Ghitplane_CGI, false);					//����ʾ�б�

	/* ��row/col��ֵ������Ϸ���������� */
	gmw_set_rowcol(&Ghitplane_CGI, 10, 10);		//д����10*10

	/* ��ʾ��� */
	gmw_draw_frame(&Ghitplane_CGI);

	//	��Ϸ����
	int loop = 1;
	int maction, mrow, mcol;
	int keycode1, keycode2;
	int ret;
	int score = 0;

	while (1)
	{
		/* ��״̬����ʾ���� */
		sprintf(temp, "��ǰ������%d ��R������ Q���˳���", score);
		gmw_status_line(&Ghitplane_CGI, TOP_STATUS_LINE, temp);

		loop = 1;
		while (loop) {
			ret = gmw_read_keyboard_and_mouse(&Ghitplane_CGI, maction, mrow, mcol, keycode1, keycode2, false);

			if (ret == CCT_MOUSE_EVENT) {



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
					default:

						break;
				}//end of switch
			}//end of else
		}//end of while

	}
}

#if 1
int main()
{
	play_game_manual();
}
#endif