

#ifndef SETTING_H
#include "list_directive.h"


#define SETTING_H

/* number of argumesnts passed to main() */
#define MODE_OFFLINE 4
#define MODE_INTERACTIVE 1

#define SPACE ' '
#define NEW_LINE '\n'

#define STATUS_FAIL 0
#define STATUS_SUCCESS 1

/* score settings */
#define DOUBLE_LETTER 'd'
#define TRIPLE_LETTER 't'
#define DOUBLE_WORD 'D'
#define TRIPLE_WORD 'T'

#define JOLLY -1
#define DELETED -13

void throw(char msg[]);

/**
 * Given the file path to the game schema, fill in the playing field matrix, the score matrix and their dimesnion
 * @param path the path to the scheme file
 * @param dim where to save the matrix's size
 * @param filed where allocate the matrix for the field
 * @param scores where allocate the matrix for letters score
 * @return STATE_SUCCESS (1) if all correct, STATE_ERROR (0) otherwise
 */
int parse_schema(char *path, int *dim, char ***filed, char ***scores);

/**
 * Given a dictionary, a field and the score matrix check which words in the dic are present in the playing field
 * @param dic_path the path to the dictionary
 * @param output_path the path where to save the output file
 * @param field the playing matrix (letters)
 * @param scores the matrix which contains letters' score
 * @param dim the dimension of the filed (and of the scores matrix)
 * @param moves a list for saving the path of the current found word
 * @return STATE_SUCCESS (1) if all correct, STATE_ERROR (0) otherwise
 */
int loop_dictionary(char *dic_path,char *output_path, char **field, char **scores, int dim, List *moves);

/**
 * Given a field, the score matrix , the searching word and a list the function poulates the list if the word can be found in the field
 * @param mat playing matrix
 * @param scores scores matrix
 * @param word the word to search for
 * @param dim the dimension of the two matrix
 * @param moves the list where to save the path
 * @return STATE_SUCCESS (1) if all correct, STATE_ERROR (0) otherwise
 */
int find_word(char **mat, char **scores, char *word, int dim, List *moves, int **used);

/**
 * Allocates dinamically a square matrix (dim x dim) in the Heap
 * @param dim the dimension of the sqare matrix you wanna create
 * @return the pointer to that matrix or null if malloc went wrong
 */
char** init_char_matrix(int dim);
int** init_int_matrix(int dim);

/**
 * Given a dinamyc allocated matrix the function free its memory
 * @param matrix the matrix to free
 * @param dim the dimension of the matrix to free
 */
void free_int_matrix(int **matrix, int dim);
void free_char_matrix(char **matrix, int dim);

/**
 * It fill a existing matrix with zeros
 * @param matrix the matrix to fill
 * @param dim the dimension of that matrix
 */
void zero_fill_matrix(int **matrix, int dim);

/**
 * It prints out the elems of a matrix
 * @param matrix the matrix 
 * @param dim the diemsnion of the matrix to print out
 */
void print_char_matrix(char **matrix, int dim);
void print_int_matrix(int **matrix, int dim);


/**
 * It upcase a string (Side-effect)
 * @param str the string to UPCASE
 */
void upcase(char *str);

void zero_fill_matrix_but_jolly(int **matrix, int dim);
int find_all(char **mat, char **scores, char *parola, int dim, List *moves, WList *availabel_paths);
#endif /* SETTING_H */

