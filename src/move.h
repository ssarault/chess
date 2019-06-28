#ifndef Move_h
#define Move_h

int pawn_promote(Square *pawn_sq);
int Piece_move(Square *start, Square *end, Square *barr[8][8], int player);
int move_input(Square *barr[8][8], int player);
int *parse_input();
int *validate_input(char *mv);
int char_to_int(char c);

#endif
