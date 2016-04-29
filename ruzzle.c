
#include <stdio.h>
#include <string.h>

#include "setting.h"
#include "list_directive.h"

/**
 * Loop though the playing field to determinate the path made to build the given word 
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
                    found = trova_parola_ricorsivo(mat, scores, parola + 1, used, r, c, last + 1, dim, &(*l));
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
int find_word(char **mat, char **scores, char *parola, int dim, List *moves, int **used) {
    /* indicates the cells already used */

    /* row and column indexes */
    int r, c;
    /* rappresents if current word has been found or not */
    int answer = 0;

    /* initialize used */

    if (used) {
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
                    if (!answer) {
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
    } else
        return -1;
}

int find_all(char **mat, char **scores, char *parola, int dim, List *moves) {
    int **used;
    int removing_index;
    int answer;

    int tries = 0;
    int exit_confirm = 0;

    int firstcycle = 1, firstanswer = 0;

    List elem = NULL;
    List copy = NULL;
    WList paths = NULL;
    used = init_int_matrix(dim);

    int i = 0;

    do {
        answer = find_word(mat, scores, parola, dim, moves, used);

        if (firstcycle && answer) {
            firstanswer = 1;
            firstcycle = 0;
        }



        if (answer) {
            prepend_wlist(&paths, *moves);
            removing_index = strlen(parola) - 1;
            elem = get_item(*moves, removing_index);
            used[elem->row][elem->col] = JOLLY;
            zero_fill_matrix_but_jolly(used, dim);

            list_copy(*moves, &copy);

        } else {
            removing_index--;
            elem = get_item(copy, removing_index);
            zero_fill_matrix(used, dim);
            used[elem->row][elem->col] = JOLLY;

            free_list(copy);
            copy = NULL;

            tries++;
            if (tries > 100)
                exit_confirm = 1;

        }
        *moves = NULL;


    } while (!exit_confirm && i++ < 10);
    printf("Print paths:\n");
    print_wlist(paths);
    
    WList max = get_max_wlist_score(paths);
    
    printf("MAX SCORE: %d\n", max->score);
    
    return firstanswer;
}