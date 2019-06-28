#include "chess.h"

int trans_table[TABLE_SIZE] = { 0 };

unsigned long long zorbist_table[8][8][12];
unsigned long long enpassant_table[8];
unsigned long long castle_table[4];
unsigned long long black_move;

unsigned long long rand64() {
    unsigned long long n = 0;
    n+=RAND15();
    n+=(((unsigned long long)RAND15()) << 15);
    n+=(((unsigned long long)RAND15()) << 30);
    n+=(((unsigned long long)RAND15()) << 45);
    n+=((((unsigned long long) RAND15())&15) << 60);
    return n;
}


void setup_zorbist_array()
{

	int i, j, k;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 12; k++) {
				zorbist_table[i][j][k] = rand64();
			}
		}
	}

	for (i = 0; i < 8; i++) {
		enpassant_table[i] = rand64();
	}

	for (i = 0; i < 4; i++) {
		castle_table[i] = rand64();
	}

	black_move = rand64();
}


unsigned long long get_zorbist_hash(Square *barr[8][8], int turn)
{
	unsigned long long encoded = 0;

	int i, j;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {

			if (barr[i][j]->piece != NULL) {

				if (barr[i][j]->piece->color) {
					
					if (barr[i][j]->piece->id == 6)
						encoded ^= zorbist_table[i][j][5];

					else if (barr[i][j]->piece->id == 5)
						encoded ^= zorbist_table[i][j][4];

					else if (barr[i][j]->piece->id == 4)
						encoded ^= zorbist_table[i][j][3];
				
					else if (barr[i][j]->piece->id == 3)
						encoded ^= zorbist_table[i][j][2];

					else if (barr[i][j]->piece->id == 2)
						encoded ^= zorbist_table[i][j][1];
		
					else
						encoded ^= zorbist_table[i][j][0];

				} else {
					
					if (barr[i][j]->piece->id == 6)
						encoded ^= zorbist_table[i][j][11];

					else if (barr[i][j]->piece->id == 5)
						encoded ^= zorbist_table[i][j][10];

					else if (barr[i][j]->piece->id == 4)
						encoded ^= zorbist_table[i][j][9];
				
					else if (barr[i][j]->piece->id == 3)
						encoded ^= zorbist_table[i][j][8];

					else if (barr[i][j]->piece->id == 2)
						encoded ^= zorbist_table[i][j][7];
		
					else
						encoded ^= zorbist_table[i][j][6];
				}
			}

			if ((i == 2) || (i == 5)) {
				if (barr[i][j]->enpassant != NULL)
					encoded ^= enpassant_table[j];
			}
		}
	}

	// check castle rights
	int castle_rights;

	if (barr[0][4]->piece != NULL && barr[0][4]->piece->id == 1 && barr[0][4]->piece->color == 1) {
		castle_rights = check_move_king_castle(barr[0][4], barr[0][2], barr, 1);
		if (castle_rights)
			encoded ^= castle_table[0];

		castle_rights = check_move_king_castle(barr[0][4], barr[0][6], barr, 1);
		if (castle_rights)
			encoded ^= castle_table[1];
	}

	if (barr[7][4]->piece != NULL && barr[7][4]->piece->id == 1 && barr[0][4]->piece->color == 0) {
		castle_rights = check_move_king_castle(barr[7][4], barr[7][2], barr, 0);
		if (castle_rights)
			encoded ^= castle_table[2];

		castle_rights = check_move_king_castle(barr[7][4], barr[7][6], barr, 0);
		if (castle_rights)
			encoded ^= castle_table[3];

	}
		
	// black to move

	if (!turn)
		encoded ^= black_move;

	return encoded;
}


int get_trans_hash(unsigned long long zorbist_hash)
{

	int hash = zorbist_hash % TABLE_SIZE;

	return hash;
}
	

int update_trans_table(unsigned long long zorbist_hash)
{
	int position = get_trans_hash(zorbist_hash);

	trans_table[position]++;

	if (trans_table[position] == 3)
		return 1;

	return 0;
}

