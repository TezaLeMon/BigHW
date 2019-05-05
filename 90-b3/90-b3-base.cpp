// 1752762 计1班 魏鹳达
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <io.h>
#include <string.h>
#include "90-b3.h"
#include "../common/cmd_console_tools.h"
using namespace std;

int read_book_list(char ***list, const char *dir) {
	struct _finddata_t file;
	int k, n_book = 0;
	long HANDLE;
	char *dir_txt;
	dir_txt = new char[strlen(dir) + strlen("./*.txt") + 1];
	strcpy(dir_txt, dir);
	strcat(dir_txt, "./*.txt");

	k = HANDLE = _findfirst(dir_txt, &file);
	if (k == -1) {
		cout << "在" << dir << "下无法找到任何文件" << endl;
		to_be_continued(NULL);
		return 0;
	}
	while (k != -1)
	{
		if (!(n_book % 10))
			*list = (char**)realloc(*list, (n_book + 10) * sizeof(char*));
		(*list)[n_book] = new char[strlen(file.name) + 1];
		strcpy((*list)[n_book], file.name);
		n_book++;
		k = _findnext(HANDLE, &file);
	}
	if (!(n_book % 10))
		*list = (char**)realloc(*list, (n_book + 1) * sizeof(char*));
	(*list)[n_book] = NULL;		//以NULL表示书单末尾
	_findclose(HANDLE);
	delete[] dir_txt;

	return 1;
}

//返回文件中记录的位置 若文件没有记录返回-2
int read_pos(char*** save_inf,const char *book_txt, const char* dir) {
	char* save_dir = new char[strlen(dir) + strlen("/position.saving") + 1];
	if (save_dir == NULL) {
		cout << "空间不足" << endl;
		return -1;
	}
	strcpy(save_dir, dir);
	strcat(save_dir, "/position.saving");
	ifstream fin_save_inf;
	fin_save_inf.open(save_dir, ios::in);
	if (!fin_save_inf.is_open()) {	//没有此文件
		delete[] save_dir;
		return -2;
	}
	
	char temp[128];
	int n = 0, flag_find = 0, pos = -2;
	while (1) {
		fin_save_inf.getline(temp, 127, '\n');
		if (fin_save_inf.eof())
			break;
		if (fin_save_inf.fail()) {	//有某行内容超出长度限制
			cout << save_dir << "某行长度溢出" << endl;
			for (int i = 0; i < n; i++)
				delete[](*save_inf)[i];
			delete[] *save_inf;
			delete[] save_dir;
			return -1;
		}
		if (n % 20 == 0)
			*save_inf = (char**)realloc(*save_inf, (n + 20) * sizeof(char*));
		(*save_inf)[n] = new char[strlen(temp) + 1];
		strcpy((*save_inf)[n], temp);
		n++;
		if (flag_find) {
			pos = atoi(temp);
			flag_find = 0;
		}
		if (strstr(temp, book_txt))
			flag_find = 1;
	}
	if (n % 20 == 0)
		*save_inf = (char**)realloc(*save_inf, (n + 1) * sizeof(char*));
	(*save_inf)[n] = NULL;

	delete[] save_dir;
	fin_save_inf.close();
	return pos;
}

int save_pos(char*** save_inf, const char *book_txt, const char* dir, const streamoff pos, const int is_new)
{

	char* save_dir = new char[strlen(dir) + strlen("/position.saving") + 1];
	if (save_dir == NULL) {
		cout << "空间不足" << endl;
		return -1;
	}
	strcpy(save_dir, dir);
	strcat(save_dir, "/position.saving");
	ofstream fout_save_inf;
	fout_save_inf.open(save_dir, ios::out);
	if (!fout_save_inf.is_open()) {
		cout << save_dir << "打开失败" << endl;
		delete[] save_dir;
		return -1;
	}

	int i, flag = 0;
	for (i = 0; (*save_inf)[i]; delete[](*save_inf)[i], i++) {
		if (flag) {
			fout_save_inf << pos << endl;
			flag = 0;
			continue;
		}
		if (strstr((*save_inf)[i], book_txt))
			flag = 1;
		fout_save_inf << (*save_inf)[i] << endl;
	}
	if (is_new) {
		char *temp = new char[strlen(book_txt) + 3];
		strcpy(temp, "[");
		strcat(temp, book_txt);
		strcat(temp, "]");
		fout_save_inf << temp << endl << pos << endl << endl;
		delete[] temp;
	}
	delete[](*save_inf)[i];
	delete[] * save_inf;
	delete[] save_dir;
	fout_save_inf.close();
	return 0;
}