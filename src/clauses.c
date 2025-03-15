#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clauses.h"
#include "utils.h"

int generate_loneliness_clauses(int *neighbors, int k, int n, int *data, int start, int index, FILE *output)
{
    int clause_count = 0;

    if (index == k) { // base da recursão
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        // Subconjunto completo, escreva
        for (int i=0; i < k; i++) {
            fprintf(output, "%d ", data[i]);
        }
        fprintf(output, "0\n"); // termina a clausula
        return 1; // retorna 1 clausula gerada
    }

    // passo recursivo
    for (int i=start; i < n && n - i >= k - index; i++){
        data[index] = neighbors[i];
        clause_count += generate_loneliness_clauses(neighbors, k, n, data, i+1, index+1, output);
    }
    return clause_count;
}

int generate_stagnation_clauses(int x, int *neighbors, int k, int n, int *data, int start, int index, FILE *output)
{
    int clause_count = 0;

    if (index == k){
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        // x
        fprintf(output, "%d ", x);

        // nao(c)
        for (int i=0; i < k; i++){
            fprintf(output, "-%d ", data[i]);
        }

        // (n - c)
        for (int i=0; i < n; i++){
            int in_subset = 0;
            for (int j = 0; j < k; j++){
                if (neighbors[i] == data[j]){
                    in_subset = 1;
                    break;
                }
            }
            if (!in_subset){
                fprintf(output, "%d ", neighbors[i]);
            }
        }

        fprintf(output, "0\n");
        return 1;
    }

    for (int i = start; i < n && n - i >= k - index; i++){
        data[index] = neighbors[i];
        clause_count += generate_stagnation_clauses(x, neighbors, k, n, data, i+1, index+1, output);
    }

    return clause_count;
}

int generate_overcrowding_clauses(int *neighbors, int k, int n, int *data, int start, int index, FILE *output)
{
    int clause_count = 0;

    if (index == k){
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        for (int i=0; i < k; i++){
            fprintf(output, "-%d ", data[i]);
        }
        fprintf(output, "0\n");
        return 1;
    }

    for (int i=start; i < n && n - i >= k - index; i++){
        data[index] = neighbors[i];
        clause_count += generate_overcrowding_clauses(neighbors, k, n, data, i+1, index+1, output);
    }
    return clause_count;
}

int generate_preservation_clauses(int x, int *neighbors, int k, int n, int *data, int start, int index, FILE *output)
{
    int clause_count = 0;

    if (index == k){
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        // nao(x)
        fprintf(output, "-%d ", x);

        // nao(c)
        for (int i=0; i < k; i++){
            fprintf(output, "-%d ", data[i]);
        }

        // (n - c)
        for (int i=0; i < n; i++){
            int in_subset = 0;
            for (int j = 0; j < k; j++){
                if (neighbors[i] == data[j]){
                    in_subset = 1;
                    break;
                }
            }
            if (!in_subset){
                fprintf(output, "%d ", neighbors[i]);
            }
        }

        fprintf(output, "0\n");
        return 1;
    }

    for (int i = start; i < n && n - i >= k - index; i++){
        if (neighbors[i] != 0) {
            data[index] = neighbors[i];
            clause_count += generate_preservation_clauses(x, neighbors, k, n, data, i+1, index+1, output);
        }
    }

    return clause_count;
}

int generate_preservation_clauses_for_borders(int x, int *neighbors, int k, int *data, int valid_count, int start, int index, FILE *output) {
    int clause_count = 0;

    if (index == k) {
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        // nao(x)
        fprintf(output, "-%d ", x);

        // nao(c)
        for (int i = 0; i < k; i++) {
            fprintf(output, "-%d ", data[i]);
        }

        // (n - c)
        for (int i = 0; i < valid_count; i++) {
            int in_subset = 0;
            for (int j = 0; j < k; j++) {
                if (neighbors[i] == data[j]) {
                    in_subset = 1;
                    break;
                }
            }
            if (!in_subset) {
                fprintf(output, "%d ", neighbors[i]);
            }
        }

        fprintf(output, "0\n");
        return 1; // Retorna 1 cláusula gerada
    }

    for (int i = start; i < valid_count && valid_count - i >= k - index; i++) {
        data[index] = neighbors[i];
        clause_count += generate_preservation_clauses_for_borders(x, neighbors, k, data, valid_count, i + 1, index + 1, output);
    }

    return clause_count;
}

int generate_life_clauses(int *neighbors, int k, int n, int *data, int start, int index, FILE *output)
{
    int clause_count = 0;

    if (index == k){
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        // nao(c)
        for (int i=0; i < k; i++){
            fprintf(output, "-%d ", data[i]);
        }

        // (n - c)
        for (int i=0; i < n; i++){
            int in_subset = 0;
            for (int j = 0; j < k; j++){
                if (neighbors[i] == data[j]){
                    in_subset = 1;
                    break;
                }
            }
            if (!in_subset){
                fprintf(output, "%d ", neighbors[i]);
            }
        }

        fprintf(output, "0\n");
        return 1;
    }

    for (int i = start; i < n && n - i >= k - index; i++){
        if (neighbors[i] != 0) {
            data[index] = neighbors[i];
            clause_count += generate_life_clauses(neighbors, k, n, data, i+1, index+1, output);
        }
    }

    return clause_count;
}

int generate_life_clauses_for_borders(int *neighbors, int k, int *data, int valid_count, int start, int index, FILE *output) 
{
    int clause_count = 0;

    if (index == k) {
        // Peso da clausula
        fprintf(output, "%d ", TOP_WEIGHT);

        // nao(c)
        for (int i = 0; i < k; i++) {
            fprintf(output, "-%d ", data[i]);
        }

        // (n - c)
        for (int i = 0; i < valid_count; i++) {
            int in_subset = 0;
            for (int j = 0; j < k; j++) {
                if (neighbors[i] == data[j]) {
                    in_subset = 1;
                    break;
                }
            }
            if (!in_subset) {
                fprintf(output, "%d ", neighbors[i]);
            }
        }

        fprintf(output, "0\n");
        return 1; // Retorna 1 cláusula gerada
    }

    for (int i = start; i < valid_count && valid_count - i >= k - index; i++) {
        data[index] = neighbors[i];
        clause_count += generate_life_clauses_for_borders(neighbors, k, data, valid_count, i + 1, index + 1, output);
    }

    return clause_count;
}

void process_clauses(char *clauses_file, int **mat, int n, int m, Neighbor *neighbors)
{
    FILE *output = fopen(clauses_file, "w");
    if (output == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Cabeçalho DIMACS inicial (provisório)
    fprintf(output, "p wcnf 0 0 0\n");

    int data1[K_LONELINESS];
    int data2[K_STAGNATION];
    int data3[K_OVERCROWDING];
    int data4[K_PRESERVATION];
    int data5[K_LIFE];

    int neighbors_id[NUM_NEIGHBORS];
    int valid_neighbors[NUM_NEIGHBORS];
    int valid_count;
    int var_counter = 1;
    int clause_counter = 0;

    int **dimacs_map = (int **)malloc(n * sizeof(int *));
    for (int i=0; i < n; i++){
        dimacs_map[i] = (int *)malloc(m * sizeof(int));
        for (int j=0; j < m; j++){
            dimacs_map[i][j] = var_counter++; // Cada célula recebe um ID único
        }
    }

    // === PROCESSAMENTO DAS BORDAS ===

    // Processa primeira e ultima linha (bordas horizontais)
    for (int j = 0; j < m; j++) {
        // Primeira linha
        int x = dimacs_map[0][j];
        get_neighbors_ids(dimacs_map, 0, j, n, m, neighbors);
        valid_count = filter_valid_neighbors(neighbors, valid_neighbors);
        clause_counter += generate_preservation_clauses_for_borders(x, valid_neighbors, K_PRESERVATION, data4, valid_count, 0, 0, output);
        clause_counter += generate_life_clauses_for_borders(valid_neighbors, K_LIFE, data5, valid_count, 0, 0, output);

        // Última linha
        x = dimacs_map[n-1][j];
        get_neighbors_ids(dimacs_map, n-1, j, n, m, neighbors);
        valid_count = filter_valid_neighbors(neighbors, valid_neighbors);
        clause_counter += generate_preservation_clauses_for_borders(x, valid_neighbors, K_PRESERVATION, data4, valid_count, 0, 0, output);
        clause_counter += generate_life_clauses_for_borders(valid_neighbors, K_LIFE, data5, valid_count, 0, 0, output);
    }

    // Processa primeira e ultima coluna (bordas verticais)
    for (int i=1; i < n-1; i++){
        // Primeira coluna
        int x = dimacs_map[i][0];
        get_neighbors_ids(dimacs_map, i, 0, n, m, neighbors);
        valid_count = filter_valid_neighbors(neighbors, valid_neighbors);
        clause_counter += generate_preservation_clauses_for_borders(x, valid_neighbors, K_PRESERVATION, data4, valid_count, 0, 0, output);
        clause_counter += generate_life_clauses_for_borders(valid_neighbors, K_LIFE, data5, valid_count, 0, 0, output);

        // Última coluna
        x = dimacs_map[i][m - 1];        
        get_neighbors_ids(dimacs_map, i, m - 1, n, m, neighbors);
        valid_count = filter_valid_neighbors(neighbors, valid_neighbors);
        clause_counter += generate_preservation_clauses_for_borders(x, valid_neighbors, K_PRESERVATION, data4, valid_count, 0, 0, output);
        clause_counter += generate_life_clauses_for_borders(valid_neighbors, K_LIFE, data5, valid_count, 0, 0, output);
    }

    // === PROCESSAMENTO DO RESTO DAS CÉLULAS ===

    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j< m - 1; j++) {
            int x = dimacs_map[i][j]; // célula central

            get_neighbors_ids(dimacs_map, i, j, n, m, neighbors);
            for (int k = 0; k < NUM_NEIGHBORS; k++) {
                neighbors_id[k] = neighbors[k].id;
            }

            if (mat[i][j] == 1)
            { // célula Viva em t1: 1-3
                clause_counter += generate_loneliness_clauses(neighbors_id, K_LONELINESS, NUM_NEIGHBORS, data1, 0, 0, output);
                clause_counter += generate_stagnation_clauses(x, neighbors_id, K_STAGNATION, NUM_NEIGHBORS, data2, 0, 0, output);
                clause_counter += generate_overcrowding_clauses(neighbors_id, K_OVERCROWDING, NUM_NEIGHBORS, data3, 0, 0, output);
            }
            else if (mat[i][j] == 0)
            { // célula Morta em t1: 4-5
                clause_counter += generate_preservation_clauses(x, neighbors_id, K_PRESERVATION, NUM_NEIGHBORS, data4, 0, 0, output);
                clause_counter += generate_life_clauses(neighbors_id, K_LIFE, NUM_NEIGHBORS, data5, 0, 0, output);
            }
            else {
                printf("Algo deu errado.\n");
            }
        }
    }
    free_matrix(dimacs_map, n);

    // Adição das Soft clauses
    for (int i = 1; i < (n*m)+1; i++) {
        fprintf(output, "1 -%d 0\n", i);
        clause_counter++;
    }

    // Atualiza o cabeçalho

    fclose(output);

    output = fopen("solution/clauses.wcnf", "r+");
    FILE *temp = fopen("solution/temp.wcnf", "w");
    if (!output || !temp) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    // Escreve a nova primeira linha no arquivo temporário
    fprintf(temp, "p wcnf %d %d %d\n", var_counter - 1, clause_counter, TOP_WEIGHT);

    // Copia o restante do arquivo original para o temporário
    char buffer[1024];
    fgets(buffer, sizeof(buffer), output);  // Lê e ignora a primeira linha (já foi substituída)
    while (fgets(buffer, sizeof(buffer), output)) {
        fputs(buffer, temp); 
    }

    fclose(output);
    fclose(temp);
    rename("solution/temp.wcnf", "solution/clauses.wcnf");

    return;
}
