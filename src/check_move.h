#ifndef Check_move_h
#define Check_move_h

int check_move_king_castle(Square *start, Square *end, Square *barr[8][8], int color);
int check_move_king(Square *start, Square *end, Square *barr[8][8], int color);
int check_move_qrb(Square *start, Square *end, Square *barr[8][8], int color);
int check_move_knight(Square *start, Square *end, Square *barr[8][8], int color);
int check_move_pawn(Square *start, Square *end, Square *barr[8][8], int color);

#endif
