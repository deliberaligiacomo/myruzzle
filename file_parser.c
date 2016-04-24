#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setting.h"

int parse_schema(char *schema_path, int *dim, char ***field, char ***scores) {
    FILE *file = fopen(schema_path, "r");

    if (!file) {
        printf("Errore apertura file. [file_parser.17]");
        return STATUS_FAIL;
    } else {
        char current_char;
        int i = 0;
        int end_line = 0;
        char cnv[2];
        int row, column;
        int field_ended = 0;

        while (fscanf(file, "%c", &current_char) && !(*dim)) {
            if (current_char != NEW_LINE && current_char != SPACE) {
                cnv[0] = current_char;
                cnv[1] = '\0';
                *dim = atoi(cnv);
            }
        }
        fclose(file);        
        
        file = fopen(schema_path, "r");


        *field = init_char_matrix(*dim);
        *scores = init_char_matrix(*dim);


        i = 0;
        row = -1;
        column = 0;
        while (fscanf(file, "%c", &current_char) && !feof(file)) {
            if (current_char != NEW_LINE && current_char != SPACE) {
                if (i != 0) {
                    /* dimension already found, begin chars */
                    if (!field_ended) {
                        (*field)[row][column] = current_char;
                    } else {
                        (*scores)[row][column] = current_char;
                    }
                    column++;
                    column = column % (*dim);
                }
                if (end_line % 4 == 0) {
                    /* line ended */
                    row++;
                    row = row % (*dim);
                }
                if (end_line == (*dim) * (*dim)) {
                    /* field ended, begin setting (score) */
                    field_ended = 1;
                }
                end_line++;
                i++;
            }
        }
        fclose(file);
        return STATUS_SUCCESS;
    }
}

int loop_dictionary(char *path, char *output, char **field, char **scores, int dim, List *moves) {
    FILE *file = fopen(path, "r");
    char cword[100];
    int present;
    int cword_score;

    /* to clean the previus version of the output file */
    FILE *file_out = fopen(output, "w");
    fclose(file_out);


    if (file)
        while (fscanf(file, "%s", cword) && !feof(file)) {
            upcase(cword);

            present = find_word(field, scores, cword, dim, &(*moves));


            if (present) {
                cword_score = get_word_score(*moves, scores);
                /*printf("\nCurrentWord: %s,[%d] present: %d", cword, cword_score, present);*/
                /*printf("\npath: ");*/
                save_on_file(output, *moves, scores);
                /*print_list(*moves);*/
            }/*else
                printf("\nWord: %s not present", cword);
            
            printf("\n");*/
            free_list(&(*moves));
            *moves = NULL;
        }

    fclose(file);
    return STATUS_SUCCESS;
}