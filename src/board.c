#include "chess.h"

void print_char(int id)
{
	switch(id) {
		case 0:
			wprintf(L"%s", "a ");
			break;
		case 1:
			wprintf(L"%s", "b ");
			break;
		case 2:
			wprintf(L"%s", "c ");
			break;
		case 3:
			wprintf(L"%s", "d ");
			break;
		case 4:
			wprintf(L"%s", "e ");
			break;
		case 5:
			wprintf(L"%s", "f ");
			break;
		case 6:
			wprintf(L"%s", "g ");
			break;
		case 7:
			wprintf(L"%s", "h ");
			break;
		default:
			break;
	}
}


int set_piece_start(int pos)
{
	switch(pos) {
		rook:	case 0:
					return 3; //rook id
		knight:	case 1:
					return 5; //knight id
		bishop:	case 2:
					return 4; //bishop id
				case 3:
					return 2; //queen id
				case 4:
					return 1; //king id
				case 5:
					goto bishop;
				case 6: 
					goto knight;
				case 7:
					goto rook;
				default:
					return 0;
	}
}


void setup_board(Square *barr[8][8], int m, int n)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			int color; 
			if (i == 0 || i == 7) {
				color = (i == 0) ? 1 : 0;
				int ptype = set_piece_start(j);
				barr[i][j] = Square_create(color, ptype, i, j, NULL, -1, 0);
			} else if (i == 1 || i == 6) {
				color = (i == 1) ? 1 : 0;
				barr[i][j] = Square_create(color, 6, i, j, NULL, -1, 0);
			} else {
				barr[i][j] = Square_create(0, 0, i, j, NULL, -1, 0);
			}
		}
	}
}


void update_board(Square *barr[8][8], Square *start, int row_e, int col_e)
{
	int row_s = start->row;
	int col_s = start->col;

	start->piece->moved = 1;

	if (barr[row_e][col_e]->piece != NULL) // if the destination contains a piece free it
		free(barr[row_e][col_e]->piece);

	barr[row_e][col_e]->piece = start->piece; // set the destination square's piece attribute pointer to the piece that was moved
	barr[row_s][col_s]->piece = NULL; // remove reference to piece on starting square

	if (barr[row_e][col_e]->enpassant != NULL) {

		if (barr[row_e][col_e]->piece->id == 6) { // if piece moved is pawn (id for pawn is 6), if not pawn, then enemy pawn not captured
			free(barr[row_e][col_e]->enpassant->piece);
			barr[row_e][col_e]->enpassant->piece = NULL; // remove target pawn of enpassant move		
		}
		// remove enpassant reference
		barr[row_e][col_e]->enpassant = NULL;
		barr[row_e][col_e]->enpassant_id = -1;
	}

	// update rook position for castle
	if (barr[row_e][col_e]->piece->id == 1 && ((col_e == col_s + 2) || (col_e == col_s - 2))) {
		Square *rook;
		int dest;

		if (col_e == 2) {
			rook = barr[row_s][0];
			dest = 3;
		} else {
			rook = barr[row_s][7];
			dest = 5;
		}
		
		rook->piece->moved = 1;

		barr[row_s][dest]->piece = rook->piece;
		rook->piece = NULL;
	}
}



void setup_test_board(Square *test_barr[8][8], Square *barr[8][8], Square *start, int row_e, int col_e)
{

	int i, j;

	for (i = 0; i < 8; i++ ) {
		for (j = 0; j < 8; j++) {
			if (test_barr[i][j] == NULL) {
				int id = 0;
				int color = 0;
				Square *enpassant = NULL;
				int enp_id = -1;
				int has_moved = 0;
			
				if (barr[i][j]->piece != NULL) {
					id = barr[i][j]->piece->id;
					color = barr[i][j]->piece->color;
					has_moved = barr[i][j]->piece->moved;
				}

				if (barr[i][j]->enpassant != NULL) {
					int row = barr[i][j]->enpassant->row;
					int col = barr[i][j]->enpassant->col;
					enp_id = barr[i][j]->enpassant_id;

					if (test_barr[row][col] == NULL) {
						int id_other = 0;
						int color_other = 0;
						int has_moved_other = 0;
					
						if (barr[row][col]->piece != NULL) {
							id_other = barr[row][col]->piece->id;
							color_other = barr[row][col]->piece->color;
							has_moved_other = barr[row][col]->piece->moved;
						}

						test_barr[row][col] = Square_create(color_other, id_other, row, col, NULL, -1, has_moved_other);
					}
					
					enpassant = test_barr[row][col];
				}
			

				test_barr[i][j] = Square_create(color, id, i, j, enpassant, enp_id, has_moved);
			}
			
		}
	}

	int row_s = start->row;
	int col_s = start->col;

	Square *position = test_barr[row_s][col_s];

	update_board(test_barr, position, row_e, col_e);
}


void board_destroy(Square *barr[8][8])
{

	int i, j;

	for (i = 0; i < 8; i++ ) {
		for (j = 0; j < 8; j++) {
			Square_destroy(barr[i][j]);
		}
	}
}


void draw_board(Square *barr[8][8], int m, int n)
{
	setlocale(LC_CTYPE, "");
	int i = 0;
	int j = 0;

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if (j == 0) {
				int row_num = m - i;
				wprintf(L"%d ", row_num);
			}

			if (barr[i][j]->piece) {
				wprintf(L"%lc ", barr[i][j]->piece->img);
			} else {
				wprintf(L"%lc ", barr[i][j]->img);
			}
		}
		
		wprintf(L"%lc\n", BLANK);
	}

	for (i = 0; i < m; i++) {
		if (i == 0) {
			wprintf(L"%lc ", BLANK);
		}
		print_char(i);
	}

	wprintf(L"%lc\n", BLANK);
}


