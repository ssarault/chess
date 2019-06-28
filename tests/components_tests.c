#include "chess.h"
#include "components.c"
#include "minunit.h"

Piece *piece;
Square *square;

char *test_piece_create()
{

	piece = Piece_create(1, 0, 0, 0, 0);
	mu_assert(piece != NULL, "Failed to create piece.");

	return NULL;
}


char *test_square_create()
{
	square = Square_create(0, 0, 0, 0, NULL, -1, 0);
	mu_assert(square != NULL, "Failed to create square.");

	return NULL;
}


char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_piece_create);
    mu_run_test(test_square_create);

    return NULL;
}

RUN_TESTS(all_tests);
