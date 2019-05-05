// 1752762 计1班 魏鹳达

#include <fstream>
using namespace std;
/* 书单列表显示与选择 返回所选书的指针*/
char** select_disp_booklist(char ***book_list, const int x_position, const int y_position, const int row, const int col);

/* 读取书单列表 */
int read_book_list(char ***list, const char *dir);

/* 读取书单历史阅读位置 */
int read_pos(char*** save_inf, const char *book_txt, const char* dir);
/* 保存书单历史阅读位置 */
int save_pos(char*** save_inf, const char *book_txt, const char* dir, const streamoff pos, const int is_new);

/* 返回状态 -1:error 0:退出整个程序 1:重新选择 down_up:1为下 -1为上 */
int disp_book_words(const char *book_txt, const char* dir, const int row, const int col, const int x_position, const int y_position, streamoff& pos_first);
