// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <conio.h>  
#include <math.h>
#include <time.h>
#include "../common/cmd_hdc_tools.h"
using namespace std;

const int win_bgcolor = COLOR_WHITE, win_fgcolor = COLOR_HRED;

const int EDGE_COLOR = RGB(0, 8, 0);

const int BODY_COLOR = RGB(179, 146, 119);
const int BODY_LIGHT1_COLOR = RGB(189, 162, 141);
const int BODY_LIGHT2_COLOR = RGB(205, 185, 165);

const int NOSE_COLOR = RGB(189, 131, 170);
const int NOSE_LIGHT1_COLOR = RGB(243, 203, 239);
const int NOSE_LIGHT2_COLOR = RGB(252, 252, 252);

const int EYE_COLOR = RGB(50, 42, 41);
const int EYE_LIGHT_COLOR = NOSE_LIGHT2_COLOR;

const int ROOT1_COLOR = RGB(124, 124, 122);
const int ROOT2_COLOR = RGB(203, 195, 197);
const int ROOT3_COLOR = RGB(167, 163, 164);

const int base_x = 310;		//鼻子为中心
const int base_y = 300;

const double magnification = 0.5;

void root1() {
	hdc_ellipse(base_x + int(85 * magnification), base_y + int(320 * magnification),	//基部底座主体
		int(380 * magnification), int(125 * magnification), 0, 1, 2, ROOT1_COLOR);
	hdc_ellipse(base_x + int(85 * magnification), base_y + int(320 * magnification),	//基部底座边线
		int(383 * magnification), int(128 * magnification), 0, 0, 3, EDGE_COLOR);
}
void root2() {
	hdc_ellipse(base_x - int(120 * magnification), base_y + int(320 * magnification),	//基部石头1
		int(70 * magnification), int(40 * magnification), 330, 1, 2, ROOT2_COLOR);
	hdc_ellipse(base_x - int(120 * magnification), base_y + int(320 * magnification),	//基部石头1边线
		int(72 * magnification), int(42 * magnification), 330, 0, 3, EDGE_COLOR);

	hdc_ellipse(base_x, base_y + int(320 * magnification),	//基部石头2
		int(90 * magnification), int(50 * magnification), 30, 1, 2, ROOT3_COLOR);
	hdc_ellipse(base_x, base_y + int(320 * magnification),	//基部石头2边线
		int(92 * magnification), int(52 * magnification), 30, 0, 3, EDGE_COLOR);

	hdc_ellipse(base_x + int(150 * magnification), base_y + int(320 * magnification),	//基部石头3
		int(90 * magnification), int(50 * magnification), 345, 1, 2, ROOT2_COLOR);
	hdc_ellipse(base_x + int(150 * magnification), base_y + int(320 * magnification),	//基部石头3边线
		int(92 * magnification), int(52 * magnification), 345, 0, 3, EDGE_COLOR);

	hdc_ellipse(base_x + int(270 * magnification), base_y + int(320 * magnification),	//基部石头4
		int(90 * magnification), int(50 * magnification), 10, 1, 2, ROOT3_COLOR);
	hdc_ellipse(base_x + int(270 * magnification), base_y + int(320 * magnification),	//基部石头4边线
		int(92 * magnification), int(52 * magnification), 10, 0, 3, EDGE_COLOR);
}

void body() {
	hdc_rectangle(base_x - int(150 * magnification), base_y - int(50 * magnification),	//身体下半部分
		int(470 * magnification) - 2, int(370 * magnification), 0, 1, 2, BODY_COLOR);
	hdc_sector(base_x + int(85 * magnification), base_y - int(50 * magnification),	//身体上半部分-头部
		int(235 * magnification), -90, 90, 1, 2, BODY_COLOR);
	hdc_ellipse(base_x + int(15 * magnification), base_y - int(215 * magnification),	//头部光影
		int(140 * magnification), int(53 * magnification), 338, 1, 2, BODY_LIGHT1_COLOR);
	hdc_ellipse(base_x + int(15 * magnification), base_y - int(215 * magnification),	//头部光影
		int(120 * magnification), int(40 * magnification), 338, 1, 2, BODY_LIGHT2_COLOR);

	hdc_line(base_x - int(150 * magnification), base_y - int(50 * magnification),		//身体下半部边线
		base_x - int(150 * magnification), base_y + int(320 * magnification), 5, EDGE_COLOR);
	hdc_line(base_x + int(320 * magnification) - 2, base_y - int(50 * magnification),
		base_x + int(320 * magnification) - 2, base_y + int(320 * magnification), 5, EDGE_COLOR);
	hdc_arc(base_x + int(85 * magnification), base_y - int(50 * magnification),	//身体上半部分-头部
		int(235 * magnification), -90, 90, 5, EDGE_COLOR);
}

void eye() {
	hdc_ellipse(base_x - int(50 * magnification), base_y - int(110 * magnification),	//左眼主体
		int(45 * magnification), int(15 * magnification), 95, 1, 2, EYE_COLOR);
	hdc_ellipse(base_x + int(80 * magnification), base_y - int(110 * magnification),	//右眼主体
		int(45 * magnification), int(15 * magnification), 92, 1, 2, EYE_COLOR);

	hdc_ellipse(base_x - int(50 * magnification), base_y - int(140 * magnification),	//左眼光影
		int(10 * magnification), int(6 * magnification), 95, 1, 2, EYE_LIGHT_COLOR);
	hdc_ellipse(base_x + int(80 * magnification), base_y - int(140 * magnification),	//右眼光影
		int(10 * magnification), int(6 * magnification), 92, 1, 2, EYE_LIGHT_COLOR);
}

void nose() {
	hdc_ellipse(base_x, base_y, int(90 * magnification), int(50 * magnification), 0, 1, 2, NOSE_COLOR);	//鼻子主体
	hdc_ellipse(base_x - int(15 * magnification), base_y - int(15 * magnification),	//鼻子光影1
		int(60 * magnification), int(30 * magnification), 0, 1, 2, NOSE_LIGHT1_COLOR);
	hdc_ellipse(base_x - int(45 * magnification), base_y - int(25 * magnification),	//鼻子光影2
		int(18 * magnification), int(10 * magnification), 0, 1, 2, NOSE_LIGHT2_COLOR);
	hdc_ellipse(base_x, base_y, int(93 * magnification), int(53 * magnification), 0, 0, 3, EDGE_COLOR);	//鼻子主体
}

int hdc_draw_Diglett()
{
	//不加延时可能会有显示错误...
	Sleep(100);
	root1();
	Sleep(100);
	body();
	Sleep(100);
	eye();
	Sleep(100);
	nose();
	Sleep(100);
	root2();
	return 0;
}