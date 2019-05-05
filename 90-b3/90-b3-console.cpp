// 1752762 ��1�� κ�ٴ�
#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include "90-b3.h"
#include "../common/cmd_console_tools.h"
#include "../common/common_graphics.h"
using namespace std;

void disp_booklist(char ***book_list_first, const int x_position, const int y_position, const int row, const int col)
{
	for (int i = 0; i < row && (*book_list_first)[i]; i++) {
		int len = strlen((*book_list_first)[i]) - 3;	// + 1 - strlen(".txt")
		char *temp = new char[len];
		strncpy(temp, (*book_list_first)[i], len - 1);
		temp[len - 1] = '\0';
		showstr(x_position + 2, y_position + 2 + i, temp, COLOR_BLACK, COLOR_WHITE, 1, 2 * col);
		cout.clear();
		cout << endl;
		delete[] temp;
	}
}

void move_cur_selecting_book(const char *book_now, const char *book_pre, const int row_now, const int row_pre, const int col, const int x_position, const int y_position)
{
	int len = strlen(book_pre) - 3;	// + 1 - strlen(".txt")
	char *temp = new char[len];
	strncpy(temp, book_pre, len - 1);
	temp[len - 1] = '\0';
	showstr(x_position + 2, y_position + 2 + row_pre, temp, COLOR_BLACK, COLOR_WHITE, 1, 2 * col);
	cout.clear();
	cout << endl;
	delete[] temp;

	len = strlen(book_now) - 3;	// + 1 - strlen(".txt")
	temp = new char[len];
	strncpy(temp, book_now, len - 1);
	temp[len - 1] = '\0';
	showstr(x_position + 2, y_position + 2 + row_now, temp, COLOR_WHITE, COLOR_BLACK, 1, 2 * col);
	cout.clear();
	cout << endl;
	delete[] temp;
}

char** select_book(char ***book_list, const int x_position, const int y_position, const int row, const int col)
{
	disp_booklist(book_list, x_position, y_position, row, col);
	move_cur_selecting_book(**book_list, **book_list, 0, 0, col, x_position, y_position);
	int flag = 1, row_now = 0;
	char** firstline_book = *book_list;
	while (1) {
		flag = 1;
		while (flag) {
			switch (_getch())
			{
				case 224:
					switch (_getch())
					{
						case 80:	//��
							flag = 0;
							if (row_now + 1 == row) {		//��ǰ��ѡ����ʾ�б����һ��
								if (*(firstline_book + row) != NULL) {	//���һ�������δ��ʾ
									firstline_book++;
									disp_booklist(&firstline_book, x_position, y_position, row, col);
									move_cur_selecting_book(*(firstline_book + row_now), *(firstline_book + row_now), row_now, row_now, col, x_position, y_position);
								}
							}
							else if (*(firstline_book + row_now + 1) != NULL) {		//x��ʾ���������б�߶ȵ�����ﵽ�ײ�
								move_cur_selecting_book(*(firstline_book + row_now + 1), *(firstline_book + row_now), row_now + 1, row_now, col, x_position, y_position);
								row_now++;
							}
							break;
						case 72:	//��
							flag = 0;
							if (row_now == 0) {		//��ǰ��ѡ����ʾ�б��һ��
								if (firstline_book != *book_list) {		//��ǰ�б�ǰ����δ��ʾ
									firstline_book--;
									disp_booklist(&firstline_book, x_position, y_position, row, col);
									move_cur_selecting_book(*firstline_book, *firstline_book, 0, 0, col, x_position, y_position);
								}
							}
							else {
								move_cur_selecting_book(*(firstline_book + row_now - 1), *(firstline_book + row_now), row_now - 1, row_now, col, x_position, y_position);
								row_now--;
							}
							break;
						default:
							break;
					}
					break;
				case '\r':	//�س�ȷ��
					return firstline_book + row_now;
				default:
					break;
			}
		}
	}
	return NULL;	//�߼��ϲ���ִ������
}

char** select_disp_booklist(char ***book_list, const int x_position, const int y_position, const int row, const int col)
{
	showstr(x_position, y_position, "��ѡ���鼮");
	draw_edge_img_no_div(row, col, x_position - 2, y_position, 0, COLOR_BLACK, COLOR_WHITE);
	return select_book(book_list, x_position, y_position, row, col);
}

//�������һ�����ļ��е�λ��
//mode=1(�������з���Ȼ����)
//mode=2(�������з� ��һ��ǰ�޿ո�TAB�򲻻��� ����һ�����ݼ���)
streamoff disp_book_words_singlepage(ifstream* fin_book, const int row, const int col, const int x_position, const int y_position, streamoff &pos_first, const int mode = 1, const int down_up = 0, const int is_disp = 1)
{
	if (down_up == -1)
		fin_book->seekg(0, ios::beg);
	else
		fin_book->seekg(pos_first, ios::beg);
	char *words, **output;
	int count = 0, flag_parafirstline = 1, count_ex = 0;
	int i;
	streamoff pos;

	while (fin_book->peek() == '\n')	//����ǰ���Ļ���
		fin_book->ignore();
	words = new char[2 * col + 1]{ '\0' };
	if (words == NULL) {
		cout << "�ڴ�ռ�����" << endl;
		return -1;
	}

	output = new char*[row];
	if (output == NULL) {
		cout << "�ڴ�ռ�����" << endl;
		return -1;
	}
	for (i = 0; i < row; i++) {
		output[i] = new char[2 * col + 1]{ '\0' };
		if (output[i] == NULL) {
			cout << "�ڴ�ռ�����" << endl;
			while (i) {
				delete[] output[i - 1];
				i--;
			}
			delete[] output;
			delete[] words;
			return -1;
		}
		memset(output[i], ' ', sizeof(char) * 2 * col);
	}

	int flag_begin = 0;
	if (mode == 1) {
		while (count < row) {
			count_ex = 0;
			pos = fin_book->tellg();
			fin_book->getline(words, 2 * col);

			if (fin_book->eof())
				break;

			if (fin_book->fail())		//���ȳ���2*colʱ���д����� ��Ҫ���ñ��
				fin_book->clear();

			if (words[strlen(words) - 1] < 0) {	//����չ���루���ģ���β �����а���ַ�����������
				for (i = 0; words[i] != '\0'; i++)
					if (words[i] < 0)
						count_ex++;
				if (count_ex % 2) {	//�в���������������ַ�
					fin_book->unget();
					words[strlen(words) - 1] = ' ';
				}
			}

			if (!flag_begin) {
				if (down_up == -1 && fin_book->tellg() < pos_first)
					continue;
				else if (down_up == 1 && pos <= pos_first)
					continue;
				else
					flag_begin = 1;
				pos_first = pos;
			}

			if ((int)strlen(words) < 2 * col - 1)
				flag_parafirstline = 1;

			output[count][0] = '\0';
			strcpy(output[count], words);
			output[count][strlen(words)] = ' ';
			count++;
		}
	}
	else {
		int len_wait_for_in, len_has_in;
		flag_parafirstline = 0;
		if (down_up == 0)
			flag_begin = 1;
		while (count < row) {
			if (flag_parafirstline) {
				count++;
				flag_parafirstline = 0;
			}
			pos = fin_book->tellg();

			fin_book->getline(words, 2 * col);
			if (fin_book->eof())
				break;

			if (fin_book->fail())		//���ȳ���2*colʱ���д����� ��Ҫ���ñ��
				fin_book->clear();

			char ch = fin_book->peek();
			if (ch == ' ' || ch == '\t')	//�����ѵ���ĩ
				flag_parafirstline = 1;

			len_wait_for_in = strlen(words);
			len_has_in = strlen(output[count]);
			if (len_has_in == 2 * col)
				len_has_in = 0;
			if (len_has_in + len_wait_for_in < 2 * col) {
				strcpy(output[count] + len_has_in, words);
			}
			else {
				count_ex = 0;
				strncpy(output[count] + len_has_in, words, 2 * col - len_has_in);
				int is_ed = 0;
				if (count + 1 == row)
					is_ed = 1;
				if (output[count][2 * col - 1] < 0) {	//����չ���루���ģ���β �����а���ַ�����������
					for (i = 0; output[count][i] != '\0'; i++)
						if (output[count][i] < 0)
							count_ex++;
					if (count_ex % 2) {	//�в���������������ַ�
						if (!is_ed) {
							output[count + 1][0] = output[count][i - 1];
							output[count + 1][1] = '\0';
						}
						output[count][i - 1] = ' ';
					}
				}
				if (is_ed)
					break;

				if (!flag_begin) {
					if (down_up == -1 && fin_book->tellg() < pos_first)
						continue;
					else if (down_up == 1 && pos <= pos_first)
						continue;
					else
						flag_begin = 1;
					fin_book->seekg(-(len_has_in + len_wait_for_in - 2 * col + count_ex % 2), ios::cur);
					pos_first = fin_book->tellg();
					memset(output[0], ' ', sizeof(char) * 2 * col);
					memset(output[1], ' ', sizeof(char) * 2 * col);
					continue;
				}

				count++;
				strcpy(output[count] + count_ex % 2, words + 2 * col - len_has_in);
			}
			if (ch == '\n') {
				if (!flag_begin) {
					if (down_up == -1 && fin_book->tellg() < pos_first)
						continue;
					else if (down_up == 1 && pos <= pos_first)
						continue;
					else
						flag_begin = 1;
					fin_book->seekg(-(streamoff)strlen(output[count]) + 1, ios::cur);
					pos_first = fin_book->tellg();
					for (int j = 0; j <= count; j++)
						memset(output[j], ' ', sizeof(char) * 2 * col);
					count = 0;
					continue;
				}

				output[count][strlen(output[count])] = ' ';
				count++;
			}
		}
		count++;
	}

	//showstr(x_position, y_position + count, words, COLOR_BLACK, COLOR_WHITE, 1, 2 * col);
	for (i = 0; i < row; i++) {
		if (is_disp) {
			gotoxy(x_position, y_position + i);
			if (strlen(output[i]) != 2 * col)
				output[i][strlen(output[i])] = ' ';
			cout << output[i];
		}
		delete[] output[i];
	}

	//��ʾ״̬��
	showstr(10, 72, "�����ƶ� | PgUp/PgDn��ҳ");
	showstr(10, 73, "S ������ת | N ����ѡ�� | Q �˳� | F ��ʽת��");
	showstr(100, 71, "��ǰ���ȣ�");
	fin_book->clear();
	streamoff now = fin_book->tellg();
	fin_book->seekg(ios::end);
	cout << setprecision(2) << pos_first / fin_book->tellg() << "%";
	fin_book->seekg(now, ios::cur);

	delete[] output;
	delete[] words;
	return pos;
}


//����״̬ -1:error 0:�˳��������� 1:����ѡ��
int disp_book_words(const char *book_txt, const char* dir, const int row, const int col, const int x_position, const int y_position, streamoff& pos_first)
{
	ifstream fin_book;
	char *book_dir = new char[strlen(book_txt) + strlen(dir) + 2];
	strcpy(book_dir, dir);
	strcat(book_dir, "/");
	strcat(book_dir, book_txt);
	fin_book.open(book_dir, ios::in);
	if (!fin_book.is_open()) {
		cout << book_dir << "��ʧ��" << endl;
		return -1;
	}
	draw_edge_img_no_div(row, col, x_position - 2, y_position - 1, 0, COLOR_BLACK, COLOR_WHITE);
	streamoff t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, 1, 0);

	int mode = 1;
	streamoff t_first, end;
	double per;
	while (1) {
		switch (_getch())
		{
			case 224:
				switch (_getch())
				{
					case 72:	//��
						t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode, -1);
						break;
					case 80:	//��
						t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode, 1);
						break;
					case 73:	//PgUp
						t_first = pos_first;
						while (1) {
							t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode, 0, 0);
							if (pos_first == 0)
								break;
							if (t > t_first)
								continue;
						}
						t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode, 0);
						break;
					case 81:	//PgDw
						pos_first = t;
						t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode, 0);
						break;
					default:
						break;
				}
				break;
			case 'S':
			case 's':
				fin_book.seekg(ios::end);
				end = fin_book.tellg();
				showstr(0, 0, "������ȣ�[0-100]%   ");
				cin >> per;
				showstr(0, 0, "                                       ");
				if (per - 100 > 1e-6)
					per = 100;
				else if (per < 1e-6)
					per = 0;
				pos_first = int(end * per);
				t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode);
				break;
			case 'N':
			case 'n':
				return 1;
			case 'Q':
			case 'q':
				return 0;
			case 'f':
			case 'F':
				mode = 3 - mode;
				t = disp_book_words_singlepage(&fin_book, row, col, x_position + 2, y_position + 1, pos_first, mode);
				break;
			default:
				break;
		}
	}
	return -1;
}