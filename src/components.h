#ifndef Components_h
#define Components_h

Piece *Piece_create(int id, int color, int row, int col, int moved);
void Piece_destroy(Piece *piece);
Square *Square_create(int pc_color, int id, int row, int col, Square *enpassant, int enp_id, int moved);
void Square_destroy(Square *square);

#endif
