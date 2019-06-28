#ifndef Board_h
#define Board_h

void print_char(int id);
int set_piece_start(int pos);
void setup_board(Square *barr[8][8], int m, int n);
void update_board(Square *barr[8][8], Square *start, int row_e, int col_e);
void setup_test_board(Square *test_barr[8][8], Square *barr[8][8], Square *start, int row_e, int col_e);
void board_destroy(Square *barr[8][8]);
void draw_board(Square *barr[8][8], int m, int n);

#endif
