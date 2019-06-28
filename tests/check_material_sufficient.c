#include "chess.h"
#include "components.c"
#include "board.c"
#include "check_move.c"
#include "move.c"
#include "has_legal.c"

int turn_counter = 1;

typedef struct Piece_Position {
	int row;
	int col;
	Piece *piece;
} Position;

Position *Position_create(int id, int color, int row, int col)
{
	Position *pos = malloc(sizeof(struct Piece_Position));
	pos->row = row;
	pos->col = col;
	pos->piece = Piece_create(id, color, row, col, 1);

	return pos;
}

void Position_destroy(Position *pos)
{
	//free(pos->piece);
	free(pos);
}

void setup_board_for_test(Square *barr[8][8])
{
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; i++) {
			barr[i][j] = Square_create(0, 0, i, j, NULL, -1, 0);
		}
	}
}

void set_position_for_test(Square *barr[8][8], Position *pieces, int size_arr)
{
	int i, j;

	int k = 0;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; i++) {

			if (barr[i][j]->piece != NULL) {
				free(barr[i][j]->piece);
				barr[i][j]->piece = NULL;
			}

			if (k < size_arr) {
				if (pieces[k]->row == i && pieces[k]->col == j) {
					barr[i][j]->piece = pieces[k]->piece;
					k++;
				}
			}
		}
	}
}

void free_position_arr(Position *pieces, int size_arr)
{
	int i;

	for (i = 0; i < size_arr; i++) {
		Position_destroy(pieces[i]);
	}
}
