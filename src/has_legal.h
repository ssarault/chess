#ifndef Has_legal_h
#define Has_legal_h

Square *find_king(Square *barr[8][8], int color);
int in_check(Square *barr[8][8], int color, Square *king_sq);
int has_legal_move_pawn(Square *barr[8][8], Square *position, Square *king_sq);
int has_legal_move_knight(Square *barr[8][8], Square *position, Square *king_sq);
int has_legal_move_king(Square *barr[8][8], Square *position);
int has_legal_move_qrb(Square *barr[8][8], Square *start, Square *king_sq);
int has_legal_move(Square *barr[8][8], int color, Square *king_sq);
int check_material(Square *barr[8][8]);

#endif
