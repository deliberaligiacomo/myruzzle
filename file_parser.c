#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include "setting.h"

int parse_schema(char *schema_path, int *dim, char ***field, char ***scores) {
    /* the stream rappresenting the file indicated in path [read only]*/
    FILE *file = fopen(schema_path, "r");

    if (!file) {
        printf("Errore opening the file. [file_parser.11]");
        return STATUS_FAIL;
    } else {
        /* rappresents the current scanning char */
        char current_char;
        /* when a line is parsed */
        int end_line = 0;
        /* the row and column indexes */
        int row, column;
        /* indicates if the playing matrix is finished and begin scores matrix */
        int field_ended = 0;

        /* get the dim from the file */
        fscanf(file, " %d", dim);


        /* allocates in the heap the matrixes */
        *field = init_char_matrix(*dim);
        *scores = init_char_matrix(*dim);

        row = 0;
        column = 0;
        while (fscanf(file, " %c", &current_char) && !feof(file)) {
            if (current_char != NEW_LINE && current_char != SPACE) {

                /* dimension already found, begin chars */

                /* if !filed_ended the playing matrix is finisced and begins the scores one */
                if (!field_ended) {
                    (*field)[row][column] = current_char;
                } else {
                    (*scores)[row][column] = current_char;
                }
                column++;

                /* columns go form 0 to dim-1*/
                column = column % (*dim);
                if (column % *dim == 0) {
                    /* line ended */
                    row++;
                    /* rows go from 0 to dim-1 */
                    row = row % (*dim);
                }
                /* the matrix are sqared (same rows and cols),
                 so when end_line reach dim*dim the first matrix is finisced */
                if (end_line == ((*dim) * (*dim)) - 1) {
                    /* field ended, begin setting (scores) */
                    field_ended = 1;
                }
                end_line++;
            }
        }
        /* close the file */
        fclose(file);
        return STATUS_SUCCESS;
    }
}

int loop_dictionary(char *path, char *output, char **field, char **scores, int dim, List *moves) {
    /* the stream rappresenting the dictionary file indicated in path [read only]*/
    FILE *file = fopen(path, "r");
    /* the current word (suppose max length 100 chars)*/
    char cword[100];
    /* indicates if the current word is present in the matrix */
    int present;

    /* to clean the previus version of the output file [write, NOT append]*/
    FILE *file_out = fopen(output, "w");
    fclose(file_out);

    /* if the stream is valid (not null)*/
    if (file)
        /* while the file is not finished */
        while (fscanf(file, "%s", cword) && !feof(file)) {
            /* upcase current word */
            upcase(cword);
            /* check if it's present */
            present = find_all(field, scores, cword, dim, moves,NULL);
            if (present) {
                /* since the current word is present in the matrix, save it in the output file */
                save_on_file(output, *moves);
            }

            /* clean the moves list*/
            /*free_list(*moves); */
            *moves = NULL;
        }
    /* close the output file to prevent errors */
    fclose(file);
    return STATUS_SUCCESS;
}