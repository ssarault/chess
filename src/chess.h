#ifndef Chess_h
#define Chess_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <wchar.h>
#include <unistd.h>
#include <stdio_ext.h>

#define BLANK 0x0020

#define SQUARE_W 0x25a0
#define SQUARE_B 0x25a1

#define KING_W 0x265a
#define QUEEN_W 0x265b
#define ROOK_W 0x265c
#define BISHOP_W 0x265d
#define KNIGHT_W 0x265e
#define PAWN_W 0x265f

#define KING_B 0x2654
#define QUEEN_B 0x2655
#define ROOK_B 0x2656
#define BISHOP_B 0x2657
#define KNIGHT_B 0x2658
#define PAWN_B 0x2659

typedef struct Piece {
	int id; // number to identify piece type;
	int color; // 0 for white, 1 for black
	wchar_t img; // unicode hex symbol
	int row; // row position
	int col; // column position
	int moved;
} Piece;

typedef struct Square {
	int color;
	wchar_t img;
	Piece *piece;
	int row;
	int col;
	struct Square *enpassant;
	int enpassant_id; // set to the value of turn counter when a pawn moves two spaces
} Square;

#include "board.h"
#include "check_move.h"
#include "components.h"
#include "has_legal.h"
#include "move.h"
#include "trans_table.h"

#endif
