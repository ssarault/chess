#include "chess.h"

int turn_counter = 1;


int main()
{
	setlocale(LC_CTYPE, "");

	setup_zorbist_array();

	Square *board[8][8];
	
	setup_board(board, 8, 8);
	draw_board(board, 8, 8);

	int turn = 0; // 0 for white, 1 for black
	int game_over = 0;
	
	while (!game_over) {

		move_input(board, turn);
		draw_board(board, 8, 8);
		turn++;
		turn_counter++;

		if (turn > 1) {
			turn = 0;
		}

		Square *king_sq = find_king(board, turn);
		int check = in_check(board, turn, king_sq);
		int has_move = has_legal_move(board, turn, king_sq);

		if (!has_move) {
			
			game_over = 1;
			
			if (check) {
				//checkmate
				wprintf(L"%s", "Checkmate!\n");
				if (turn == 0)
					wprintf(L"%s", "Black wins!\n");
				else
					wprintf(L"%s", "White wins!\n");
			} else {
				//stalemate
				wprintf(L"%s", "Stalemate!\n");
			}
			
			break;
		}

		if (check) {
			//check
			wprintf(L"%s", "Check!\n");
		}

		unsigned long long hashed_position = get_zorbist_hash(board, turn);
		int three_move_rep = update_trans_table(hashed_position);

		if (three_move_rep) {
			game_over = 1;
			wprintf(L"%s", "Draw!\n");
			break;
		}

		int insufficient_material = check_material(board);
		
		if (insufficient_material) {
			game_over = 1;
			wprintf(L"%s", "Draw!\n");
			break;
		}
	}	

	wprintf(L"%s", "Game Over!\n");

	board_destroy(board);

	return 0;
}
