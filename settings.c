#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setting.h"

char** init_char_matrix(int dim) {
    int i, j;
    char **matrix;

    matrix = malloc(dim * sizeof (char *));
    matrix[0] = malloc(dim * dim * sizeof (char));
    for (i = 1; i < dim; i++)
        matrix[i] = matrix[0] + i * dim;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            matrix[i][j] = 'O';

    return matrix;
}

void zero_fill_matrix(int **matrix, int dim) {
    int i, j;
    for (i = dim - 1; i >= 0; i--)
        for (j = dim - 1; j >= 0; j--)
            matrix[i][j] = 0;
}

int** init_int_matrix(int dim) {
    int i, j;
    int **matrix;

    matrix = malloc(dim * sizeof (int *));
    matrix[0] = malloc(dim * dim * sizeof (int));
    for (i = 1; i < dim; i++)
        matrix[i] = matrix[0] + i * dim;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            matrix[i][j] = 0;

    return matrix;
}

void print_char_matrix(char **matrix, int dim) {
    int i, j;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_int_matrix(int **matrix, int dim) {
    int i, j;
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void upcase(char *str) {
    if (*str != '\0') {
        if (*str >= 'a' && *str <= 'z')
            *str -= ('a' - 'A');
        return upcase(str + 1);
    }
}

/*******************************************************
    TODO:
    void free_int_matrix(int **matrix, int dim);
    void free_char_matrix(char **matrix, int dim);
*******************************************************/