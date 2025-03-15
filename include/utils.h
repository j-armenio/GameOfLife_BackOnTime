#ifndef _UTILS_
#define _UTILS_

#define NUM_NEIGHBORS 8

typedef struct
{
    int id;
    int alive;
}Neighbor;

int **read_input(const char *filename, int *rows, int *cols);
void free_matrix(int **matrix, int rows);
void print_matrix(int **matrix, int rows, int cols, int flag);
void get_neighbors_ids(int **dimacs_map, int i, int j, int rows, int cols, Neighbor *neighbors);
void get_neighbors_alive(int **mat, int i, int j, int rows, int cols, Neighbor *neighbors);
int filter_valid_neighbors(Neighbor *neighbors, int *valid_neighbors);
int filter_alive_neighbors(Neighbor *neighbors);
int **read_solution(const char *solution_file, int rows, int cols, int *solution_status);
void filter_solution(const char *solution_file);
int **copy_matrix(int** mat, int n, int m);
void findBestSolution(const char* cnf_file, const char* sol_file);

#endif