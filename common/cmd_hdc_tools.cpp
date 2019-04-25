// 1752762 ��1�� κ�ٴ�
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

/* ���������Ҫ��ͷ�ļ� */
#include <math.h>
#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */

/* �������� �����ˣ���С��	*/
static int cross_product(int x1_i, int y1_i, int x2_i, int y2_i) {
	return x1_i * y2_i - x2_i * y1_i;
}

/*	�������������ֵ	*/
static int max3(int a, int b, int c) {
	int nmax = a;
	if (nmax < b)
		nmax = b;
	if (nmax < c)
		nmax = c;
	return nmax;
}

/*	������������Сֵ*/
static int min3(int a, int b, int c) {
	int nmin = a;
	if (nmin > b)
		nmin = b;
	if (nmin > c)
		nmin = c;
	return nmin;
}

/* ����Ϸ�����������ȫ�����߽磩����1 ���Ϸ�����ȫ�����߽磩����0 */
static int recorect_XY_line(int *x1, int *y1, int *x2, int *y2, const double k) {

	if (*x1 >= 0 && *x1 <= _Width_ && *x2 >= 0 && *x2 <= _Width_	//���㶼�ڷ�Χ�� 
		&& *y1 >= 0 && *y1 <= _High && *y2 >= 0 && *y2 <= _High)
		return 1;
	else if (k == 0) {	//ֱ��ƽ����x��
		if (*y1 > _High || *y1 < 0)
			return 0;
		if (*x1 < 0)
			*x1 = 0;
		else if (*x1 > _Width_)
			*x1 = _Width_;
		if (*x2 < 0)
			*x2 = 0;
		else if (*x2 > _Width_)
			*x2 = _Width_;
	}
	else if (k == INT_MAX) {	//ֱ��ƽ����y��
		if (*x1<0 || *x1>_Width_)
			return 0;
		if (*y1 < 0)
			*y1 = 0;
		else if (*y1 > _Width_)
			*y1 = _Width_;
		if (*y2 < 0)
			*y2 = 0;
		else if (*y2 > _Width_)
			*y2 = _Width_;
	}
	else {
		// ty-y2=k(tx-x2)
		if (*x1 >= 0 && *x1 <= _Width_ && *y1 >= 0 && *y1 <= _High) {	//��һ����������
			if (*x2 < 0) {
				int ty = int(*y2 - *x2*k);
				if (ty >= 0 && ty <= _High) {
					*x2 = 0;
					*y2 = ty;
					return 1;
				}
			}
			else if (*x2 > _Width_) {
				int ty = int(*y2 + k * (_Width_ - *x2));
				if (ty >= 0 && ty <= _High) {
					*x2 = _Width_;
					*y2 = ty;
					return 1;
				}
			}

			if (*y2 < 0) {
				int tx = int(-*y2 / k + *x2);
				if (tx >= 0 && tx <= _High) {
					*x2 = tx;
					*y2 = 0;
					return 1;
				}
			}
			else if (*y2 > _High) {
				int tx = int((_High - *y2) / k + *x2);
				if (tx >= 0 && tx <= _High) {
					*x2 = tx;
					*y2 = _High;
					return 1;
				}
			}
		}
		else if (*x2 >= 0 && *x2 <= _Width_ && *y2 >= 0 && *y2 <= _High) {	//��һ�㲻�� �ڶ�����
			if (*x1 < 0) {
				int ty = int(*y1 - *x1*k);
				if (ty >= 0 && ty <= _High) {
					*x1 = 0;
					*y1 = ty;
					return 1;
				}
			}
			else if (*x1 > _Width_) {
				int ty = int(*y1 + k * (_Width_ - *x1));
				if (ty >= 0 && ty <= _High) {
					*x1 = _Width_;
					*y1 = ty;
					return 1;
				}
			}

			if (*y1 < 0) {
				int tx = int(-*y1 / k + *x1);
				if (tx >= 0 && tx <= _High) {
					*x1 = tx;
					*y1 = 0;
					return 1;
				}
			}
			else if (*y1 > _High) {
				int tx = int((_High - *y1) / k + *x1);
				if (tx >= 0 && tx <= _High) {
					*x1 = tx;
					*y1 = _High;
					return 1;
				}
			}
		}
		else {	//���㶼����������
			if (*x1 < 0) {
				if (*x2 < 0)
					return 0;
				int ty = int((*y1 + -*x1*k));
				if (ty >= 0 && ty <= _High) {
					*x1 = 0;
					*y1 = ty;
				}
				else
					return 0;
			}
			else if (*x1 > _Width_) {
				if (*x2 > _Width_)
					return 0;
				int ty = int(*y1 + k * (_Width_ - *x1));
				if (ty >= 0 && ty <= _High) {
					*x1 = 0;
					*y1 = ty;
				}
				else
					return 0;
			}
			if (*y1 < 0) {
				if (*y2 < 0)
					return 0;
				int tx = int(-*y1 / k + *x1);
				if (tx >= 0 && tx <= _High) {
					*x1 = tx;
					*y1 = 0;
				}
				else
					return 0;
			}
			else if (*y1 > _High) {
				if (*y2 > _High)
					return 0;
				int tx = int((_High - *y1) / k + *x1);
				if (tx >= 0 && tx <= _High) {
					*x1 = tx;
					*y1 = _High;
				}
				else
					return 0;
			}

			if (*x2 < 0) {
				int ty = int((*y1 + -*x1*k));
				if (ty >= 0 && ty <= _High) {
					*x2 = 0;
					*y2 = ty;
				}
				else
					return 0;
			}
			else if (*x2 > _Width_) {
				int ty = int(*y2 + k * (_Width_ - *x2));
				if (ty >= 0 && ty <= _High) {
					*x2 = 0;
					*y2 = ty;
				}
				else
					return 0;
			}
			if (*y2 < 0) {
				int tx = int(-*y2 / k + *x2);
				if (tx >= 0 && tx <= _High) {
					*x2 = tx;
					*y2 = 0;
				}
				else
					return 0;
			}
			else if (*y2 > _High) {
				int tx = int((_High - *y2) / k + *x2);
				if (tx >= 0 && tx <= _High) {
					*x2 = tx;
					*y2 = _High;
				}
				else
					return 0;
			}
		}
	}
	return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ת���Ƕ�
  ���������const int angles����ת���Ƕ�
			const int begin_angles���ο�����ʼ�Ƕȣ� 0Ϊ���ҷ��� ��
			const int direction_rotation  ����ת����1Ϊ˳ʱ�� -1Ϊ��ʱ�룩
  �� �� ֵ��ϵͳ�Ƕȣ� 0 - 360 ��
  ˵    ����
***************************************************************************/
static int recorect_angles(int angles, int begin_angles = 0, int direction_rotation = 1)
{
	angles %= 360;
	begin_angles %= 360;
	if (direction_rotation != 1 && direction_rotation != -1)	//Ĭ��˳ʱ��
		direction_rotation = 1;
	int real_angle;
	real_angle = direction_rotation * (angles + begin_angles);
	real_angle = (real_angle + 360) % 360;
	return real_angle;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ת����ת����
  ���������const int angles����ת���Ƕ�
			const int begin_angles���ο�����ʼ�Ƕȣ� 0Ϊ���ҷ��� ��
			const int direction_rotation  ����ת����1Ϊ˳ʱ�� -1Ϊ��ʱ�룩
  �� �� ֵ��ϵͳ�Ƕȣ� 0 - 360 ��
  ˵    ����
***************************************************************************/
void rotate_XY(int *x, int *y, const int center_x, const int center_y, const int angles)
{
	int tx = *x - center_x, ty = *y - center_y;
	double theta = angles * PI / 180;
	int _x = int(tx * cos(theta) - ty * sin(theta)), _y = int(tx * sin(theta) + ty * cos(theta));
	*x = _x + center_x;
	*y = _y + center_y;
}

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
			const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	setcursor(CURSOR_INVISIBLE);
	setcolor(bgcolor, fgcolor);
	setfontsize("��������", 16);
	setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16), 120, 999); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
	/* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
			const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
			const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_point���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */
   /***************************************************************************
	 �������ƣ�
	 ��    �ܣ����߶�
	 ���������const int x1		������x
			   const int y1		������y
			   const int x2		���յ��x
			   const int y2		���յ��y
			   const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			   const int RGB_value	���߶ε���ɫ����ȱʡֵ��
	 �� �� ֵ��
	 ˵    ����
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������

	int level;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	int t_x = 0, t_y = 0;

	double k = x2 - x1 == 0 ? INT_MAX : double(y2 - y1) / (x2 - x1);
	if (fabs(k) - 1 >= 1e-6)
		t_x = 1;
	else
		t_y = 1;

	int tx1, ty1, tx2, ty2;
	for (level = 0; level <= tn; level++) {
		int t = level % 2 ? 1 : -1;
		t *= level / 2;
		tx1 = x1 + t * t_x;
		ty1 = y1 + t * t_y;
		tx2 = x2 + t * t_x;
		ty2 = y2 + t * t_y;
		if (recorect_XY_line(&tx1, &ty1, &tx2, &ty2, k))
			hdc_base_line(tx1, ty1, tx2, ty2);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
			const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������

	hdc_line(x1, y1, x2, y2, tn, RGB_value);
	hdc_line(x2, y2, x3, y3, tn, RGB_value);
	hdc_line(x3, y3, x1, y1, tn, RGB_value);
	if (filled) {
		int x_min = min3(x1, x2, x3), y_min = min3(y1, y2, y3), x_max = max3(x1, x2, x3), y_max = max3(y1, y2, y3);
		int i, j;
		int t1, t2, t3;
		int flag_from, flag_to, from, to;
		for (i = x_min; i <= x_max; i++) {
			flag_from = 1, flag_to = 1;
			from = y_min, to = y_max;
			for (j = y_min - 1; j <= y_max + 1; j++) {	//�жϸ����Ƿ���԰��
				t1 = cross_product(x1 - i, y1 - j, x2 - i, y2 - j);
				t2 = cross_product(x2 - i, y2 - j, x3 - i, y3 - j);
				t3 = cross_product(x3 - i, y3 - j, x1 - i, y1 - j);
				if (flag_from && ((t1 >= 0 && t2 >= 0 && t3 >= 0) || (t1 <= 0 && t2 <= 0 && t3 <= 0))) {	//��¼��ʼ���������ڵ�����
					from = j;
					flag_from = 0;
				}
				if (flag_to && !flag_from && !((t1 >= 0 && t2 >= 0 && t3 >= 0) || (t1 <= 0 && t2 <= 0 && t3 <= 0))) {	//��¼������������ڵ�����
					to = j;
					flag_to = 0;
				}
				if (!(flag_from || flag_to)) {
					hdc_line(i, from, i, to, 1, RGB_value);
					break;
				}
			}
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	double theta = rotation_angles * PI / 180;
	int right_up_x, right_up_y, left_down_x, left_down_y, right_down_x, right_down_y;
	right_up_x = (int)(left_up_x + width * cos(theta));
	right_up_y = (int)(left_up_y + width * sin(theta));
	left_down_x = (int)(left_up_x - high * sin(theta));
	left_down_y = (int)(left_up_y + high * cos(theta));
	right_down_x = left_down_x + right_up_x - left_up_x;
	right_down_y = left_down_y + right_up_y - left_up_y;

	if (filled) {
		hdc_triangle(left_up_x, left_up_y, left_down_x, left_down_y, right_up_x, right_up_y, 1, thickness, RGB_value);
		hdc_triangle(right_down_x, right_down_y, left_down_x, left_down_y, right_up_x, right_up_y, 1, thickness + 1, RGB_value);
	}
	else {
		hdc_line(left_up_x, left_up_y, right_up_x, right_up_y, thickness, RGB_value);
		hdc_line(right_up_x, right_up_y, right_down_x, right_down_y, thickness, RGB_value);
		hdc_line(right_down_x, right_down_y, left_down_x, left_down_y, thickness, RGB_value);
		hdc_line(left_down_x, left_down_y, left_up_x, left_up_y, thickness, RGB_value);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����㷨��΢�е����� ĳЩ�ض��뾶���ܻᵼ���м�����������(��С���ľ��ȵ���)
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������

	int real_angle_begin = recorect_angles(angle_begin, -90),
		real_angle_end = recorect_angles(angle_end, -90);
	int angle = real_angle_begin;

	int old_x1, old_y1, x1, y1, old_x2, old_y2;

	angle = real_angle_begin;
	old_x1 = point_x + (int)(radius * cos(angle * PI / 180));
	old_y1 = point_y + (int)(radius * sin(angle * PI / 180));
	angle++;
	old_x2 = point_x + (int)(radius * cos(angle * PI / 180));
	old_y2 = point_y + (int)(radius * sin(angle * PI / 180));
	do {
		angle++;
		angle %= 360;
		x1 = point_x + (int)(radius * cos(angle * PI / 180));
		y1 = point_y + (int)(radius * sin(angle * PI / 180));

		if (x1 != old_x1 || y1 != old_y1) {
			hdc_line(old_x1, old_y1, x1, y1, tn, RGB_value);
			old_x1 = old_x2;
			old_y1 = old_y2;
			old_x2 = x1;
			old_y2 = y1;
			//hdc_point(x1, y1, tn, RGB_value);		Ч�ʽϵ� ���ڴ����ظ����ĵ�
		}
	} while (abs(angle - real_angle_end) > 0);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	int real_angle_begin = recorect_angles(angle_begin, -90),
		real_angle_end = recorect_angles(angle_end, -90);
	if ((angle_begin - angle_end) % 360) {
		hdc_line(point_x, point_y, point_x + (int)(radius * cos(real_angle_begin * PI / 180)),
			point_y + (int)(radius * sin(real_angle_begin * PI / 180)), thickness, RGB_value);
		hdc_line(point_x, point_y, point_x + (int)(radius * cos(real_angle_end * PI / 180)),
			point_y + (int)(radius * sin(real_angle_end * PI / 180)), thickness, RGB_value);
	}
	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);

	if (filled) {
		double angle;
		int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;

		angle = real_angle_begin;
		do {
			x1 = point_x + (int)((radius + 1) * cos(angle * PI / 180));
			y1 = point_y + (int)((radius + 1) * sin(angle * PI / 180));

			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_line(point_x, point_y, x1, y1, 3, RGB_value);
			}

			angle += 0.125;
			if (fabs(angle - 360) <= 1e-6) {
				if (fabs(angle - real_angle_end) < 1e-6)
					return;
				angle -= 360;
			}
		} while (fabs(angle - real_angle_end) > 1e-6);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	//hdc_sector(point_x, point_y, radius, 0, 360, filled, thickness, RGB_value);

	hdc_arc(point_x, point_y, radius, 0, 0, thickness, RGB_value);
	if (filled) {
		for (int i = point_x - radius + 1; i < point_x + radius; i++) {
			//(x-px)^2 + (y-py)^2 = radius^2
			int t = int(pow((radius + i - point_x)*(radius - i + point_x), 0.5));
			hdc_line(i, point_y + t, i, point_y - t, 1, RGB_value);
		}
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int t1 = 0, t2;
	int tx1, ty1, tx2, ty2;
	if (filled) {
		for (int i = point_y - radius_b; i <= point_y + radius_b; i++) {
			//(x-px)^2/a^2 + (y-py)^2/b^2 = 1
			t2 = int(pow(1 - double(i - point_y)*(i - point_y) / (radius_b*radius_b), 0.5) * radius_a);
			tx1 = point_x + t2;
			ty1 = i;
			tx2 = point_x - t2;
			ty2 = i;
			rotate_XY(&tx1, &ty1, point_x, point_y, rotation_angles);
			rotate_XY(&tx2, &ty2, point_x, point_y, rotation_angles);
			hdc_line(tx1, ty1, tx2, ty2, 4, RGB_value);
			t1 = t2;
		}
	}
	else {
		int old_x1 = point_x, old_y1 = point_y - radius_b;
		rotate_XY(&old_x1, &old_y1, point_x, point_y, rotation_angles);
		int old_x2 = old_x1, old_y2 = old_y1;
		int tx3, ty3, tx4, ty4;
		t2 = int(pow(1 - double(1 - radius_b)*(1 - radius_b) / (radius_b*radius_b), 0.5) * radius_a);
		tx1 = point_x + t2;
		ty1 = point_y - radius_b + 1;
		tx3 = point_x - t2;
		ty3 = point_y - radius_b + 1;
		rotate_XY(&tx1, &ty1, point_x, point_y, rotation_angles);
		rotate_XY(&tx3, &ty3, point_x, point_y, rotation_angles);
		for (int i = point_y - radius_b + 2; i <= point_y + radius_b; i++) {
			//(x-px)^2/a^2 + (y-py)^2/b^2 = 1
			t2 = int(pow(1 - double(i - point_y)*(i - point_y) / (radius_b*radius_b), 0.5) * radius_a);

			tx2 = point_x + t2;
			ty2 = i;
			rotate_XY(&tx2, &ty2, point_x, point_y, rotation_angles);
			hdc_line(old_x1, old_y1, tx2, ty2, thickness, RGB_value);
			old_y1 = ty1;
			old_x1 = tx1;
			tx1 = tx2;
			ty1 = ty2;

			tx4 = point_x - t2;
			ty4 = i;
			rotate_XY(&tx4, &ty4, point_x, point_y, rotation_angles);
			hdc_line(old_x2, old_y2, tx4, ty4, thickness, RGB_value);
			old_y2 = ty3;
			old_x2 = tx3;
			tx3 = tx4;
			ty3 = ty4;
		}
	}
}