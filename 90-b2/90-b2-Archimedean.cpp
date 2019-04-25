// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "../common/cmd_hdc_tools.h"
using namespace std;

static const double PI = 3.14159;

int hdc_draw_Archimedean(const int point_x, const int point_y, const int a, const int b, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限

	double angle = angle_begin, l;
	int old_x1, old_y1, x1, y1, old_x2, old_y2;

	// x = (α+βθ)cos(θ)
	// y = (α+βθ)sin(θ)
	l = a * PI / 180 + b * PI / 180 * angle * PI / 180;
	old_x1 = point_x + (int)(l * cos(angle * PI / 180));
	old_y1 = point_y + (int)(l * sin(angle * PI / 180));
	angle += 1;
	l = a * PI / 180 + b * PI / 180 * angle * PI / 180;
	old_x2 = point_x + (int)(l * cos(angle * PI / 180));
	old_y2 = point_y + (int)(l * sin(angle * PI / 180));
	do {
		angle += 1;
		l = a * PI / 180 + b * PI / 180 * angle * PI / 180;
		x1 = point_x + (int)(l * cos(angle * PI / 180));
		y1 = point_y + (int)(l * sin(angle * PI / 180));

		if (x1 != old_x1 || y1 != old_y1) {
			hdc_line(old_x1, old_y1, x1, y1, tn, RGB_value);
			old_x1 = old_x2;
			old_y1 = old_y2;
			old_x2 = x1;
			old_y2 = y1;
			//hdc_point(x1, y1, tn, RGB_value);		效率较低 存在大量重复画的点
		}
	} while (fabs(angle - angle_end) > 1e-6);

	return 0;
}