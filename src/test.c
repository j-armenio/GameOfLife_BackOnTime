#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "test.h"

void nextGeneration(int **mat, Neighbor *neighbors, int n, int m) {
    int **nextMat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        nextMat[i] = (int *)malloc(m * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Obtenha os vizinhos vivos
            get_neighbors_alive(mat, i, j, n, m, neighbors);
            int aliveNeighbors = filter_alive_neighbors(neighbors);

            // Aplica as regras do jogo
            if (mat[i][j] == 1) { // Célula viva
                if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                    nextMat[i][j] = 0; // Morre
                } else {
                    nextMat[i][j] = 1; // Permanece viva
                }
            } else { // Célula morta
                if (aliveNeighbors == 3) {
                    nextMat[i][j] = 1; // Revive
                } else {
                    nextMat[i][j] = 0; // Permanece morta
                }
            }
        }
    }

    // Copie a próxima geração de volta para a matriz original
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat[i][j] = nextMat[i][j];
        }
        free(nextMat[i]); // Libere a memória da linha
    }
    free(nextMat); // Libere a memória da matriz}
}

void verifyCorrespondence(int** original_mat, int** mat, int n, int m) {

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(original_mat[i][j] == mat[i][j])
                printf("✅");
            else
                printf("❌");
        }
        printf("\n");
    }
}