// 1752762 ��1�� κ�ٴ�

#include <fstream>
using namespace std;
/* �鵥�б���ʾ��ѡ�� ������ѡ���ָ��*/
char** select_disp_booklist(char ***book_list, const int x_position, const int y_position, const int row, const int col);

/* ��ȡ�鵥�б� */
int read_book_list(char ***list, const char *dir);

/* ��ȡ�鵥��ʷ�Ķ�λ�� */
int read_pos(char*** save_inf, const char *book_txt, const char* dir);
/* �����鵥��ʷ�Ķ�λ�� */
int save_pos(char*** save_inf, const char *book_txt, const char* dir, const streamoff pos, const int is_new);

/* ����״̬ -1:error 0:�˳��������� 1:����ѡ�� down_up:1Ϊ�� -1Ϊ�� */
int disp_book_words(const char *book_txt, const char* dir, const int row, const int col, const int x_position, const int y_position, streamoff& pos_first);
