#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "clauses.h"
#include "test.h"

int main(int argc, char **argv) {
    // rows, cols
    int n, m;

    // Le a matriz do arquivo de entrada
    int **mat;
    if (argc >= 3)
        mat = read_input(argv[2], &n, &m);
    else 
        mat = read_input(argv[1], &n, &m);

    int **original_mat = copy_matrix(mat, n, m);

    Neighbor neighbors[NUM_NEIGHBORS];

    printf("INPUT %dx%d\n", n, m);
    print_matrix(mat, n, m, 0);
    printf("\n");

    // Processa todas as clausulas com base no "input.txt" e coloca em "clauses_file.cnf"
    process_clauses("solution/clauses.wcnf", mat, n, m, neighbors);
    free_matrix(mat, n);
    
    // === Busca solução SAT exaustiva ===
    // findBestSolution("solution/output.cnf", "solution.txt");

    // === Busca solução por MaxSAT ===
    system("./open-wbo -cpu-lim=300 -algorithm=3 -verbosity=0 solution/clauses.wcnf > solution/solution.txt");
    filter_solution("solution/solution.txt");

    int *solution_status = NULL;
    mat = read_solution("solution/solution.txt", n, m, solution_status);

    printf("Solution\n");
    
    int n_alive_cells;
    for (int i=0; i < n; i++){
        for (int j=0; j < m; j++){
            if (mat[i][j] == 1)
                n_alive_cells++;
        }
    }
    printf("%d alive cells.\n", n_alive_cells);

    print_matrix(mat, n, m, 0);
    printf("\n");

    // Verifica se a resposta é válida gerando próximo estado
    if (argc >= 3) {
        if (strcmp(argv[1], "-verify") == 0) {
            nextGeneration(mat, neighbors, n, m);

            printf("Solution matrix next step\n");
            print_matrix(mat, n, m, 0);
            printf("\n");

            printf("Cell by cell verification\n");
            verifyCorrespondence(original_mat, mat, n, m);
            printf("\n");
        }
    }

    return 0;
}
