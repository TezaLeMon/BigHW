// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

/* 允许添加需要的头文件 */
#include <math.h>
#include "cmd_hdc_tools.h"
using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow();

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */

/* 给出向量 计算叉乘（大小）	*/
static int cross_product(int x1_i, int y1_i, int x2_i, int y2_i) {
	return x1_i * y2_i - x2_i * y1_i;
}

/*	求三个数的最大值	*/
static int max3(int a, int b, int c) {
	int nmax = a;
	if (nmax < b)
		nmax = b;
	if (nmax < c)
		nmax = c;
	return nmax;
}

/*	求三个数的最小值*/
static int min3(int a, int b, int c) {
	int nmin = a;
	if (nmin > b)
		nmin = b;
	if (nmin > c)
		nmin = c;
	return nmin;
}

/* 坐标合法（包括不完全超出边界）返回1 不合法（完全超出边界）返回0 */
static int recorect_XY_line(int *x1, int *y1, int *x2, int *y2, const double k) {

	if (*x1 >= 0 && *x1 <= _Width_ && *x2 >= 0 && *x2 <= _Width_	//两点都在范围内 
		&& *y1 >= 0 && *y1 <= _High && *y2 >= 0 && *y2 <= _High)
		return 1;
	else if (k == 0) {	//直线平行于x轴
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
	else if (k == INT_MAX) {	//直线平行于y轴
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
		if (*x1 >= 0 && *x1 <= _Width_ && *y1 >= 0 && *y1 <= _High) {	//第一点在区域内
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
		else if (*x2 >= 0 && *x2 <= _Width_ && *y2 >= 0 && *y2 <= _High) {	//第一点不在 第二点在
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
		else {	//两点都不在区域内
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
  函数名称：
  功    能：转化角度
  输入参数：const int angles：待转化角度
			const int begin_angles：参考轴起始角度（ 0为正右方向 ）
			const int direction_rotation  ：旋转方向（1为顺时针 -1为逆时针）
  返 回 值：系统角度（ 0 - 360 ）
  说    明：
***************************************************************************/
static int recorect_angles(int angles, int begin_angles = 0, int direction_rotation = 1)
{
	angles %= 360;
	begin_angles %= 360;
	if (direction_rotation != 1 && direction_rotation != -1)	//默认顺时针
		direction_rotation = 1;
	int real_angle;
	real_angle = direction_rotation * (angles + begin_angles);
	real_angle = (real_angle + 360) % 360;
	return real_angle;
}

/***************************************************************************
  函数名称：
  功    能：转化旋转坐标
  输入参数：const int angles：待转化角度
			const int begin_angles：参考轴起始角度（ 0为正右方向 ）
			const int direction_rotation  ：旋转方向（1为顺时针 -1为逆时针）
  返 回 值：系统角度（ 0 - 360 ）
  说    明：
***************************************************************************/
void rotate_XY(int *x, int *y, const int center_x, const int center_y, const int angles)
{
	int tx = *x - center_x, ty = *y - center_y;
	double theta = angles * PI / 180;
	int _x = int(tx * cos(theta) - ty * sin(theta)), _y = int(tx * sin(theta) + ty * cos(theta));
	*x = _x + center_x;
	*y = _y + center_y;
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
			const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	setcursor(CURSOR_INVISIBLE);
	setcolor(bgcolor, fgcolor);
	setfontsize("点阵字体", 16);
	setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16), 120, 999); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
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
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
			const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
			const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_point函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */
   /***************************************************************************
	 函数名称：
	 功    能：画线段
	 输入参数：const int x1		：起点的x
			   const int y1		：起点的y
			   const int x2		：终点的x
			   const int y2		：终点的y
			   const int thickness	：线段的粗细（有缺省值）
			   const int RGB_value	：线段的颜色（有缺省值）
	 返 回 值：
	 说    明：
   ***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限

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
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
			const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限

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
			for (j = y_min - 1; j <= y_max + 1; j++) {	//判断各点是否在园内
				t1 = cross_product(x1 - i, y1 - j, x2 - i, y2 - j);
				t2 = cross_product(x2 - i, y2 - j, x3 - i, y3 - j);
				t3 = cross_product(x3 - i, y3 - j, x1 - i, y1 - j);
				if (flag_from && ((t1 >= 0 && t2 >= 0 && t3 >= 0) || (t1 <= 0 && t2 <= 0 && t3 <= 0))) {	//记录开始在三角形内的坐标
					from = j;
					flag_from = 0;
				}
				if (flag_to && !flag_from && !((t1 >= 0 && t2 >= 0 && t3 >= 0) || (t1 <= 0 && t2 <= 0 && t3 <= 0))) {	//记录最后在三角形内的坐标
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
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：算法略微有点问题 某些特定半径可能会导致有极少数错画现象(对小数的精度导致)
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限

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
			//hdc_point(x1, y1, tn, RGB_value);		效率较低 存在大量重复画的点
		}
	} while (abs(angle - real_angle_end) > 0);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
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
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
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
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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