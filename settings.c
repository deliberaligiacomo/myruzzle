#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "setting.h"

void throw(char msg[]) {
    printf("\n========= TROW =========\n");
    printf(">>\t%s\n", msg);
    printf("========================\n\n");
    assert(msg);
}

/* http://c-faq.com/aryptr/dynmuldimary.html */
char** init_char_matrix(int dim) {
    int i;
    char **matrix;

    matrix = malloc(dim * sizeof (char *));

    if (matrix) {
        for (i = 0; i < dim; i++)
            matrix[i] = malloc(dim * sizeof (char));

        return matrix;
    } else
        return NULL;
}

void zero_fill_matrix(int **matrix, int dim) {
    int i, j;
    for (i = dim - 1; i >= 0; i--)
        for (j = dim - 1; j >= 0; j--)
            if (matrix[i][j] != DELETED)
                matrix[i][j] = 0;
}

void zero_fill_matrix_but_jolly(int **matrix, int dim) {
    int i, j;
    for (i = dim - 1; i >= 0; i--)
        for (j = dim - 1; j >= 0; j--)
            if (matrix[i][j] != JOLLY)
                matrix[i][j] = 0;
}

/* http://c-faq.com/aryptr/dynmuldimary.html */
int** init_int_matrix(int dim) {
    int i, j;
    int **matrix;

    matrix = malloc(dim * sizeof (int *));

    if (matrix) {
        for (i = 0; i < dim; i++)
            matrix[i] = malloc(dim * sizeof (int));

        zero_fill_matrix(matrix, dim);

        for (i = 0; i < dim; i++)
            for (j = 0; j < dim; j++)
                matrix[i][j] = '\0';

        return matrix;
    } else
        return NULL;
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
        upcase(str + 1);
    }
}

void free_int_matrix(int **matrix, int dim) {
    int i;
    for (i = 0; i < dim; i++)
        free((void *) matrix[i]);
    free((void *) matrix);
}

void free_char_matrix(char **matrix, int dim) {
    int i;
    for (i = 0; i < dim; i++)
        free((void *) matrix[i]);
    free((void *) matrix);
}