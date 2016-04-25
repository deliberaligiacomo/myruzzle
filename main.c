#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "setting.h"
#include "list_directive.h"

int main(int argc, char *argv[]) {

    /* if the arguments passed are exatly 3 */
    if (argc == MODE_OFFLINE) {
        /* the dictionary physical path */
        char *dictionary;
        /* the schema physical path */
        char *schema;
        /* the output file physical path (create)*/
        char *output;
        /* the playing field */
        char **field;
        /* the playing field score matrix */
        char **scores;
        /* the dimesnion of the playing field */
        int dim = 0;
        /* the list of moves made to find the word */
        List l = NULL;

        dictionary = argv[1];
        schema = argv[2];
        output = argv[3];

        /* fill vars schema, dim, field and scores */
        if (parse_schema(schema, &dim, &field, &scores)) {
            loop_dictionary(dictionary, output, field, scores, dim, &l);
            printf("Playground:\n");
            print_char_matrix(field, dim);
            printf("\nOutput file saved successfully!\n");
        } else
            printf("\nSomething went wrong while parsing the schema!\n");
    } else if (argc == MODE_INTERACTIVE) {
        /* the playing field */
        char **field;
        /* the playing field score matrix */
        char **scores;
        /* the dimesnion of the playing field */
        int dim = 0;
        /* the counters of the current row and column */
        int row, column;
        /* check if user wanna continue inserting words or not */
        int continua = 1;
        /* contain the current word (input by user) */
        char cword[100];
        /* contain the score of the current word */
        int cword_score = 0;
        /* the list of moves made to find the current word */
        List moves = NULL;
        /* indicates if current word is present in the playing field */
        int present;

        printf("Interactive mode. Create the field and define bonus.\n--------------\n");
        printf("Insert field dim: ");
        scanf("%d", &dim);

        printf("--------------\n");
        field = init_char_matrix(dim);
        scores = init_char_matrix(dim);

        /* loop though the matrix field to ask values */
        for (row = 0; row < dim; row++) {
            for (column = 0; column < dim; column++) {
                printf("Insert letter [%d][%d]: ", row, column);
                scanf(" %c", &field[row][column]);
                /* upcase char if neccessary*/
                if (field[row][column] >= 'a' && field[row][column] <= 'z')
                    field[row][column] = field[row][column] - 'a' + 'A';
            }
            printf("\n");
        }

        printf("--------------\nThe field you just created is:\n");
        print_char_matrix(field, dim);

        printf("--------------\nNow type bonus matrix:\n");
        /* loop trough the scores matrix and ask score for each cell */
        for (row = 0; row < dim; row++) {
            for (column = 0; column < dim; column++) {
                printf("Insert bonus for %c [%d][%d]: ", field[row][column], row, column);
                scanf(" %c", &scores[row][column]);
                /* validity check */
                if (scores[row][column] != DOUBLE_LETTER &&
                        scores[row][column] != TRIPLE_LETTER &&
                        scores[row][column] != DOUBLE_WORD &&
                        scores[row][column] != TRIPLE_WORD) {
                    scores[row][column] = '.';
                }

            }
            printf("\n");
        }

        printf("--------------\nThe bonus matrix you just created is:\n");
        print_char_matrix(scores, dim);
        printf("--------------\n");
        
        /* while the users insert valid words, continue */
        while (continua) {
            printf("Insert a word to check [type 0 to end]: ");
            scanf("%s", cword);

            /* if the typed word differ "0" */
            if (strcmp(cword, "0") != 0) {
                upcase(cword);
                present = find_word(field, scores, cword, dim, &moves);
                if (present) {
                    cword_score = get_word_score(moves, scores);
                    printf("Word %s present with score %d\n", cword, cword_score);
                    print_list(moves);
                    free_list(&moves);
                } else
                    printf("%s: not present\n\n", cword);
            } else
                /* the user typed "0", stop asking words */
                continua = 0;
        }
    } else {
        /* the given parameters are not correct (validity only checks pars number) */
        printf("Usage: /ruzzle [<dizionario> <schema> <output>]\n");
    }

    return EXIT_SUCCESS;
}

