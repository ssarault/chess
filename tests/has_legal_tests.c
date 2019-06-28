#include "chess.h"
#include "components.c"
#include "board.c"
#include "check_move.c"
#include "move.c"
#include "has_legal.c"
#include "minunit.h"

int turn_counter = 1;

void setup_board_for_test(Square *barr[8][8])
{
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			barr[i][j] = Square_create(0, 0, i, j, NULL, -1, 0);
		}
	}
}

void clear_position(Square *barr[8][8])
{
	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {

			if (barr[i][j]->piece != NULL) {
				free(barr[i][j]->piece);
				barr[i][j]->piece = NULL;
			}

		}
	}
}


char *test_check_material()
{  
	Square *board[8][8] = { NULL };
	setup_board_for_test(board); // makes empty board of squares

	// white king and knight vs black king
	//Piece *pos_arr[3];
	board[0][4]->piece = Piece_create(1, 1, 0, 4, 1);
	board[7][0]->piece = Piece_create(5, 1, 7, 0, 1);
	board[7][4]->piece = Piece_create(1, 0, 7, 4, 1);
	
	//setup_position(board, pos_arr, 3); //setups pieces
	
	int is_draw = check_material(board);
	
	clear_position(board); // frees pieces for board
	//free_position_arr(pos_arr, 3); // frees Position struct, don't need to free pieces as already freed
	if (is_draw != 1)
		board_destroy(board);

	mu_assert(is_draw == 1, "Failed to detect insufficient material for king & knight vs king.");

	// king with bishops on same color square vs same
	//Position *pos_arr2[6];
	board[0][0]->piece = Piece_create(4, 1, 0, 0, 1);
	board[0][2]->piece = Piece_create(4, 1, 0, 2, 1);
	board[0][3]->piece = Piece_create(1, 1, 0, 3, 1);
	board[7][0]->piece = Piece_create(4, 0, 7, 0, 1);
	board[7][2]->piece = Piece_create(4, 0, 7, 2, 1);
	board[7][3]->piece = Piece_create(1, 0, 7, 3, 1);
	
	//setup_position(board, pos_arr2, 6); //setups pieces

	is_draw = check_material(board);

	board_destroy(board); // frees squares and pieces for board
	//free_position_arr(pos_arr2, 6);

	mu_assert(is_draw == 1, "Failed to detect insufficient material for king + unlimited bishops same square vs same.");

	return NULL;
}


char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_check_material);

    return NULL;
}

RUN_TESTS(all_tests);
