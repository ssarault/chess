#include "chess.h"

Square *find_king(Square *barr[8][8], int color)
{
	int i;
	int j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (barr[i][j]->piece != NULL && barr[i][j]->piece->id == 1 && barr[i][j]->piece->color == color) {
				return barr[i][j];
			}
		}
	}

	return NULL;
}

int in_check(Square *barr[8][8], int color, Square *king_sq)
{
	if (king_sq == NULL)
		king_sq = find_king(barr, color);

	int opp_color;

	if (color == 0) {
		opp_color = 1;
	} else {
		opp_color = 0;
	}

	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (barr[i][j]->piece != NULL && barr[i][j]->piece->color == opp_color) {
				int piece_id = barr[i][j]->piece->id;
				
				Square *pos = barr[i][j];

				int legal;

				switch(piece_id) {
					case 2: // fallthrough
					case 3:
					case 4:
						legal = check_move_qrb(pos, king_sq, barr, opp_color);
						break;
					case 5:
						legal = check_move_knight(pos, king_sq, barr, opp_color);
						break;
					case 6:
						legal = check_move_pawn(pos, king_sq, barr, opp_color);
						break;
					default:
						legal = 0;
						break;
				}
			
				if (legal) //if opponent can reach king then player is in check so return 1 for in check is true
					return 1;
			}
		}
	}
	
	return 0; // not in check, return false so 0
}


int has_legal_move_pawn(Square *barr[8][8], Square *position, Square *king_sq)
{
	int row_s = position->row;
	int col_s = position->col;
	int color = position->piece->color;

	int mod;

	if (color == 0)
		mod = -1;
	else
		mod = 1;

	int moves[4][2];

	moves[0][0] = row_s + mod * 1;
	moves[0][1] = col_s;
	moves[1][0] = row_s + mod * 1;
	moves[1][1] = col_s;
	moves[2][0] = row_s + mod * 1;
	moves[2][1] = col_s + 1;
	moves[3][0] = row_s + mod * 2;
	moves[3][1] = col_s;

	int i;

	for (i = 0; i < 4; i++) {
		int row = moves[i][0];
		int col = moves[i][1];

		if ((row < 0) || (row > 7) || (col < 0) || (col > 7))
			continue;

		Square *end = barr[row][col];

		int valid = check_move_pawn(position, end, barr, color);
		if (valid) {
			Square *check_barr[8][8] = { NULL };
			setup_test_board(check_barr, barr, position, row, col);
			int check = in_check(check_barr, color, king_sq);
			board_destroy(check_barr);
			if (!check)
				return 1; 
		}
	}

	return 0;
}


int has_legal_move_knight(Square *barr[8][8], Square *position, Square *king_sq)
{
	int row_s = position->row;
	int col_s = position->col;
	int color = position->piece->color;

	int moves[8][2];

	moves[0][0] = row_s - 2;
	moves[0][1] = col_s + 1;
	moves[1][0] = row_s - 2;
	moves[1][1] = col_s - 1;
	moves[2][0] = row_s - 1;
	moves[2][1] = col_s + 2;
	moves[3][0] = row_s + 1;
	moves[3][1] = col_s + 2;
	moves[4][0] = row_s + 2;
	moves[4][1] = col_s + 1;
	moves[5][0] = row_s + 2;
	moves[5][1] = col_s - 1;
	moves[6][0] = row_s + 1;
	moves[6][1] = col_s - 2;
	moves[7][0] = row_s - 1;
	moves[7][1] = col_s - 2;


	int i;

	for (i = 0; i < 8; i++) {
		int row = moves[i][0];
		int col = moves[i][1];

		if ((row < 0) || (row > 7) || (col < 0) || (col > 7))
			continue;

		if (barr[row][col]->piece != NULL && barr[row][col]->piece->color == color)
			continue;
		
		Square *check_barr[8][8] = { NULL };
		setup_test_board(check_barr, barr, position, row, col);
		int check = in_check(check_barr, color, king_sq);
		board_destroy(check_barr);
		if (!check)
			return 1; 
	}

	return 0;
}


int has_legal_move_king(Square *barr[8][8], Square *position)
{
	int row_s = position->row;
	int col_s = position->col;
	int color = position->piece->color;

	int moves[8][2];

	moves[0][0] = row_s - 1;
	moves[0][1] = col_s;
	moves[1][0] = row_s - 1;
	moves[1][1] = col_s + 1;
	moves[2][0] = row_s;
	moves[2][1] = col_s + 1;
	moves[3][0] = row_s + 1;
	moves[3][1] = col_s + 1;
	moves[4][0] = row_s + 1;
	moves[4][1] = col_s;
	moves[5][0] = row_s + 1;
	moves[5][1] = col_s - 1;
	moves[6][0] = row_s;
	moves[6][1] = col_s - 1;
	moves[7][0] = row_s - 1;
	moves[7][1] = col_s - 1;

	// no need to check if king can castle because castle will never be only legal move

	int i;

	for (i = 0; i < 8; i++) {
		int row = moves[i][0];
		int col = moves[i][1];

		if ((row < 0) || (row > 7) || (col < 0) || (col > 7))
			continue;

		if (barr[row][col]->piece != NULL && barr[row][col]->piece->color == color)
			continue;
		
		Square *check_barr[8][8] = { NULL };
		setup_test_board(check_barr, barr, position, row, col);
		Square *king_sq = check_barr[row][col];
		int check = in_check(check_barr, color, king_sq);
		board_destroy(check_barr);
		if (!check)
			return 1; 
	}

	return 0;
}


int has_legal_move_qrb(Square *barr[8][8], Square *start, Square *king_sq) 
{
	int id = start->piece->id;
	int counter = 1;

	int row_s = start->row;
	int col_s = start->col;
	int color = start->piece->color;

	int row = col_s;
	int col = col_s;

	//start by searching up
	int mod_col = 0;
	int mod_row = -1;
 	int blocked = 0;

	if (king_sq == NULL)
		king_sq = find_king(barr, color);
		
	while (1) {

		if (blocked == 1) {
			row = row_s;
			col = col_s;
			blocked = 0;

			switch(counter) {
				case 1:
					mod_row = 1;
					break;
				case 2:
					mod_row = -1;
					mod_col = 1;
					break;
				case 3:
					mod_row = 1;
					mod_col = -1;
					break;
				case 4:
					mod_row = 0;
					mod_col = 1;
					break;
				case 5:
					mod_col = -1;
					break;
				case 6:
					mod_row = 1;
					mod_col = 1;
					break;
				case 7:
					mod_row = -1;
					mod_col = -1;
					break;
				default:
					return 0;
			}
			
			counter++;
		}

		// skip horizontal and vertical for bishop
		if (((counter == 1) || (counter == 5)) && id == 4) {
			if (counter == 1)
				mod_row = -1;
			else
				mod_row = 1;

			row = row_s;
			col = col_s;
			mod_col = 1;
			counter += 2;
		}

		//skip diagnol for rook
		if (((counter == 3) || (counter == 7)) && id == 3) {
			if (counter == 7)
				return 0;

			row = row_s;
			col = col_s;
			mod_row = 0;
			mod_col = 1;
			counter += 2;
		}

		row += mod_row;
		col += mod_col;

		if ((row < 0) || (row > 7) || (col < 0) || (col > 7)) {
			blocked = 1;
			continue;
		}

		if (barr[row][col]->piece != NULL) {
			blocked = 1;

			if (barr[row][col]->piece->color == color)
				continue;
		}

		Square *check_barr[8][8] = { NULL };
		setup_test_board(check_barr, barr, start, row, col);
		int check = in_check(check_barr, color, king_sq);
		board_destroy(check_barr);
		if (!check)
			return 1; 
	}
	
	return 0;
}			


int has_legal_move(Square *barr[8][8], int color, Square *king_sq)
{

	if (king_sq == NULL)
		king_sq = find_king(barr, color);

	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (barr[i][j]->piece != NULL && barr[i][j]->piece->color == color) {
				Square *pos = barr[i][j];
				int id = barr[i][j]->piece->id;
				int legal;

				switch(id) {
					case 1:
						legal = has_legal_move_king(barr, pos);
						break;
					case 2: //fallthrough
					case 3:
					case 4:
						legal = has_legal_move_qrb(barr, pos, king_sq);
						break;
					case 5:
						legal = has_legal_move_knight(barr, pos, king_sq);
						break;
					case 6:
						legal = has_legal_move_pawn(barr, pos, king_sq);
						break;
					default:
						legal = 0;
						break;
				}

				if (legal)
					return 1;
			}
		}	
	}

	return 0; // No legal move found
}


int check_material(Square *barr[8][8])
{
	int white_count = 0;
	int black_count = 0;

	int white_bishop_same_color = -1;
	int black_bishop_same_color = -1;

	int WK = 0; // king
	int WQ = 0; // queen
	int WR = 0; // rook
	int WB = 0; // bishop
	int WN = 0; // knight
	int WP = 0; // pawn

	int BK = 0; // king
	int BQ = 0; // queen
	int BR = 0; // rook
	int BB = 0; // bishop
	int BN = 0; // knight
	int BP = 0; // pawn

	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (barr[i][j]->piece != NULL) {

				int color = barr[i][j]->piece->color;
				int id = barr[i][j]->piece->id;
				int sq_color = barr[i][j]->color;
			
				if (color)
					black_count++;
				else
					white_count++;
			
				if (id == 6) {
					if (color)
						BP++;
					else
						WP++;
				}

				else if (id == 5) {
					if (color)
						BN++;
					else
						WN++;
				}

				else if (id == 4) {
					if (color) {
						BB++;

						if (black_bishop_same_color == -1)
							black_bishop_same_color = sq_color;
						else if (black_bishop_same_color != sq_color)
							black_bishop_same_color = 2;
					}
					else {
						WB++;

						if (white_bishop_same_color == -1)
							white_bishop_same_color = sq_color;
						else if (white_bishop_same_color != sq_color)
							white_bishop_same_color = 2;
					}
				}

				else if (id == 3) {
					if (color)
						BR++;
					else
						WR++;
				}	
			
				else if (id == 2) {
					if (color)
						BQ++;
					else
						WQ++;
				}

				else {
					if (color)
						BK++;
					else
						WK++;
				}
			}
		}
	}
		
	// king vs king
	if (white_count == 1 && black_count == 1)
		return 1;

	// king and unlimited bishops on same color square vs same
	if (WQ == 0 && WR == 0 && WN == 0 && WP == 0 && WB >= 1 && BQ == 0 && BR == 0 && BN == 0 && BP == 0 && BB >= 1) {
		if (white_bishop_same_color != 2 && black_bishop_same_color != 2)
			return 1;
	}

	// white king and bishop vs black king
	if (white_count == 2 && WB == 1 && black_count == 1)
		return 1;

	// white king and knight vs black king
	if (white_count == 2 && WN == 1 && black_count == 1)
		return 1;

	// black king and bishop vs  white king
	if (black_count == 2 && BB == 1 && white_count == 1)
		return 1;

	// black king and knight vs white king
	if (black_count == 2 && BN == 1 && white_count == 1)
		return 1;
					

	return 0; // no draw, there is sufficient material to checkmate
}

