#include "chess.h"

extern int turn_counter;

int check_move_king_castle(Square *start, Square *end, Square *barr[8][8], int color)
{
	int row_s = start->row;
	int col_s = start->col;
	int row_e = end->row;
	int col_e = end->col;

	// check king has not moved prior and has moved to a square that could be a legal castle
	if ((start->piece->moved) || (row_s != row_e) || (col_e != 2 && col_e != 6))
		return 0;

	// check rook hasn't moved
	if (col_e == 2 && ((barr[row_s][0]->piece == NULL) || (barr[row_s][0]->piece->moved)))
		return 0;
	
	if (col_e == 6 && ((barr[row_s][7]->piece == NULL) || (barr[row_s][7]->piece->moved)))
		return 0;

	// make sure player is not in check
	int check = in_check(barr, color, start);
	if (check)
		return 0;

	// now we will make sure the path is clear and king does not move through check
		
	int col;
	int dest;

	if (col_e > col_s) {
		col = col_s;
		dest = 7; // rook on right side of board
	} else {
		col = 0; // square to right of rook on left side after ++col executed
		dest = col_s;
	}

	for (++col; col < dest; col++) {
		Square *position = barr[row_s][col];

		if (position->piece != NULL) //make sure no pieces between king and rook
			return 0;
		
		if (col != 1) { // column 1 does not need to be checked for check because king does not move to or thru here
			check = in_check(barr, color, position);
			if (check)
				return 0; // can't move through check
		}
	}
		
	return 1; // move is legal
}


int check_move_king(Square *start, Square *end, Square *barr[8][8], int color)
{
	int row_s = start->row;
	int col_s = start->col;
	int row_e = end->row;
	int col_e = end->col;

	if (barr[row_e][col_e]->piece != NULL && barr[row_e][col_e]->piece->color == color) {
		return 0; // square occupied by friendly so move is illegal
	}

	if (row_s == row_e && ((col_e == 2) || (col_e == 6))) {
		int castle = check_move_king_castle(start, end, barr, color);
		if (castle)
			return 1;
	}

	if ((row_e < row_s - 1) || (row_e > row_s + 1)) {
		return 0;
	}

	if ((col_e < col_s - 1) || (col_e > col_s + 1)) {
		return 0;
	}

	if (barr[row_e][col_e]->piece != NULL && barr[row_e][col_e]->piece->color == color) {
		return 0;
	}

	return 1;
}


int check_move_qrb(Square *start, Square *end, Square *barr[8][8], int color)
{
	int id = start->piece->id;
	int row_s = start->row;
	int col_s = start->col;
	int row_e = end->row;
	int col_e = end->col;

	if (barr[row_e][col_e]->piece != NULL && barr[row_e][col_e]->piece->color == color) {
		return 0;
	}

	// Now if the destination is empty or contains an enemy check that the path is clear and a straight line

	float ys = (float)row_s;
	float ye = (float)row_e;
	float xs = (float)col_s;
	float xe = (float)col_e;

	float y = ye - ys;
	float x =  xe - xs;
	float slope;

	if (x != 0.0) {
		slope = y / x;
	} else {
		slope = 0.0; // slope would be undefined here, put placeholder 0.0 to pass next check
	}

	// Here we check that the path is a straight line
	if (slope != (int)slope) {
		return 0;
	}

	if ((int)slope != 0 && (int)slope != -1 && (int)slope != 1)
		return 0; 

	// if rook check move is not diagnol
	if (id == 3 && (int)slope != 0)
		return 0;

	// if bishop check move is not horizontal or vertical
	if (id == 4 && (int)slope == 0)
		return 0;

	// Can use integer now because line is confirmed to be straight
	int posx = (int)x;
	int m = (int)slope;
	int b = -1 * (m * col_e - row_e); // get the value of b for linear equation y = mx + b

	int i;
	int j;

	// Starting here we will check if the path is clear to the destination
	if (posx != 0) { // if posx is zero slope is undefined so need to use second for loop
		
		if (col_e > col_s) {
			i = col_s;
			j = col_e;
		} else {
			i = col_e;
			j = col_s;
		}

		for (++i; i < j; i++) {
			int found_row = m * i + b; // y = mx + b
			if (barr[found_row][i]->piece != NULL) // check that there is no pieces in squares between start and destination
				return 0;
		}

	} else {
		
		if (row_e > row_s) {
			i = row_s;
			j = row_e;
		} else {
			i = row_e;
			j = row_s;
		}

		for (++i; i < j; i++) {
			if (barr[i][col_e]->piece != NULL) // check that square is empty like above
				return 0;
		}
	}

	return 1; //if we reach this line the move is legal
}


int check_move_knight(Square *start, Square *end, Square *barr[8][8], int color)
{
	int row_s = start->row;
	int col_s = start->col;
	int row_e = end->row;
	int col_e = end->col;

	// Check that the destination is empty or contains an enemy	

	if (barr[row_e][col_e]->piece != NULL && barr[row_e][col_e]->piece->color == color) {
		return 0; // square occupied by friendly so move is illegal
	}

	// Check if move is up or down 2 squares and left or right 1 square
	
	if (((row_e == row_s + 2) || (row_e == row_s - 2)) && ((col_e == col_s + 1) || (col_e == col_s - 1)))
		return 1; // move is legal so return

	// Check if move is left or right 2 squares and up or down 1 square	

	if (((col_e == col_s + 2) || (col_e == col_s - 2)) && ((row_e == row_s + 1) || (row_e == row_s - 1)))
		return 1; // move is legal so return
	
	return 0; // if we get here the move was not legal so we return 0
}


int check_move_pawn(Square *start, Square *end, Square *barr[8][8], int color)
{
	int row_s = start->row;
	int col_s = start->col;
	int row_e = end->row;
	int col_e = end->col;

	if (barr[row_e][col_e]->piece != NULL && barr[row_e][col_e]->piece->color == color) {
		return 0;
	}

	// Double move for start position
	if (color == 0 && row_s == 6 && row_e == 4 && col_e == col_s) {
		if (barr[row_e + 1][col_e]->piece != NULL)
			return 0;
		barr[row_e + 1][col_e]->enpassant = barr[row_e][col_e];
		barr[row_e + 1][col_e]->enpassant_id = turn_counter;
		return 1;
	}

	if (color == 1 && row_s == 1 && row_e == 3 && col_e == col_s) {
		if (barr[row_e - 1][col_e]->piece != NULL)
			return 0;
		barr[row_e - 1][col_e]->enpassant = barr[row_e][col_e];
		barr[row_e -1][col_e]->enpassant_id = turn_counter;
		return 1;
	}

	if (color == 0 && row_e != row_s - 1) {
		return 0;
	}

	if (color == 1 && row_e != row_s + 1) {
		return 0;
	}

	if (col_s != col_e) {

		if ((col_e != col_s + 1) && (col_e != col_s -1)) {
			return 0;
		}

		if (barr[row_e][col_e]->piece == NULL && ((barr[row_e][col_e]->enpassant == NULL)
				|| (barr[row_e][col_e]->enpassant_id != (turn_counter - 1)))) { 
			return 0;
		} 
	}

	if (barr[row_e][col_e]->piece != NULL && col_e == col_s) {
		return 0;
	}

	return 1;
}
