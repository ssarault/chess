#ifndef Trans_table_h
#define Trans_table_h

#define RAND15()    (rand() & 0x7fff)
#define TABLE_SIZE 2000

unsigned long long rand64();
void setup_zorbist_array();
unsigned long long get_zorbist_hash(Square *barr[8][8], int turn);
unsigned long long get_zorbist_hash(Square *barr[8][8], int turn);
int get_trans_hash(unsigned long long zorbist_hash);
int update_trans_table(unsigned long long zorbist_hash);

#endif
