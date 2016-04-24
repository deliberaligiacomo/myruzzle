
#include <stdio.h>

#include "setting.h"
#include "list_directive.h"

/**
 * METODO RICORSIVO
 * @param mat matrice con le lettere (il campo)
 * @param parola la stringa da ricercare
 * @param used la matrice dei caratteri già utilizzati
 * @param pr da dove comincia la parola (posizione riga)
 * @param pc da dove comincia la parola (posizione colonna)
 * @param last l'ultimo numero inserito nella matrice
 * @return 
 */
int trova_parola_ricorsivo(char **mat, char **scores, char *parola, int **used, int pr, int pc, int last, int dim, List *l) {
    if (parola[0] != '\0') {
        /* caso base*/
        int r, c; /* indice di riga e colonna*/
        int found = 0; /* indica se ho trovato la parola*/

        r = pr - 1;

        while (r <= pr + 1 && !found) {
            c = pc - 1;
            while (c <= pc + 1 && !found) {
                /*non sforare gli indici       */ /*char never used*/ /* char equal */
                if (r >= 0 && r < dim && c >= 0 && c < dim && !used[r][c] && mat[r][c] == parola[0]) {
                    used[r][c] = last + 1;

                    append(&(*l), r, c, scores[r][c], mat[r][c]); /*#######*/

                    found = trova_parola_ricorsivo(mat,scores, parola + 1, used, r, c, last + 1, dim, &(*l));
                    if (!found) {
                        used[r][c] = 0; /* backtrack */

                        delete_last(&(*l)); /*#######*/
                    }
                }
                c++;
            }
            r++;
        }
        return found;
    } else {
        /* sto cercando la parola vuota, trovata */
        /*printf("Lista:\n");
        print_list(l);*/
        return 1;
    }
}

/**
 * @param mat la matrice delle lettere
 * @param parola stringa da ricercare
 * @return 1 se parola presente in mat, 0a altrimenti
 */
int find_word(char **mat,char **scores, char *parola, int dim, List *moves) {

    int **used;
    int r, c;
    int answer = 0;

    /* inizializzo la matrice used */
    used = init_int_matrix(dim);
    
    /* scorro la matrice mat */
    r = 0;
    while (r < dim && !answer) {
        c = 0;
        while (c < dim && !answer) {
            /* assumo che parola abbia almeno un carattere */
            if (mat[r][c] == parola[0]) {
                
                append(&(*moves), r, c, scores[r][c], mat[r][c]); /*#######*/
                
                used[r][c] = 1;
                answer = trova_parola_ricorsivo(mat, scores, parola + 1, used, r, c, 1, dim, &(*moves));

                if (!answer){
                    used[r][c] = 0;
                    delete_last(&(*moves)); /*#######*/
                }
            }
            c++;
        }
        r++;
    }
    
    return answer;
}