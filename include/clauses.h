#ifndef _CLAUSES_
#define _CLAUSES_
#include "utils.h"

#define K_LONELINESS 7
#define K_STAGNATION 2
#define K_OVERCROWDING 4
#define K_PRESERVATION 2
#define K_LIFE 3

#define TOP_WEIGHT 10

void process_clauses(char *clauses_file, int **mat, int n, int m, Neighbor *neighbors);

#endif