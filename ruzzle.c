
#include <stdio.h>

#include "setting.h"
#include "list_directive.h"

/**
 * Loop though the playing field to determinate the path made to build the word given
 * @param mat the playing field
 * @param scores the bonus matrix
 * @param parola the current word to search
 * @param used the matrix rappresenting the cells alredy used
 * @param pr the index to the starting row point
 * @param pc the index to the starting column point
 * @param last the number used to highlit the path
 * @param dim the dimension of the square matrixes
 * @param l the list which cointians the moves
 * @return 1 if word found, 0 otherwise
 */
int trova_parola_ricorsivo(char **mat, char **scores, char *parola, int **used, int pr, int pc, int last, int dim, List *l) {
    /* main case */
    if (parola[0] != '\0') {
        /* local row and column indexes */
        int r, c;
        /* indicates if word found or not*/
        int found = 0;

        /* starting row point: the top row */
        r = pr - 1;
        while (r <= pr + 1 && !found) {
            /* starting column point: the left column*/
            c = pc - 1;
            while (c <= pc + 1 && !found) {
                /* still insede indexes AND cell never used AND current cell char equals the first of the current word  */
                if (r >= 0 && r < dim && c >= 0 && c < dim && !used[r][c] && mat[r][c] == parola[0]) {
                    /* could valid move, increment highlight*/
                    used[r][c] = last + 1;
                    /* append the path */
                    append(&(*l), r, c, scores[r][c], mat[r][c]);
                    /* try search with the next char */
                    found = trova_parola_ricorsivo(mat,scores, parola + 1, used, r, c, last + 1, dim, &(*l));
                    /* if not found, it wasn-t a valid path */
                    if (!found) {
                        /* backtrack */
                        used[r][c] = 0;
                        delete_last(&(*l));
                    }
                }
                c++;
            }
            r++;
        }
        return found;
    } else {
        /* looking for an empty word, found */
        return 1;
    }
}

/**
 * Search a word inside the playing field 
 * @param mat the playing field
 * @param scores the bonus matrix
 * @param parola the word to search
 * @param dim the dimension of the playing field (square matrix)
 * @param moves the list of the moves done to find the current word
 * @return 1 if word found, 0 otherwise
 */
int find_word(char **mat,char **scores, char *parola, int dim, List *moves) {

    /* indicates the cells already used */
    int **used;
    /* row and column indexes */
    int r, c;
    /* rappresents if current word has been found or not */
    int answer = 0;

    /* initialize used */
    used = init_int_matrix(dim);
    
    /* loop though mat */
    r = 0;
    while (r < dim && !answer) {
        c = 0;
        while (c < dim && !answer) {
            /* assumo che parola abbia almeno un carattere */
            if (mat[r][c] == parola[0]) {
                /* it could be a valid move, append the path */
                append(&(*moves), r, c, scores[r][c], mat[r][c]);
                /* set the current cell used */
                used[r][c] = 1;
                /* try to find the word from this cell */
                answer = trova_parola_ricorsivo(mat, scores, parola + 1, used, r, c, 1, dim, &(*moves));
                /* if not found */
                if (!answer){
                    /* this cell wasn't valid, backtrack */
                    used[r][c] = 0;
                    delete_last(&(*moves));
                }
            }
            c++;
        }
        r++;
    }
    
    return answer;
}