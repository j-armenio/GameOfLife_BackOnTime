#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "utils.h"

// Função para ler o arquivo e criar a matriz
int **read_input(const char *filename, int *rows, int *cols) 
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Ler as dimensões da matriz
    if (fscanf(file, "%d %d", rows, cols) != 2) {
        fprintf(stderr, "Erro ao ler as dimensões da matriz\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Alocar memória para a matriz
    int **matrix = malloc((*rows) * sizeof(int *));
    if (matrix == NULL) {
        perror("Erro ao alocar memória para as linhas da matriz");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *rows; i++) {
        matrix[i] = malloc((*cols) * sizeof(int));
        if (matrix[i] == NULL) {
            perror("Erro ao alocar memória para as colunas da matriz");
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    // Ler os valores da matriz
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Erro ao ler o conteúdo da matriz\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
    return matrix;
}

// Libera memória da matriz
void free_matrix(int **matrix, int rows) 
{
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Função para imprimir a matriz (apenas para teste)
void print_matrix(int **matrix, int rows, int cols, int flag)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (flag)
                matrix[i][j] == 1 ? printf("⬜") : printf("⬛");
            else
                printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void get_neighbors_ids(int **dimacs_map, int i, int j, int rows, int cols, Neighbor *neighbors) {
    int index = 0;

    // Verifica os vizinhos superiores
    if (i > 0 && j > 0) neighbors[index++].id = dimacs_map[i - 1][j - 1]; // Superior esquerdo
    if (i > 0) neighbors[index++].id = dimacs_map[i - 1][j];              // Superior
    if (i > 0 && j < cols - 1) neighbors[index++].id = dimacs_map[i - 1][j + 1]; // Superior direito

    // Verifica os vizinhos laterais
    if (j > 0) neighbors[index++].id = dimacs_map[i][j - 1];              // Esquerda
    if (j < cols - 1) neighbors[index++].id = dimacs_map[i][j + 1];       // Direita

    // Verifica os vizinhos inferiores
    if (i < rows - 1 && j > 0) neighbors[index++].id = dimacs_map[i + 1][j - 1]; // Inferior esquerdo
    if (i < rows - 1) neighbors[index++].id = dimacs_map[i + 1][j];              // Inferior
    if (i < rows - 1 && j < cols - 1) neighbors[index++].id = dimacs_map[i + 1][j + 1]; // Inferior direito

    // Preenche IDs restantes como 0
    while (index < 8) {
        neighbors[index++].id = 0;
    }
}

void get_neighbors_alive(int **mat, int i, int j, int rows, int cols, Neighbor *neighbors) {
    int index = 0;

    // Verifica os vizinhos superiores
    if (i > 0 && j > 0) neighbors[index++].alive = mat[i - 1][j - 1]; // Superior esquerdo
    if (i > 0) neighbors[index++].alive = mat[i - 1][j];              // Superior
    if (i > 0 && j < cols - 1) neighbors[index++].alive = mat[i - 1][j + 1]; // Superior direito

    // Verifica os vizinhos laterais
    if (j > 0) neighbors[index++].alive = mat[i][j - 1];              // Esquerda
    if (j < cols - 1) neighbors[index++].alive = mat[i][j + 1];       // Direita

    // Verifica os vizinhos inferiores
    if (i < rows - 1 && j > 0) neighbors[index++].alive = mat[i + 1][j - 1]; // Inferior esquerdo
    if (i < rows - 1) neighbors[index++].alive = mat[i + 1][j];              // Inferior
    if (i < rows - 1 && j < cols - 1) neighbors[index++].alive = mat[i + 1][j + 1]; // Inferior direito

    // Preenche estados restantes como 0
    while (index < 8) {
        neighbors[index++].alive = 0;
    }
}

// Retorna quantidade de vizinhos válidos de uma célula
int filter_valid_neighbors(Neighbor *neighbors, int *valid_neighbors)
{
    int count = 0;
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        if (neighbors[i].id != 0) // ignora vizinhos inválidos
            valid_neighbors[count++] = neighbors[i].id;
    }

    return count;
}

// Retorna quantidade de vizinhos vivos de uma célula
int filter_alive_neighbors(Neighbor *neighbors)
{
    int count = 0;
    for (int i = 0; i < NUM_NEIGHBORS; i++) {
        if (neighbors[i].alive != 0) // ignora vizinhos mortos
            count++;;
    }

    return count;
}

// Deixa o arquivo de solução com apenas a linhas da valoração das variáveis
void filter_solution(const char *solution_file) {
    FILE *solution = fopen(solution_file, "r");
    if (!solution) {
        printf("Erro ao abrir o arquivo de solução.\n");
        return;
    }
    FILE *temp = fopen("solution/temp.txt", "w");
    if (!temp) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(solution);
        return;
    }

    char buffer[3000];

    // Processa o arquivo, procurando pela linha que começa com 'v '
    while (fgets(buffer, sizeof(buffer), solution)) {
        // printf("%s", buffer);
        if (buffer[0] == 's' || buffer[0] == 'v') {
            fprintf(temp, "%s", buffer); // Escreve a linha no arquivo temporário
        }
    }

    fclose(solution);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    if (rename("solution/temp.txt", solution_file) != 0) {
        printf("Erro ao renomear o arquivo temporário.\n");
    }
}

// Le as variaveis em um arquivo solução e retorna uma matriz int equivalente
int **read_solution(const char *solution_file, int rows, int cols, int *solution_status)
{
    FILE *solution = fopen(solution_file, "r");
    if (!solution){
        printf("Erro ao abrir arquivo de solução.\n");
        return NULL;
    }

    int **mat = (int **)malloc(rows * sizeof(int *));
    for (int i=0; i < rows; i++) {
        mat[i] = (int *)malloc(cols * sizeof(int));
    }

    char buffer[3000];
    int var_count = 0;
    int total_vars = rows * cols;

    // Ignora linha "s SATISFIABLE"
    fgets(buffer, sizeof(buffer), solution);
    printf("%s\n", buffer);

    while (fgets(buffer, sizeof(buffer), solution)) {
        if (buffer[0] == 'v') {
            char *token = strtok(buffer + 2, " "); // ignora o 'v'
            while (token != NULL && var_count < total_vars) {
                int var = atoi(token);
                int i = var_count / cols; // calcula linhas
                int j = var_count % cols; // calcula colunas
                mat[i][j] = (var > 0) ? 1 : 0;
                var_count++;
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(solution);

    return mat;
}

int **copy_matrix(int** mat, int n, int m){
    int **original_mat = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        original_mat[i] = (int *)malloc(m * sizeof(int));
        memcpy(original_mat[i], mat[i], m * sizeof(int)); // Copia linha por linha
    }

    return original_mat;
}

void findBestSolution(const char* cnf_file, const char* sol_file) {
    char line[1024];
    int contLiveCells = 0;
    int lessCellsSol = INT_MAX;

    // Loop externo: continuamos enquanto não encontramos "u UNSATISFIABLE"
    while (1) {
        contLiveCells = 0;
        // *** Executa o comando Picosat para gerar uma nova solução ***
        system("picosat output.cnf > solution.txt");
        
        // Abrindo o arquivo CNF para leitura e escrita
        FILE *cnf = fopen(cnf_file, "r+");
        if (!cnf) {
            printf("Erro ao abrir arquivo CNF.\n");
            break;
        }

        // Abrindo o arquivo de solução para leitura
        FILE *solution = fopen(sol_file, "r");
        if (!solution) {
            printf("Erro ao abrir arquivo de solução.\n");
            break;
        }

        // Verifica se encontramos "s UNSATISFIABLE" e interrompe o loop
        if (strncmp(line, "s UNSATISFIABLE", 15) == 0) {
            printf("Solução UNSATISFIABLE encontrada. Terminando execução.\n");
            break;
        }

        fseek(cnf, 0, SEEK_SET);  // Vai para o início do arquivo CNF

        // Lê a primeira linha do arquivo CNF
        if (fgets(line, sizeof(line), cnf) != NULL) {
            int col3, col4;
            // Divide os valores da linha inicial em colunas
            sscanf(line, "p cnf %d %d", &col3, &col4);

            // Incrementa a quarta coluna
            col4 += 1;

            // Reescreve a primeira linha do arquivo CNF
            fseek(cnf, 0, SEEK_SET);  // Garante que estamos sobrescrevendo o início
            fprintf(cnf, "p cnf %d %d\n", col3, col4);
            fflush(cnf);  // Garante a gravação imediata no arquivo
        }

        // Move o ponteiro do arquivo CNF para o final para adicionar cláusulas
        fseek(cnf, 0, SEEK_END);

        // Processa as linhas seguintes do arquivo de solução (as que contêm as variáveis)
        while (fgets(line, sizeof(line), solution) != NULL) {
            if (line[0] == 'v') {
                // Processa cada número na linha
                char *token = strtok(line, " "); // Ignora o 'v' inicial
                token = strtok(NULL, " "); // Pega o primeiro número

                // Escreve as variáveis negadas até encontrar o número 0
                while (token != NULL) {
                    int var = atoi(token);  // Converte o número para inteiro

                    // Negamos a variável (se for positiva, torna-se negativa e vice-versa)
                    if (var > 0) {
                        contLiveCells++;
                        fprintf(cnf, "-%d ", var);
                    } else {
                        fprintf(cnf, "%d ", -var);
                    }

                    // Passa para o próximo número
                    token = strtok(NULL, " ");
                }

                // Fim da linha de saída
                fprintf(cnf, "\n");
            }
        }
        
        if(contLiveCells < lessCellsSol){
            lessCellsSol = contLiveCells;
            if((strncmp(line, "s UNSATISFIABLE", 15) != 0))
                system("cp solution.txt bestSol.txt");
        }

        fclose(solution); // Fecha o arquivo de solução ao final do processamento
        // Fecha o arquivo CNF
        fclose(cnf);
    }
}
