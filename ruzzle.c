
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

int find_all(char **mat, char **scores, char *parola, int dim, List *moves) {

    int **used = init_int_matrix(dim);

    int size = 1;
    List *list_array;
    int answer;
    int index;
    int cindex_removing = strlen(parola) - 2;
    List elem, rm_item, starting_cell;
    WList paths = NULL;


    answer = find_word(mat, scores, parola, dim, moves, used);

    if (answer && cindex_removing>=0) {
        list_array = malloc(sizeof (struct node));
        do {
            starting_cell = *moves;
            list_array = realloc(list_array, size * sizeof (struct node));
            list_copy(*moves, &list_array[size - 1]);

            *moves = NULL;
            
            zero_fill_matrix(used, dim);
            elem = get_item(list_array[size - 1], cindex_removing + 1);
            used[elem->row][elem->col] = JOLLY;
            do {
                while (trova_parola_ricorsivo(mat, scores, parola, used, starting_cell->row, starting_cell->col, 1, dim, moves)) {
                    size++;
                    list_array = realloc(list_array, size * sizeof (struct node));
                    list_copy(*moves, &list_array[size - 1]);
                    elem = get_last_item(*moves);

                    *moves = NULL;

                    used[elem->row][elem->col] = JOLLY;
                    zero_fill_matrix_but_jolly(used, dim);
                }
                zero_fill_matrix(used, dim);
                for (index = 0; index < size; index++) {
                    rm_item = get_item(list_array[index], cindex_removing);
                    used[rm_item->row][rm_item->col] = (cindex_removing == 0) ? DELETED : JOLLY;
                }
                cindex_removing--;
            } while (cindex_removing >= 0);
            size++;
            cindex_removing = strlen(parola) - 2;
        } while (find_word(mat, scores, parola, dim, moves, used));

        int max_score = 0;
        List max = NULL,supp=NULL,list=NULL;
        for (index = 0; index < size - 1; index++) {
            /*print_list(get_item(list_array[index], 0));*/
            list = get_item(list_array[index], 0);
            if(get_word_score(list) > max_score){
                free_list(supp);
                supp = NULL;
                list_copy(list,&supp);
                max_score = get_word_score(list); 
            }
            prepend_wlist(&paths,get_item(list_array[index], 0));
        }
        free_list(*moves);
        *moves = NULL;
        list_copy(supp,moves);
        
        printf("Available paths:\n");
        print_wlist(paths);
        printf("\n");
        free_wlist(paths);
        /*
        for (index = 0; index < size - 1; index++) {
            free_list(get_item(list_array[index], 0));
        }
         * */
    }

    return answer;
}