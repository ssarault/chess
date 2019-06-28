#include "chess.h"

int pawn_promote(Square *pawn_sq)
{
	setlocale(LC_CTYPE, "");
	
	__fpurge(stdin);
	
	wprintf(L"%s ", "Promote your pawn. Enter a number listed below to promote to piece:\n");
	wprintf(L"%s ", "1: Queen\n");
	wprintf(L"%s ", "2: Rook\n");
	wprintf(L"%s ", "3: Bishop\n");
	wprintf(L"%s ", "4: Knight\n");

	char choice[2];
	int id;
	fgets(choice, 2, stdin);

	wprintf(L"%s ", " \n");

	switch(choice[0]) {
		case '1':
			id = 2; // Queen
			break;
		case '2':
			id = 3; // Rook
			break;
		case '3':
			id = 4; // Bishop
			break;
		case '4':
			id = 5; // Knight
			break;
		default:
			id = -1;
			break;
	}

	if (id == -1) {
		wprintf(L"%s ", "Not a valid input, try again.\n");
		wprintf(L"%s", "\n");
		return pawn_promote(pawn_sq);
	}

	wprintf(L"%s", "\n");

	int color = pawn_sq->piece->color;
	int row = pawn_sq->piece->row;
	int col = pawn_sq->piece->col;
	int moved = 1;

	Piece *promoted_piece = Piece_create(id, color, row, col, moved);
	free(pawn_sq->piece);
	pawn_sq->piece = promoted_piece;

	return 0;
}



int Piece_move(Square *start, Square *end, Square *barr[8][8], int player)
{
	int row_s = start->row;
	int col_s = start->col;
	int row_e = end->row;
	int col_e = end->col;

	if ((barr[row_s][col_s]->piece == NULL) || (barr[row_s][col_s]->piece->color != player)) {
		return 1;
	}

	int id = barr[row_s][col_s]->piece->id;
	int color = barr[row_s][col_s]->piece->color;
	int legal;

	switch(id) {
		case 1:
			legal = check_move_king(start, end, barr, color);
			break;
		case 2: // fallthrough
		case 3:
		case 4:
			legal = check_move_qrb(start, end, barr, color);
			break;
		case 5:
			legal = check_move_knight(start, end, barr, color);
			break;
		case 6:
			legal = check_move_pawn(start, end, barr, color);
			break;
		default:
			legal = 0;
			break;
	}

	if (!legal) {
		return 1;
	}

	if (id != 1 && ((col_e != 2) || (col_e != 6)) && col_s != 4) { // skip testing for check if castled

		Square *test_barr[8][8] = { NULL };
		setup_test_board(test_barr, barr, start, row_e, col_e);		
		Square *king_sq = find_king(test_barr, color);
		int check = in_check(test_barr, color, king_sq);
		board_destroy(test_barr);

	 	if (check)
			return 1;

	}

	// pawn promote
	if (id == 6 && ((row_e == 0) || (row_e == 7)))
		pawn_promote(start);

	update_board(barr, start, row_e, col_e);

	return 0;
}


int move_input(Square *barr[8][8], int player)
{
	setlocale(LC_CTYPE, "");

	__fpurge(stdin);

	if (player == 0) {
		wprintf(L"%s", "White's turn\n");
	} else {
		wprintf(L"%s", "Black's turn\n");
	}

	wprintf(L"%s", "Enter a move:\n");
	
	int *mvarr = parse_input();
	
	if (mvarr == NULL) {
		wprintf(L"%s", "Invalid input, try again.\n");
		return move_input(barr, player);
	}
	
	//int result = Piece_move(mvarr, barr, player);
	Square *start = barr[mvarr[0]][mvarr[1]];
	Square *end = barr[mvarr[2]][mvarr[3]];
	int result = Piece_move(start, end, barr, player);
	free(mvarr);

	if (result == 1) {
		wprintf(L"%s", "Illegal move, try again.\n");
		return move_input(barr, player);
	}

	return 0;
}


int *parse_input()
{
	setlocale(LC_CTYPE, "");
	char move[6];
	fgets(move, 6, stdin);

	char *p = strtok(move, " ");	
	
	if (p == NULL) {
		return NULL;
	}

	strcpy(move, p);
	p = strtok(NULL, " ");

	if (p == NULL) {
		return NULL;
	}

	char move_end[6];
	strcpy(move_end, p);
	// wprintf(L"%s\n", move);
	// wprintf(L"%s\n", move_end);

	int *mv_start = validate_input(move);
	
	if (mv_start == NULL) {
		return NULL;
	}

	int *mv_end = validate_input(move_end);

	if (mv_end == NULL) {
		return NULL;
	}

	int *moves = malloc(4 * sizeof(int));
	assert(moves != NULL);

	memcpy(moves, mv_start, 2 * sizeof(int));
	memcpy(moves + 2, mv_end, 2 * sizeof(int));

	free(mv_start);
	free(mv_end);

	return moves;

}


int *validate_input(char *mv)
{
	setlocale(LC_CTYPE, "");
	
	char *end;

	strtol(mv, &end, 10); //search for first char that cannot be converted to number, so letter or space
	
	if (end != mv) { // first char should be a letter, so end and mv should both point to a letter
		return NULL;
	}

	int col = char_to_int(mv[0]); // set col equal to the letter's corresponding column on board

	if (col == -1) {
		return NULL;
	}

	memmove(&mv[0], &mv[1], strlen(mv)); // remove index 0 of mv, should be letter, now index 1 is index 0

	strtol(mv, &end, 10); //again search for characters not convertible to number

	if (*end != '\0') { // should reach end of string if a valid input for move
		return NULL;
	}

	int row = mv[0] - '0'; // now mv[0] should be number, convert the char to int
	row--;
	row = abs(row - 7); // real in game board numbered 0 to 7 with top left square being [0, 0] so need to convert value

	if ( row < 0 || row > 7) {
		return NULL;
	}

	// wprintf(L"%d\n", row);
	// wprintf(L"%d\n", col);

	// now put row and col variables in one array to be returned

	int *result = malloc(sizeof(int) * 2);
	assert(result != NULL);

	result[0] = row;
	result[1] = col;

	return result;
}


int char_to_int(char c)
{
	switch(c) {
		case 'a':
			return 0;
		case 'b':
			return 1;
		case 'c':
			return 2;
		case 'd':
			return 3;
		case 'e':
			return 4;
		case 'f':
			return 5;
		case 'g':
			return 6;
		case 'h':
			return 7;
		default:
			return -1;
	}
}

