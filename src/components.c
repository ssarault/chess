#include "chess.h"

Piece *Piece_create(int id, int color, int row, int col, int moved)
{
	Piece *piece = malloc(sizeof(struct Piece));
	assert(piece != NULL);

	piece->id = id;
	piece->color = color;

	
	switch(id) {
		case 1:
			if (color == 0) {
				piece->img = KING_W;
			} else {
				piece->img = KING_B;
			}
			break;
		case 2:
			if (color == 0) {
				piece->img = QUEEN_W;
			} else {
				piece->img = QUEEN_B;
			}
			break;
		case 3:
			if (color == 0) {
				piece->img = ROOK_W;
			} else {
				piece->img = ROOK_B;
			}
			break;
		case 4:
			if (color == 0) {
				piece->img = BISHOP_W;
			} else {
				piece->img = BISHOP_B;
			}
			break;
		case 5:
			if (color == 0) {
				piece->img = KNIGHT_W;
			} else {
				piece->img = KNIGHT_B;
			}
			break;
		default:
			if (color == 0) {
				piece->img = PAWN_W;
			} else {
				piece->img = PAWN_B;
			}
			break;
	}

	piece->row = row;
	piece->col = col;
	
	piece->moved = moved;

	return piece;
}


void Piece_destroy(Piece *piece)
{
	free(piece);
}


Square *Square_create(int pc_color, int id, int row, int col, Square *enpassant, int enp_id, int moved)
{
	Square *square = malloc(sizeof(struct Square));
	assert(square != NULL);
	
	//square->color = color;
	int color;

	if ((row % 2) > 0 && (col % 2) == 0) {
		color = 1; //black
	} else if ((col % 2) > 0 && (row % 2) == 0) {
		color = 1;
	} else {
		color = 0; //white
	}

	square->color = color;

	if (square->color == 0) {
		square->img = SQUARE_W;
	} else {
		square->img = SQUARE_B;
	}

	if (id != 0) {
		square->piece = Piece_create(id, pc_color, row, col, moved);
	} else {
		square->piece = NULL;
	}
	
	square->row = row;
	square->col = col;

	square->enpassant = enpassant;
	square->enpassant_id = enp_id;

	return square;
}


void Square_destroy(Square *square)
{
	if (square != NULL) {
		if (square->piece != NULL) {
			free(square->piece);
			square->piece = NULL;
		}

		square->enpassant = NULL;
		free(square);
	}	
}

