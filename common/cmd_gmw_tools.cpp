// 1752762 ��1�� κ�ٴ�
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
#include <conio.h>
#include "cmd_console_tools.h"
#include "cmd_gmw_tools.h"
using namespace std;

/* --------------------------------------------------
		�˴����Ը�����Ҫ�ľ�̬ȫ�ֱ������������٣����û�У�����̬ȫ��ֻ������/�궨�壨�������ޣ���
   -------------------------------------------------- */


   /* --------------------------------------------------
		   �˴����Ը�����Ҫ���ڲ��������ߺ���
		   1�����������ޣ�����Ϊ gmw_inner_*
		   2����������
		   3��������static������ȷ��ֻ�ڱ�Դ�ļ���ʹ��
	  -------------------------------------------------- */


	  /* -----------------------------------------------
			  ʵ����������ĺ���������������׼����
		 ----------------------------------------------- */
		 /***************************************************************************
		   �������ƣ�
		   ��    �ܣ�������Ϸ����ܵ�������
		   ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
					 const int row						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
					 const int col						������(������Ϊ0���������ޣ���Ϊ��֤��ȷ��)
		   �� �� ֵ��
		   ˵    ����1��ָ��������Ϸ�ľ������������ֵ
					 2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
		 ***************************************************************************/
int gmw_set_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col)
{
	//����ܲ���
	int t = row <= 0 ? 10 : row;
	pCGI->row_num = t;	//����ܰ�����ɫ�������
	t = col <= 0 ? 10 : col;
	pCGI->col_num = t;	//����ܰ�����ɫ�������

	//����cmd���ڴ�С
	//Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬Ҫ���ڼ���õ��Ľ��������
	//���϶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4��1�������뷨��ʾ��+3Ԥ�����У�
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_row_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no * 2 + 1;

	//��״̬��λ��
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->draw_frame_with_row_no;
	//��״̬�����
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->draw_frame_with_col_no * 2;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������������ڣ�����Ϸ�������������ڵ�����cmd���ڣ�����ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int bg_color					��ǰ��ɫ��ȱʡCOLOR_BLACK��
		   const int fg_color					������ɫ��ȱʡCOLOR_WHITE��
		   const bool cascade					���Ƿ�����ȱʡΪtrue-������
  �� �� ֵ��
  ˵    ����1��cascade = trueʱ
				ͬ���޸���Ϸ���������ɫ
				ͬ���޸�����״̬���������ı��ı���ɫ��ǰ��ɫ����Ŀ�ı��ı���ɫ��ǰ��ɫ���䣩
			2���������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15
					ǰ��ɫ����ɫ��ֵһ�µ����޷���������
					ǰ��ɫ������״̬����Ŀǰ��ɫ�������޷�������Ŀ��ʾ
					...
***************************************************************************/
int gmw_set_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor, const bool cascade)
{
	//��������򱳾�ɫ
	//int t = (bgcolor < 0 || bgcolor>15) ? COLOR_BLACK : bgcolor;  //����飿
	pCGI->area_bgcolor = bgcolor;
	//���������ǰ��ɫ
	//t = (fgcolor < 0 || fgcolor>15) < 0 ? COLOR_WHITE : fgcolor;
	pCGI->area_fgcolor = fgcolor;

	if (cascade) {
		//�����򲿷�
		pCGI->CFI.bgcolor = bgcolor;
		pCGI->CFI.fgcolor = fgcolor;
		//״̬������
		pCGI->SLI.top_normal_bgcolor = pCGI->area_bgcolor;	//�����ı���ɫͬ����һ��
		pCGI->SLI.top_normal_fgcolor = pCGI->area_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = pCGI->area_bgcolor;	//��Ŀ�ı�������ɫͬ����һ��
		pCGI->SLI.lower_normal_bgcolor = pCGI->area_bgcolor;	//�����ı���ɫͬ����һ��
		pCGI->SLI.lower_normal_fgcolor = pCGI->area_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = pCGI->area_bgcolor;	//��Ŀ�ı�������ɫͬ����һ��
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ô��ڵ�����
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *fontname					���������ƣ�ֻ����"Terminal"��"������"���֣������򷵻�-1�����ı����壩
		   const int fs_high					������߶ȣ�ȱʡ������Ϊ16�������������ƣ���Ϊ��֤��
		   const int fs_width					������߶ȣ�ȱʡ������Ϊ8�������������ƣ���Ϊ��֤��
  �� �� ֵ��
  ˵    ����1����cmd_console_tools�е�setfontsize���ƣ�Ŀǰֻ֧�֡��������塱�͡������塱
			2������������������ֱ�ӷ��أ�����ԭ�������ò���
***************************************************************************/
int gmw_set_font(CONSOLE_GRAPHICS_INFO *const pCGI, const char *fontname, const int fs_high, const int fs_width)
{
	//���岿��
	strcpy(pCGI->CFT.font_type, fontname);	//Ĭ�ϵ���8*16
	pCGI->CFT.font_size_high = fs_high;
	pCGI->CFT.font_size_width = fs_width;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ʱ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type						����ʱ�����ͣ�ĿǰΪ3�֣�
		   const int delay_ms					����msΪ��λ����ʱ
			   ���߿����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ��ɫ�����ʱ��0 ~ �������ޣ���Ϊ��֤��ȷ��<0����0��
			   ɫ���ƶ�����ʱ��BLOCK_MOVED_DELAY_MS ~ �������ޣ���Ϊ��֤��ȷ�� <BLOCK_MOVED_DELAY_MS ���� BLOCK_MOVED_DELAY_MS��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_delay(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int delay_ms)
{
	int dm = delay_ms < 0 ? 0 : delay_ms;

	if (type == DELAY_OF_BLOCK_MOVED)
		pCGI->delay_of_block_moved = dm;
	else if (type == DELAY_OF_DRAW_BLOCK)
		pCGI->delay_of_draw_block = dm;
	else if (type == DELAY_OF_DRAW_FRAME)
		pCGI->delay_of_draw_frame = dm;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ���������������Ϸ����ܽṹ֮����Ҫ�����Ķ�������
  ��    �ܣ�CONSOLE_GRAPHICS_INFO *const pCGI	��
		   const int up_lines					���ϲ�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int down_lines				���²�������У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int left_cols					����߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
		   const int right_cols				���ұ߶�����У�ȱʡ������Ϊ0���������ޣ���Ϊ��֤��
  �� �� ֵ��
  ˵    �����������еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_ext_rowcol(CONSOLE_GRAPHICS_INFO *const pCGI, const int up_lines, const int down_lines, const int left_cols, const int right_cols)
{
	//�������и���
	pCGI->extern_up_lines = up_lines < 0 ? 0 : up_lines;
	pCGI->extern_down_lines = down_lines < 0 ? 0 : down_lines;
	pCGI->extern_left_cols = left_cols < 0 ? 0 : left_cols;
	pCGI->extern_right_cols = right_cols < 0 ? 0 : right_cols;

	//������Աֵ����
	//���������ο�������ʼλ��
	pCGI->start_x = pCGI->extern_left_cols;
	pCGI->start_y = pCGI->extern_up_lines;

	//״̬������
	pCGI->SLI.top_start_x = pCGI->start_x;
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.lower_start_x = pCGI->start_x;
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->draw_frame_with_row_no;
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->draw_frame_with_col_no * 2;

	//����cmd���ڴ�С
	//Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬Ҫ���ڼ���õ��Ľ��������
	//���϶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4��1�������뷨��ʾ��+3Ԥ�����У�
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + (2 + pCGI->CFI.separator*(pCGI->row_num - 1)) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_row_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no * 2 + 1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_frame_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	//������������Ԫ�ص���״
	if (type == 1) {
		strcpy(pCGI->CFI.top_left, "�X");
		strcpy(pCGI->CFI.lower_left, "�^");
		strcpy(pCGI->CFI.top_right, "�[");
		strcpy(pCGI->CFI.lower_right, "�a");
		strcpy(pCGI->CFI.h_normal, "�T");
		strcpy(pCGI->CFI.v_normal, "�U");
		strcpy(pCGI->CFI.h_top_separator, "�j");
		strcpy(pCGI->CFI.h_lower_separator, "�m");
		strcpy(pCGI->CFI.v_left_separator, "�d");
		strcpy(pCGI->CFI.v_right_separator, "�g");
		strcpy(pCGI->CFI.mid_separator, "�p");
	}
	else if (type == 2) {
		strcpy(pCGI->CFI.top_left, "��");
		strcpy(pCGI->CFI.lower_left, "��");
		strcpy(pCGI->CFI.top_right, "��");
		strcpy(pCGI->CFI.lower_right, "��");
		strcpy(pCGI->CFI.h_normal, "��");
		strcpy(pCGI->CFI.v_normal, "��");
		strcpy(pCGI->CFI.h_top_separator, "��");
		strcpy(pCGI->CFI.h_lower_separator, "��");
		strcpy(pCGI->CFI.v_left_separator, "��");
		strcpy(pCGI->CFI.v_right_separator, "��");
		strcpy(pCGI->CFI.mid_separator, "��");
	}
	else if (type == 3) {
		strcpy(pCGI->CFI.top_left, "�V");
		strcpy(pCGI->CFI.lower_left, "�\");
		strcpy(pCGI->CFI.top_right, "�Y");
		strcpy(pCGI->CFI.lower_right, "�_");
		strcpy(pCGI->CFI.h_normal, "�T");
		strcpy(pCGI->CFI.v_normal, "��");
		strcpy(pCGI->CFI.h_top_separator, "�h");
		strcpy(pCGI->CFI.h_lower_separator, "�k");
		strcpy(pCGI->CFI.v_left_separator, "�b");
		strcpy(pCGI->CFI.v_right_separator, "�e");
		strcpy(pCGI->CFI.mid_separator, "�n");
	}
	else if (type == 4)
	{
		strcpy(pCGI->CFI.top_left, "�W");
		strcpy(pCGI->CFI.lower_left, "�]");
		strcpy(pCGI->CFI.top_right, "�Z");
		strcpy(pCGI->CFI.lower_right, "�`");
		strcpy(pCGI->CFI.h_normal, "��");
		strcpy(pCGI->CFI.v_normal, "�U");
		strcpy(pCGI->CFI.h_top_separator, "�i");
		strcpy(pCGI->CFI.h_lower_separator, "�l");
		strcpy(pCGI->CFI.v_left_separator, "�c");
		strcpy(pCGI->CFI.v_right_separator, "�f");
		strcpy(pCGI->CFI.mid_separator, "�o");
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�11������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const char *...						����11�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_frame_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left, const char *top_right,
	const char *lower_right, const char *h_normal, const char *v_normal, const char *h_top_separator,
	const char *h_lower_separator, const char *v_left_separator, const char *v_right_separator, const char *mid_separator)
{
	char temp[5] = { '\0' };

	strncpy(temp, top_left ? top_left : "  ", 2);
	temp[top_left ? strlen(top_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.top_left, temp, 2);

	strncpy(temp, lower_left ? lower_left : "  ", 2);
	temp[lower_left ? strlen(lower_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.lower_left, temp, 2);

	strncpy(temp, top_right ? top_right : "  ", 2);
	temp[top_right ? strlen(top_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.top_right, temp, 2);

	strncpy(temp, lower_right ? lower_right : "  ", 2);
	temp[lower_right ? strlen(lower_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.lower_right, temp, 2);

	strncpy(temp, h_normal ? h_normal : "  ", 2);
	temp[h_normal ? strlen(h_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.h_normal, temp, 2);

	strncpy(temp, v_normal ? v_normal : "  ", 2);
	temp[v_normal ? strlen(v_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.v_normal, temp, 2);

	strncpy(temp, h_top_separator ? h_top_separator : "  ", 2);
	temp[h_top_separator ? strlen(h_top_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.h_top_separator, temp, 2);

	strncpy(temp, h_lower_separator ? h_lower_separator : "  ", 2);
	temp[h_lower_separator ? strlen(h_lower_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.h_lower_separator, temp, 2);

	strncpy(temp, v_left_separator ? v_left_separator : "  ", 2);
	temp[v_left_separator ? strlen(v_left_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.v_left_separator, temp, 2);

	strncpy(temp, v_right_separator ? v_right_separator : "  ", 2);
	temp[v_right_separator ? strlen(v_right_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.v_right_separator, temp, 2);

	strncpy(temp, mid_separator ? mid_separator : "  ", 2);
	temp[mid_separator ? strlen(mid_separator) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CFI.mid_separator, temp, 2);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_FRAME_TYPE �ṹ�е�ɫ��������С���Ƿ���Ҫ�ָ��ߵ�
  ������������������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int block_width						����ȣ�����ȱʡ2����ΪԼ�������Ϊ�����Ʊ�����������������Ҫ+1��
			const int block_high						���߶ȣ�����ȱʡ1��
			const bool separator						���Ƿ���Ҫ�ָ��ߣ�ȱʡΪtrue����Ҫ�ָ��ߣ�
  �� �� ֵ��
  ˵    ������ܴ�С/�Ƿ���Ҫ�ָ��ߵȵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_frame_style(CONSOLE_GRAPHICS_INFO *const pCGI, const int block_width, const int block_high, const bool separator)
{
	if (block_width <= 0)
		pCGI->CFI.block_width = 2;
	else if (block_width % 2)
		pCGI->CFI.block_width = block_width + 1;
	else
		pCGI->CFI.block_width = block_width;

	if (block_high <= 0)
		pCGI->CFI.block_high = 1;
	else
		pCGI->CFI.block_high = block_high;

	pCGI->CFI.separator = separator;

	pCGI->CFI.block_width_ext = 2 * pCGI->CFI.separator;
	pCGI->CFI.block_high_ext = 1 * pCGI->CFI.separator;
	//���������ɫ�� ÿ��/ÿ���ܿ�ȣ����ָ��ߣ�
	pCGI->CFI.bwidth = pCGI->CFI.block_width + 2 * pCGI->CFI.separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.separator;

	//״̬������
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line;
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1));

	//����cmd���ڴ�С
	//Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬Ҫ���ڼ���õ��Ľ��������
	//���϶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4��1�������뷨��ʾ��+3Ԥ�����У�
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_row_no + pCGI->top_status_line + pCGI->lower_status_line + 4;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_col_no * 2 + 1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BORDER_TYPE �ṹ�е���ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int bg_color					������ɫ��ȱʡ -1��ʾ�ô��ڱ���ɫ��
			const int fg_color					��ǰ��ɫ��ȱʡ -1��ʾ�ô���ǰ��ɫ��
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_frame_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int bgcolor, const int fgcolor)
{
	pCGI->CFI.bgcolor = bgcolor == -1 ? pCGI->area_bgcolor : bgcolor;
	pCGI->CFI.fgcolor = fgcolor == -1 ? pCGI->area_fgcolor : fgcolor;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6�����ͣ�ȱʡ4�֣�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��1 - ȫ˫�� 2 - ȫ���� 3 - ��˫���� 4 - �ᵥ��˫
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_set_block_default_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const int type)
{
	if (type == 1) {
		strcpy(pCGI->CBI.top_left, "�X");
		strcpy(pCGI->CBI.lower_left, "�^");
		strcpy(pCGI->CBI.top_right, "�[");
		strcpy(pCGI->CBI.lower_right, "�a");
		strcpy(pCGI->CBI.h_normal, "�T");
		strcpy(pCGI->CBI.v_normal, "�U");
	}
	else if (type == 2) {
		strcpy(pCGI->CBI.top_left, "��");
		strcpy(pCGI->CBI.lower_left, "��");
		strcpy(pCGI->CBI.top_right, "��");
		strcpy(pCGI->CBI.lower_right, "��");
		strcpy(pCGI->CBI.h_normal, "��");
		strcpy(pCGI->CBI.v_normal, "��");
	}
	else if (type == 3) {
		strcpy(pCGI->CBI.top_left, "�V");
		strcpy(pCGI->CBI.lower_left, "�\");
		strcpy(pCGI->CBI.top_right, "�Y");
		strcpy(pCGI->CBI.lower_right, "�_");
		strcpy(pCGI->CBI.h_normal, "�T");
		strcpy(pCGI->CBI.v_normal, "��");
	}
	else if (type == 4)
	{
		strcpy(pCGI->CBI.top_left, "�W");
		strcpy(pCGI->CBI.lower_left, "�]");
		strcpy(pCGI->CBI.top_right, "�Z");
		strcpy(pCGI->CBI.lower_right, "�`");
		strcpy(pCGI->CBI.h_normal, "��");
		strcpy(pCGI->CBI.v_normal, "�U");
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���� CONSOLE_BLOCK_INFO �ṹ�е�6������
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const char *...					����6�֣������.h���˴���
  �� �� ֵ��
  ˵    ����Լ��Ϊһ�������Ʊ��������ʹ���������ݣ���Ϊ��֤2�ֽ�
			1������2�ֽ���ֻȡǰ2�ֽ�
			2�������NULL���������ո����
			3�������1�ֽڣ���һ���ո������˶�������ʾ�ң������
***************************************************************************/
int gmw_set_block_linetype(CONSOLE_GRAPHICS_INFO *const pCGI, const char *top_left, const char *lower_left,
	const char *top_right, const char *lower_right, const char *h_normal, const char *v_normal)
{
	char temp[5] = { '\0' };

	strncpy(temp, top_left ? top_left : "  ", 2);
	temp[top_left ? strlen(top_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.top_left, temp, 2);

	strncpy(temp, lower_left ? lower_left : "  ", 2);
	temp[lower_left ? strlen(lower_left) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.lower_left, temp, 2);

	strncpy(temp, top_right ? top_right : "  ", 2);
	temp[top_right ? strlen(top_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.top_right, temp, 2);

	strncpy(temp, lower_right ? lower_right : "  ", 2);
	temp[lower_right ? strlen(lower_right) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.lower_right, temp, 2);

	strncpy(temp, h_normal ? h_normal : "  ", 2);
	temp[h_normal ? strlen(h_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.h_normal, temp, 2);

	strncpy(temp, v_normal ? v_normal : "  ", 2);
	temp[v_normal ? strlen(v_normal) : 2] = '\0';
	strcat(temp, "  ");
	strncpy(pCGI->CBI.v_normal, temp, 2);

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ÿ����Ϸɫ��(����)�Ƿ���ҪС�߿�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡfalse��
  �� �� ֵ��
  ˵    �����߿�Լ��Ϊ�����Ʊ����˫��
***************************************************************************/
int gmw_set_block_border_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->CBI.block_border = on_off;
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ����״̬��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const bool on_off					��true - ��Ҫ flase - ����Ҫ��ȱʡtrue��
  �� �� ֵ��
  ˵    ����1��״̬�������Լ�����£�
			   1.1����״̬��ֻ��һ�У������������Ϸ�����/�б�����棬Ϊ��������ʼһ�У�����������Ͻ����������״̬�������꣩
			   1.2����״̬��ֻ��һ�У������������·����ߵ�����
			   1.3��״̬���Ŀ��Ϊ�������ȣ������Ϣ������ض�
		   2�����еı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_status_line_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const bool on_off)
{
	if (type == TOP_STATUS_LINE) {
		pCGI->top_status_line = on_off;
		pCGI->SLI.is_top_status_line = on_off;
	}
	else if (type == LOWER_STATUS_LINE) {
		pCGI->lower_status_line = on_off;
		pCGI->SLI.is_lower_status_line = on_off;
	}
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->extern_up_lines + pCGI->draw_frame_with_col_no;
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���������״̬������ɫ
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const int type						��״̬�����ͣ���/�£�
			const int normal_bgcolor			�������ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int normal_fgcolor			�������ı�ǰ��ɫ��ȱʡ -1��ʾʹ�ô���ǰ��ɫ��
			const int catchy_bgcolor			����Ŀ�ı�����ɫ��ȱʡ -1��ʾʹ�ô��ڱ���ɫ��
			const int catchy_fgcolor			����Ŀ�ı�ǰ��ɫ��ȱʡ -1��ʾʹ������ɫ��
  ���������
  �� �� ֵ��
  ˵    �����������ɫֵ���󼰳�ͻ����Ҫ��Ϊ��֤
				������ɫ��0-15��ǰ��ɫ����ɫ��ֵһ�µ����޷��������ݵ�
***************************************************************************/
int gmw_set_status_line_color(CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const int normal_bgcolor, const int normal_fgcolor, const int catchy_bgcolor, const int catchy_fgcolor)
{
	//״̬������
	if (type == TOP_STATUS_LINE) {
		pCGI->SLI.top_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;	//ȱʡ ��ɫͬ����һ��
		pCGI->SLI.top_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.top_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;	//ȱʡ ��Ŀ�ı�������ɫͬ����һ��
		pCGI->SLI.top_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;	//ȱʡ ��Ŀ�ı�ǰ��Ϊ����
	}
	else if (type == LOWER_STATUS_LINE) {
		pCGI->SLI.lower_normal_bgcolor = normal_bgcolor == -1 ? pCGI->area_bgcolor : normal_bgcolor;	//ȱʡ ��ɫͬ����һ��
		pCGI->SLI.lower_normal_fgcolor = normal_fgcolor == -1 ? pCGI->area_fgcolor : normal_fgcolor;
		pCGI->SLI.lower_catchy_bgcolor = catchy_bgcolor == -1 ? pCGI->area_bgcolor : catchy_bgcolor;	//ȱʡ ��Ŀ�ı�������ɫͬ����һ��
		pCGI->SLI.lower_catchy_fgcolor = catchy_fgcolor == -1 ? COLOR_HYELLOW : catchy_fgcolor;	//ȱʡ ��Ŀ�ı�ǰ��Ϊ����
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�к�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���к�Լ��Ϊ��ĸA��ʼ�������У��������26�����a��ʼ������52��ͳһΪ*��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�кŵı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_rowno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_row_no = on_off;

	pCGI->SLI.width = 2 * pCGI->draw_frame_with_row_no + pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1));

	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no * 2 + 1;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ƿ���ʾ�б�
  ���������CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
			const bool on_off					��true - ��ʾ flase - ����ʾ��ȱʡfalse��
  �� �� ֵ��
  ˵    ����1���б�Լ��Ϊ����0��ʼ�������У�����0-99������99ͳһΪ**��ʵ��Ӧ�ò����ܣ�
			2���Ƿ���ʾ�б�ı仯�ᵼ��CONSOLE_GRAPHICS_INFO�ṹ����������Աֵ�ı仯��Ҫ����
***************************************************************************/
int gmw_set_colno_switch(CONSOLE_GRAPHICS_INFO *const pCGI, const bool on_off)
{
	pCGI->draw_frame_with_col_no = on_off;

	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->draw_frame_with_col_no + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line;

	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡ CONSOLE_GRAPHICS_INFO �ṹ���еĸ���Աֵ
  ���������
  �� �� ֵ��
  ˵    ����1�����������ã���ӡ��ʽ�Զ���
			2������������������δ���ù������Բ�ʵ��
***************************************************************************/
int gmw_print(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��� CONSOLE_GRAPHICS_INFO �ṹ����ȱʡֵ���г�ʼ��
  ���������CONSOLE_GRAPHICS_INFO *const pCGI������ṹָ��
		   const int row					����Ϸ����ɫ��������ȱʡ10��
		   const int col					����Ϸ����ɫ��������ȱʡ10��
		   const int bgcolor				���������ڱ���ɫ��ȱʡ COLOR_BLACK��
		   const int fgcolor				���������ڱ���ɫ��ȱʡ COLOR_WHITE��
  �� �� ֵ��
  ˵    �������ڱ�����/ǰ���ף�����16*8�����������޶������У�����״̬�����У����к�/�б꣬�������Ϊ˫�ߣ�ɫ����2/�߶�1/��С�߿���ɫ��
***************************************************************************/
int gmw_init(CONSOLE_GRAPHICS_INFO *const pCGI, const int row, const int col, const int bgcolor, const int fgcolor)
{
	//����ܲ���
	int t = row <= 0 ? 10 : row;
	pCGI->row_num = t;	//����ܰ�����ɫ�������
	t = col <= 0 ? 10 : col;
	pCGI->col_num = t;	//����ܰ�����ɫ�������

	//���������ο�������ʼλ��
	pCGI->start_x = 0;
	pCGI->start_y = 0;

	//������������Ԫ�ص���״
	strcpy(pCGI->CFI.top_left, "�X");
	strcpy(pCGI->CFI.lower_left, "�^");
	strcpy(pCGI->CFI.top_right, "�[");
	strcpy(pCGI->CFI.lower_right, "�a");
	strcpy(pCGI->CFI.h_normal, "�T");
	strcpy(pCGI->CFI.v_normal, "�U");
	strcpy(pCGI->CFI.h_top_separator, "�j");
	strcpy(pCGI->CFI.h_lower_separator, "�m");
	strcpy(pCGI->CFI.v_left_separator, "�d");
	strcpy(pCGI->CFI.v_right_separator, "�g");
	strcpy(pCGI->CFI.mid_separator, "�p");

	//��������򱳾�ɫ
	t = col < 0 ? COLOR_BLACK : bgcolor;
	pCGI->CFI.bgcolor = t;
	pCGI->area_bgcolor = t;
	//���������ǰ��ɫ
	t = col < 0 ? COLOR_WHITE : fgcolor;
	pCGI->CFI.fgcolor = t;
	pCGI->area_fgcolor = t;

	//���������ɫ�� ��ȡ��߶�
	pCGI->CFI.block_width = 2;
	pCGI->CFI.block_high = 1;
	//���������ɫ�� �ָ��ߣ�Ĭ���У�
	pCGI->CFI.separator = true;
	//ÿ��ɫ�鸽�ӵĿ�Ⱥ͸߶�
	pCGI->CFI.block_width_ext = 2 * pCGI->CFI.separator;
	pCGI->CFI.block_high_ext = 1 * pCGI->CFI.separator;
	//���������ɫ�� ÿ��/ÿ���ܿ�ȣ����ָ��ߣ�
	pCGI->CFI.bwidth = pCGI->CFI.block_width + 2 * pCGI->CFI.separator;
	pCGI->CFI.bhigh = pCGI->CFI.block_high + pCGI->CFI.separator;

	//CFI��䲿����0
	memset(pCGI->CFI.pad1, '\0', sizeof(pCGI->CFI.pad1));
	memset(pCGI->CFI.pad2, '\0', sizeof(pCGI->CFI.pad2));
	memset(pCGI->CFI.pad, '\0', sizeof(pCGI->CFI.pad));

	//ɫ����Ϣ����
	strcpy(pCGI->CBI.top_left, "�X");
	strcpy(pCGI->CBI.lower_left, "�^");
	strcpy(pCGI->CBI.top_right, "�[");
	strcpy(pCGI->CBI.lower_right, "�a");
	strcpy(pCGI->CBI.h_normal, "�T");
	strcpy(pCGI->CBI.v_normal, "�U");
	pCGI->CBI.block_border = false;		//Ĭ��ɫ���ޱ߿�

	//CBI��䲿����0
	memset(&pCGI->CBI.pad1, '\0', sizeof(pCGI->CBI.pad1));
	memset(pCGI->CBI.pad, '\0', sizeof(pCGI->CBI.pad));

	//״̬������
	pCGI->top_status_line = true;	//Ĭ�Ͽ�����״̬��
	pCGI->SLI.is_top_status_line = true;
	pCGI->SLI.top_start_x = pCGI->start_x;	//λ�ã�0��0��
	pCGI->SLI.top_start_y = pCGI->start_y;
	pCGI->SLI.top_normal_bgcolor = pCGI->area_bgcolor;	//�����ı���ɫͬ����һ��
	pCGI->SLI.top_normal_fgcolor = pCGI->area_fgcolor;
	pCGI->SLI.top_catchy_bgcolor = pCGI->area_bgcolor;	//��Ŀ�ı�������ɫͬ����һ��
	pCGI->SLI.top_catchy_fgcolor = COLOR_HYELLOW;	//��Ŀ�ı�ǰ��Ϊ����
	pCGI->lower_status_line = true;	//Ĭ�Ͽ�����״̬��
	pCGI->SLI.is_lower_status_line = true;
	pCGI->SLI.lower_start_x = pCGI->start_x;	//λ�ã�0��...��
	pCGI->SLI.lower_start_y = pCGI->start_y + pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->top_status_line + pCGI->draw_frame_with_col_no;
	pCGI->SLI.lower_normal_bgcolor = pCGI->area_bgcolor;	//�����ı���ɫͬ����һ��
	pCGI->SLI.lower_normal_fgcolor = pCGI->area_fgcolor;
	pCGI->SLI.lower_catchy_bgcolor = pCGI->area_bgcolor;	//��Ŀ�ı�������ɫͬ����һ��
	pCGI->SLI.lower_catchy_fgcolor = COLOR_HYELLOW;	//��Ŀ�ı�ǰ��Ϊ����
	pCGI->SLI.width = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1));

	//SLI��䲿����0
	memset(pCGI->SLI.pad1, '\0', sizeof(pCGI->SLI.pad1));
	memset(pCGI->SLI.pad, '\0', sizeof(pCGI->SLI.pad));

	//���岿��
	strcpy(pCGI->CFT.font_type, "Terminal");	//Ĭ�ϵ���8*16
	pCGI->CFT.font_size_high = 16;
	pCGI->CFT.font_size_width = 8;

	//Ĭ�ϲ���ʾ�кż��б�
	pCGI->draw_frame_with_row_no = false;
	pCGI->draw_frame_with_col_no = false;

	pCGI->delay_of_draw_frame = 0;	//�������Ҹ�������ȫ��Ϊ0
	pCGI->delay_of_draw_block = 0;	//���߿�ɫ��ʱ����ʱ
	pCGI->delay_of_block_moved = 3;	//ɫ���ƶ�ʱ��ʱ3ms

	//����cmd���ڴ�С
	//Ϊ�˸��������뷨��ʾ�м����н�������ʾ��Ϣ���ռ䣬Ҫ���ڼ���õ��Ľ��������
	//���϶���ռ�+��״̬��+�б���ʾ+������+��״̬����+ 4��1�������뷨��ʾ��+3Ԥ�����У�
	pCGI->extern_up_lines = 0;
	pCGI->extern_down_lines = 0;
	pCGI->lines = pCGI->CFI.block_high * pCGI->row_num + 2 + pCGI->CFI.separator*(pCGI->row_num - 1) + pCGI->extern_up_lines + pCGI->extern_down_lines + pCGI->draw_frame_with_col_no + pCGI->top_status_line + pCGI->lower_status_line + 4;

	pCGI->extern_left_cols = 0;
	pCGI->extern_right_cols = 0;
	pCGI->cols = pCGI->CFI.block_width * pCGI->col_num + 2 * (2 + pCGI->CFI.separator*(pCGI->col_num - 1)) + pCGI->extern_left_cols + pCGI->extern_right_cols + pCGI->draw_frame_with_row_no * 2 + 1;

	//���ṹ�������0
	memset(pCGI->pad, '\0', sizeof(pCGI->pad));

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������Ϸ���
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
  �� �� ֵ��
  ˵    ��������ɲο�demo��Ч��
***************************************************************************/
int gmw_draw_frame(const CONSOLE_GRAPHICS_INFO *const pCGI)
{
	int i, j;
	setfontsize(pCGI->CFT.font_type, pCGI->CFT.font_size_high, pCGI->CFT.font_size_width);
	setconsoleborder(pCGI->cols, pCGI->lines, pCGI->cols, pCGI->lines);
	setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
	cls();
	setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);

	if (pCGI->draw_frame_with_col_no) {
		setcolor(pCGI->area_bgcolor, pCGI->area_fgcolor);
		for (j = 0; j < pCGI->col_num; j++) {
			//����һЩϸ�����⣨�Լ���...�� δʹ��showstr/showint/showch
			gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no + 2 + (pCGI->CFI.block_width / 2) / 2 * 2 + j * pCGI->CFI.bwidth, pCGI->start_y + pCGI->top_status_line);
			if (j <= 99)
				cout << j;
			else
				cout << "*";
		}
	}
	//��һ��
	gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
	cout << pCGI->CFI.top_left;
	for (j = 1; j < pCGI->col_num; j++) {
		for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
			cout << pCGI->CFI.h_normal;
		if (pCGI->CFI.separator)
			cout << pCGI->CFI.h_top_separator;
	}
	for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
		cout << pCGI->CFI.h_normal;
	cout << pCGI->CFI.top_right;
	Sleep(pCGI->delay_of_draw_frame);
	//�м䲿��
	for (i = 1; i < pCGI->row_num; i++) {
		for (int h = 0; h < pCGI->CFI.block_high; h++) {
			if (pCGI->draw_frame_with_row_no&&h == (pCGI->CFI.block_high - 1) / 2) {
				char ch = i - 1 > 'Z' - 'A' ? '*' : 'A' + i - 1;
				showch(pCGI->start_x, h + pCGI->CFI.bhigh * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, ch, pCGI->area_bgcolor, pCGI->area_fgcolor);
				setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
			}
			gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + pCGI->CFI.bhigh * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.v_normal;
			for (j = 1; j < pCGI->col_num; j++) {
				for (int t = 0; t < pCGI->CFI.block_width; t++)
					cout << " ";
				if (pCGI->CFI.separator)
					cout << pCGI->CFI.v_normal;
			}
			for (int t = 0; t < pCGI->CFI.block_width; t++)
				cout << " ";
			cout << pCGI->CFI.v_normal;
			Sleep(pCGI->delay_of_draw_frame);
		}
		if (pCGI->CFI.separator) {
			gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + (pCGI->CFI.bhigh) * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.v_left_separator;
			for (j = 1; j < pCGI->col_num; j++) {
				for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
					cout << pCGI->CFI.h_normal;
				cout << pCGI->CFI.mid_separator;
			}
			for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
				cout << pCGI->CFI.h_normal;
			cout << pCGI->CFI.v_right_separator;
		}
		Sleep(pCGI->delay_of_draw_frame);
	}
	//�������
	for (int h = 0; h < pCGI->CFI.block_high; h++) {
		if (pCGI->draw_frame_with_row_no&&h == (pCGI->CFI.block_high - 1) / 2) {
			char ch = i - 1 > 'Z' - 'A' ? '*' : 'A' + i - 1;
			showch(pCGI->start_x, pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no + 1 + h + pCGI->CFI.bhigh * (pCGI->row_num - 1), ch, pCGI->area_bgcolor, pCGI->area_fgcolor);
			setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
		}
		gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + (pCGI->CFI.bhigh) * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
		cout << pCGI->CFI.v_normal;
		for (j = 1; j < pCGI->col_num; j++) {
			for (int t = 0; t < pCGI->CFI.block_width; t++)
				cout << " ";
			if (pCGI->CFI.separator)
				cout << pCGI->CFI.v_normal;
		}
		for (int t = 0; t < pCGI->CFI.block_width; t++)
			cout << " ";
		cout << pCGI->CFI.v_normal;
		Sleep(pCGI->delay_of_draw_frame);
	}
	gotoxy(pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + (pCGI->CFI.bhigh) * (i - 1) + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
	cout << pCGI->CFI.lower_left;
	for (j = 1; j < pCGI->col_num; j++) {
		for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
			cout << pCGI->CFI.h_normal;
		if (pCGI->CFI.separator)
			cout << pCGI->CFI.h_lower_separator;
	}
	for (int w = 0; w < pCGI->CFI.block_width / 2; w++)
		cout << pCGI->CFI.h_normal;
	cout << pCGI->CFI.lower_right;
	Sleep(pCGI->delay_of_draw_frame);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���״̬������ʾ��Ϣ
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int type							��ָ������/��״̬��
		   const char *msg						��������Ϣ
		   const char *catchy_msg					����Ҫ�ر��ע����Ϣ����������Ϣǰ��ʾ��
  �� �� ֵ��
  ˵    ����1���������޶�Ϊ����ܵĿ�ȣ����к��б�λ�ã����������ȥ
			2��������һ���ַ���ĳ���ֵ�ǰ������ᵼ�º������룬Ҫ����
***************************************************************************/
int gmw_status_line(const CONSOLE_GRAPHICS_INFO *const pCGI, const int type, const char *msg, const char *catchy_msg)
{
	if (type == TOP_STATUS_LINE && pCGI->top_status_line) {
		showstr(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y, " ", pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.width);
		gotoxy(pCGI->SLI.top_start_x, pCGI->SLI.top_start_y);
		if (catchy_msg) {
			setcolor(pCGI->SLI.top_catchy_bgcolor, pCGI->SLI.top_catchy_fgcolor);
			cout << catchy_msg;
		}
		setcolor(pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor);
		cout << msg;
	}
	else if (type == LOWER_STATUS_LINE && pCGI->lower_status_line) {
		showstr(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y, " ", pCGI->SLI.top_normal_bgcolor, pCGI->SLI.top_normal_fgcolor, pCGI->SLI.width);
		gotoxy(pCGI->SLI.lower_start_x, pCGI->SLI.lower_start_y);
		if (catchy_msg) {
			setcolor(pCGI->SLI.lower_catchy_bgcolor, pCGI->SLI.lower_catchy_fgcolor);
			cout << catchy_msg;
		}
		setcolor(pCGI->SLI.lower_normal_bgcolor, pCGI->SLI.lower_normal_fgcolor);
		cout << msg;
	}
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʾĳһ��ɫ��(����Ϊ�ַ���������Ϊrow/col)
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const BLOCK_DISPLAY_INFO *const bdi		����Ÿ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
  �� �� ֵ��
  ˵    ����1��BLOCK_DISPLAY_INFO �ĺ����ͷ�ļ����÷��ο���������
			2��bdi_valueΪ BDI_VALUE_BLANK ��ʾ�հ׿飬Ҫ���⴦��
***************************************************************************/
int gmw_draw_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const BLOCK_DISPLAY_INFO *const bdi)
{

	const BLOCK_DISPLAY_INFO *p;
	int fg, bg;
	for (p = bdi; p->value != BDI_VALUE_END; p++) {
		if (p->value == bdi_value) {
			fg = p->fgcolor == -1 ? pCGI->CFI.fgcolor : p->fgcolor;
			bg = p->bgcolor == -1 ? pCGI->CFI.bgcolor : p->bgcolor;
			setcolor(bg, fg);
			break;
		}
	}

	if (pCGI->CBI.block_border) {
		//��һ��
		if (bdi_value == BDI_VALUE_BLANK)
			showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, " ", bg, fg, pCGI->CFI.block_width);
		else {
			gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.top_left;
			for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
				cout << pCGI->CFI.h_normal;
			}
			cout << pCGI->CFI.top_right;
		}
		//�м���
		for (int h = 1; h < pCGI->CFI.block_high - 1; h++) {
			if (bdi_value == BDI_VALUE_BLANK)
				showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, " ", bg, fg, pCGI->CFI.block_width);
			else {
				gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, h + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				cout << pCGI->CFI.v_normal;
				for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
					cout << "  ";
				}
				cout << pCGI->CFI.v_normal;
			}
		}
		//���һ��
		if (bdi_value == BDI_VALUE_BLANK)
			showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, " ", bg, fg, pCGI->CFI.block_width);
		else {
			gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
			cout << pCGI->CFI.lower_left;
			for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
				cout << pCGI->CFI.h_normal;
			}
			cout << pCGI->CFI.lower_right;
		}
	}

	//���ֵ
	gotoxy(2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
	if (p->content)
		cout << p->content;
	else if (p->value)
		cout << p->value;
	else
		cout << "  ";

	Sleep(pCGI->delay_of_draw_block);
	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ƶ�ĳһ��ɫ��
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   const int row_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int col_no						���кţ���0��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
		   const int bdi_value						����Ҫ��ʾ��ֵ
		   const int blank_bdi_value				���ƶ����������ڶ���Ч����ʾʱ���ڱ�ʾ�հ׵�ֵ��һ��Ϊ0���˴���Ϊ�������룬�ǿ��ǵ����ܳ��ֵ����������
		   const BLOCK_DISPLAY_INFO *const bdi		�������ʾֵ/�հ�ֵ��Ӧ����ʾ��Ϣ�Ľṹ������
		   const int direction						���ƶ�����һ�����֣������cmd_gmw_tools.h
		   const int distance						���ƶ����루��1��ʼ����Ϊ��֤��ȷ�ԣ����򲻼�飩
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int gmw_move_block(const CONSOLE_GRAPHICS_INFO *const pCGI, const int row_no, const int col_no, const int bdi_value, const int blank_bdi_value, const BLOCK_DISPLAY_INFO *const bdi, const int direction, const int distance)
{
	const BLOCK_DISPLAY_INFO *p;
	int fg, bg;
	for (p = bdi; p->value != BDI_VALUE_END; p++) {
		if (p->value == bdi_value) {
			fg = p->fgcolor == -1 ? pCGI->CFI.fgcolor : p->fgcolor;
			bg = p->bgcolor == -1 ? pCGI->CFI.bgcolor : p->bgcolor;
			break;
		}
	}
	if (p->value == BDI_VALUE_END)
		return -1;

	const BLOCK_DISPLAY_INFO *p_blank;
	int fg_blank, bg_blank;
	for (p_blank = bdi; p_blank->value != BDI_VALUE_END; p_blank++) {
		if (p_blank->value == blank_bdi_value) {
			fg_blank = p_blank->fgcolor == -1 ? pCGI->CFI.fgcolor : p_blank->fgcolor;
			bg_blank = p_blank->bgcolor == -1 ? pCGI->CFI.bgcolor : p_blank->bgcolor;
			break;
		}
	}
	if (p_blank->value == BDI_VALUE_END)
		return -1;

	for (int d = 0; d < distance; d++) {
		//ÿ���ƶ�һ��
		if (direction == DOWN_TO_UP || direction == UP_TO_DOWN) {
			int t = direction * 2 - 1;	//���� t=-1		���� t=1
			for (int h = 1; h <= pCGI->CFI.bhigh; h++) {
				Sleep(pCGI->delay_of_block_moved);
				//Ĩȥֵ
				gotoxy(2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + (h - 1)*t + pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				setcolor(bg_blank, fg_blank);
				if (p_blank->content)
					cout << p_blank->content;
				else if (p_blank->value)
					cout << p_blank->value;
				else
					cout << "  ";

				if (pCGI->CBI.block_border) {
					//��Ĩȥ��Զһ��
					setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + (h - 1)* t + direction + pCGI->CFI.bhigh* (1 - direction) + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					for (int w = 0; w < pCGI->CFI.block_width; w++)
						cout << " ";

					setcolor(bg, fg);
					//��һ��
					gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h*t + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.top_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.top_right;
					//�м���
					for (int bh = 1; bh < pCGI->CFI.block_high - 1; bh++) {
						gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h*t + bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
						cout << pCGI->CFI.v_normal;
						for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
							cout << "  ";
						}
						cout << pCGI->CFI.v_normal;
					}
					//���һ��
					gotoxy(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h*t + pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.lower_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.lower_right;
				}

				//���ֵ
				setcolor(bg, fg);
				gotoxy(2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + h * t + pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				if (p->content)
					cout << p->content;
				else if (p->value)
					cout << p->value;
				else
					cout << "  ";
			}
			//���ϱ����ǵ��ı߿�
			if (pCGI->CFI.separator) {
				showstr(col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, t*d*pCGI->CFI.bhigh + direction * pCGI->CFI.bhigh + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, pCGI->CFI.h_normal, pCGI->CFI.bgcolor, pCGI->CFI.fgcolor, pCGI->CFI.block_width / 2);
			}
		}
		else if (direction == LEFT_TO_RIGHT || direction == RIGHT_TO_LEFT) {
			int t = direction * 2 - 5;	//���� t=-1		���� t=1
			for (int h = 1; h <= pCGI->CFI.bwidth / 2; h++) {
				Sleep(pCGI->delay_of_block_moved);
				//Ĩȥֵ
				gotoxy(t*d*pCGI->CFI.bwidth + (h - 1) * 2 * t + 2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				setcolor(bg_blank, fg_blank);
				if (p_blank->content)
					cout << p_blank->content;
				else if (p_blank->value)
					cout << p_blank->value;
				else
					cout << "  ";

				if (pCGI->CBI.block_border) {
					//��Ĩȥ��Զһ��(2��)
					for (int bh = 0; bh < pCGI->CFI.block_high; bh++) {
						showstr(t * d*pCGI->CFI.bwidth + pCGI->CFI.bwidth* (3 - direction) + 2 * (h - 1) * t + 2 * (direction - 2) + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no, bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no, "  ", pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
					}

					setcolor(bg, fg);
					//��һ��
					gotoxy(t*d*pCGI->CFI.bwidth + 2 * h * t + col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.top_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.top_right;
					//�м���
					for (int bh = 1; bh < pCGI->CFI.block_high - 1; bh++) {
						gotoxy(t*d*pCGI->CFI.bwidth + 2 * h * t + col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
						cout << pCGI->CFI.v_normal;
						for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
							cout << "  ";
						}
						cout << pCGI->CFI.v_normal;
					}
					//���һ��
					gotoxy(t*d*pCGI->CFI.bwidth + 2 * h * t + col_no * pCGI->CFI.bwidth + 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high + row_no * pCGI->CFI.bhigh + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.lower_left;
					for (int w = 0; w < pCGI->CFI.block_width / 2 - 2; w++) {
						cout << pCGI->CFI.h_normal;
					}
					cout << pCGI->CFI.lower_right;
				}

				//���ֵ
				setcolor(bg, fg);
				gotoxy(t*d*pCGI->CFI.bwidth + h * 2 * t + 2 + pCGI->CBI.block_border * 2 + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, pCGI->CFI.block_high / 2 + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
				if (p->content)
					cout << p->content;
				else if (p->value)
					cout << p->value;
				else
					cout << "  ";
			}
			//���ϱ����ǵ��ı߿�
			if (pCGI->CFI.separator) {
				setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
				for (int bh = 0; bh < pCGI->CFI.block_high; bh++) {
					gotoxy(t*d*pCGI->CFI.bwidth + (direction - 2) * pCGI->CFI.bwidth + col_no * pCGI->CFI.bwidth + pCGI->start_x + 2 * pCGI->draw_frame_with_row_no, bh + row_no * pCGI->CFI.bhigh + 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no);
					cout << pCGI->CFI.v_normal;
				}
			}
		}
	}

	return 0; //�˾�ɸ�����Ҫ�޸�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������̻����
  ���������const CONSOLE_GRAPHICS_INFO *const pCGI	������ṹָ��
		   int &MAction							��������� CCT_MOUSE_EVENT�����ֵ��Ч��Ϊ MOUSE_LEFT_BUTTON_CLICK/MOUSE_RIGHT_BUTTON_CLICK ����֮һ
													   ������� CCT_KEYBOARD_EVENT�����ֵ��Ч
		   int &MRow								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &MCol								��������� CCT_MOUSE_EVENT �� MAction = MOUSE_LEFT_BUTTON_CLICK�����ֵ��Ч����ʾ���ѡ�����Ϸ������кţ���0��ʼ��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode1							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ��һ����
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   int &KeyCode2							��������� CCT_KEYBOARD_EVENT�����ֵ��Ч��Ϊ�����ļ��루���˫���룬��Ϊ�ڶ���������ǵ����룬��Ϊ0��
												  ���������ֵ��Ч�����������Чֵ���´��󣬲��Ǳ������Ĵ�!!!��
		   const bool update_lower_status_line		������ƶ�ʱ���Ƿ�Ҫ�ڱ���������ʾ"[��ǰ���] *��*��"����Ϣ��true=��ʾ��false=����ʾ��ȱʡΪtrue��
  �� �� ֵ����������Լ��
			1�����������ƶ��������أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] *��*��"
		   2������ǰ������������ҵ�ǰ���ָ��ͣ��������Ϸ�����*��*���ϣ��򷵻� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow Ϊ�кţ�MCol Ϊ�б�
								  �ҵ�ǰ���ָ��ͣ���ڷǷ����򣨷���Ϸ������Ϸ�����еķָ��ߣ����򲻷��أ����� update_lower_status_line ����������״̬����ʾ"[��ǰ���] λ�÷Ƿ�"
		   3������ǰ�������Ҽ������ж����ָ��ͣ�������Ƿ�Ϸ���ֱ�ӷ��� CCT_MOUSE_EVENT ��MAction Ϊ MOUSE_LEFT_BUTTON_CLICK, MRow��MCol������
		   4��������¼����ϵ�ĳ������˫���밴��������ֱ�ӷ��� CCT_KEYBOARD_EVENT��KeyCode1/KeyCode2��Ϊ��Ӧ�ļ���ֵ
 ˵    ����ͨ������ cmd_console_tools.cpp �е� read_keyboard_and_mouse ����ʵ��
***************************************************************************/
int gmw_read_keyboard_and_mouse(const CONSOLE_GRAPHICS_INFO *const pCGI, int &MAction, int &MRow, int &MCol, int &KeyCode1, int &KeyCode2, const bool update_lower_status_line)
{
	int X = 2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no, Y = 1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no;	//������ʼֵ ��Ӧ0��0��
	int ret;
	char temp[256];
	enable_mouse();
	setcolor(pCGI->CFI.bgcolor, pCGI->CFI.fgcolor);
	while (1) {
		/* �����/���̣�����ֵΪ���������е�ĳһ��, ��ǰ���λ����<X,Y>�� */
		ret = read_keyboard_and_mouse(X, Y, MAction, KeyCode1, KeyCode2);
		if (ret == CCT_MOUSE_EVENT) {
			MRow = (Y - (1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no)) / pCGI->CFI.bhigh;
			MCol = (X - (2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no)) / pCGI->CFI.bwidth;
			if (X - (2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no) < 0 || Y - (1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no) < 0
				|| (Y - (1 + pCGI->start_y + pCGI->top_status_line + pCGI->draw_frame_with_col_no)) % pCGI->CFI.bhigh >= pCGI->CFI.block_high
				|| (X - (2 + pCGI->start_x + 2 * pCGI->draw_frame_with_col_no)) % pCGI->CFI.bwidth >= pCGI->CFI.block_width	//�ڱ߿���
				|| MRow < 0 || MRow >= pCGI->row_num || MCol < 0 || MCol >= pCGI->col_num) {	//������Χ
				sprintf(temp, "[��ǰ���] λ�÷Ƿ�");
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				continue;
			}
			else {
				sprintf(temp, "[��ǰ���] %c�� %d�� ", 'A' + MRow, MCol);
				gmw_status_line(pCGI, LOWER_STATUS_LINE, temp);
				switch (MAction) {
					case MOUSE_LEFT_BUTTON_CLICK:			//�������
						return CCT_MOUSE_EVENT;
					case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
						return CCT_MOUSE_EVENT;
					default:
						break;
				}
			}
		}
		else if (ret == CCT_KEYBOARD_EVENT) {
			return CCT_KEYBOARD_EVENT;
		}
	}
	disable_mouse();	//�������

	return 0; //�˾�ɸ�����Ҫ�޸�
}
