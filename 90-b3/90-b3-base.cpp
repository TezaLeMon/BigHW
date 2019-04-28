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

	return 1;
}