
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
                if (r >= 0 && r < dim && c >= 0 && c < dim && !used[r][c] && mat[r][c] == parola[0] && used[r][c] != JOLLY && used[r][c] != DELETED) {
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
                if (mat[r][c] == parola[0] && used[r][c] != JOLLY && used[r][c] != DELETED) {
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

int find_all(char **mat, char **scores, char *parola, int dim, List *moves, WList *available_paths) {
    /* indicated which cell has been used */
    int **used = init_int_matrix(dim);
    /* list array thatcontains all the path of a single word */
    List *list_array;
    /* the size of the list_array */
    int size = 1;
    /* indicates if the given word is present in the playing matrix */
    int current_answer;
    /* the index used to loop inside list array */
    int index;
    /* the list array index to be removed */
    int cindex_removing;
    /* the last cell of the current word paths list */
    List elem;
    /* the cell to be removed */
    List rm_item;
    /* the starting point for the current word path */
    List starting_cell;
    /* contains all the paths of the given word */
    WList paths = NULL;
    /* the path with the max score*/
    List max_scored_list;
    /* used as support when looking for the highest score*/
    List support_list;

    /* the index that limits the list_array */
    int cword_index;
    /* used to return the corrent value */
    int org_answer;
    /* the max score of the given word */
    int max_score;


    cword_index = 0;
    /* penultimate cell */
    cindex_removing = strlen(parola) - 2;
    /* check if the word is present in the original matrix */
    current_answer = find_word(mat, scores, parola, dim, moves, used);

    /* the real answer is the first one */
    org_answer = current_answer;

    /* if al least 1 path is possible and thw word len > 1*/
    if (current_answer && cindex_removing >= 0) {
        /* malloc for the first path */
        list_array = malloc(sizeof (struct node));
        if (list_array) {
            do {
                /* set starting_cell as the first cell used to find this path */
                starting_cell = *moves;
                /* realloc the another path [2° cycle] */
                list_array = realloc(list_array, size * sizeof (struct node));
                /* if realloc not ok*/
                if (list_array == NULL) {
                    throw("Realloc failure -- ruzzle.c");
                }
                /* copy the current path to the last available cell in the list array */
                list_copy(*moves, &list_array[size - 1]);
                /* free up memory*/
                *moves = NULL;

                /* used matrix is zero filled (if not DELETED)*/
                zero_fill_matrix(used, dim);
                /* get the "cindex_removing + 1" th cell from the last path  */
                elem = get_item(list_array[size - 1], cindex_removing + 1);
                /* set this cell as used */
                used[elem->row][elem->col] = JOLLY;
                do {
                    /* while another path is available not using the previous marked cell */
                    while (trova_parola_ricorsivo(mat, scores, parola, used, starting_cell->row, starting_cell->col, 1, dim, moves)) {
                        /* another path has been found, size increases */
                        size++;
                        /* realloc the list array */
                        list_array = realloc(list_array, size * sizeof (struct node));
                        /* copy the new path inside the list array */
                        list_copy(*moves, &list_array[size - 1]);
                        /* get the last path element */
                        elem = get_last_item(*moves);
                        /* free up memory */
                        *moves = NULL;
                        /* set this cell as used */
                        used[elem->row][elem->col] = JOLLY;
                        /* fill used matrix with zeros in all the cell not marked as jolly */
                        zero_fill_matrix_but_jolly(used, dim);
                    }
                    /* fill used matrix with zeros in all the cell not marked as deleted */
                    zero_fill_matrix(used, dim);
                    /* loop the paths of the current word */
                    for (index = cword_index; index < size; index++) {
                        /* take a cell to delete (from the end to the start)*/
                        rm_item = get_item(list_array[index], cindex_removing);
                        /* one the word start is reached (cindex_removing==0) mark the cell as deleted (no other path can be done)*/
                        used[rm_item->row][rm_item->col] = (cindex_removing == 0) ? DELETED : JOLLY;
                    }
                    /* set the cell to remove to the prevous */
                    cindex_removing--;
                    /* while the removing cell is not the path start */
                } while (cindex_removing >= 0);
                /* all the paths have been found starting from the prevoous staring point  */
                size++;
                cindex_removing = strlen(parola) - 2;
                cword_index++;
                /* try from another starting point */
            } while (find_word(mat, scores, parola, dim, moves, used));

            max_score = 0;
            max_scored_list = NULL, support_list = NULL;
            /* all path have been found. Search the path with the max score*/
            for (index = 0; index < size - 1; index++) {
                support_list = get_item(list_array[index], 0);
                if (get_word_score(support_list) > max_score) {
                    free_list(max_scored_list);
                    max_scored_list = NULL;
                    list_copy(support_list, &max_scored_list);
                    max_score = get_word_score(support_list);
                }
                prepend_wlist(&paths, get_item(list_array[index], 0));
            }
            /* the caller reads from moves, so copty the max score path in this list */
            list_copy(max_scored_list, moves);

            /* for the caller */
            if(available_paths != NULL)
                *available_paths = paths;

            /*
            for (index = 0; index < size - 1; index++) {
                support_list = get_item(list_array[index], 0);
                free_list(support_list);
                support_list = NULL;
            }
            free(list_array);
            list_array = NULL;
             */
        } else {
            throw("MALLOC FAIL -- find_all");
        }
    }

    return org_answer;
}