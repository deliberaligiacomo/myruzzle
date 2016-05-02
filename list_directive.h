#ifndef LIST_DIRECTIVE_H
#define LIST_DIRECTIVE_H

/* the node witch rappresents each char of a word found in the schema */
struct node{
    int row;
    int col;
    char extra;
    char letter;
    int letter_score;
    struct node * next;
};
/* A List structure that rappresnts a word, it score and it path into the playing field */
typedef struct node* List;

struct wnode{
    int score;
    char *word;
    char *path;
    struct wnode *next;
};
typedef struct wnode* WList;

/**
 * Append an element to the tail of a list (at the end)
 * @param l the list where append
 * @param row the row of the char just added
 * @param col the column of the char just added
 * @param extra the extra score of the char just added (score taken from scores matrix)
 * @param letter the char just added
 * @return STATE_SUCCESS (1) if append ok, STATE_ERROR (0) otherwise
 */
int append (List *l, int row, int col, char extra, char letter);

/**
 * Append an element before the head of a list
 * @param l the list where prepend
 * @param row the row of the char just added
 * @param col the column of the char just added
 * @param extra the extra score of the char just added (score taken from scores matrix)
 * @param letter the char just added
 * @return STATE_SUCCESS (1) if prepend ok, STATE_ERROR (0) otherwise
 */
int prepend(List *l, int row, int col, char extra, char letter);

/**
 * Delete the last node of a list
 * @param l the list where to delete the last node
 */
void delete_last(List *l);

/**
 * Print out all the elems of a list
 * @param l the list
 */
void print_list(List l);

/**
 * Free the memory taken by a list
 * @param l the list to free
 */
void free_list(List l);

/**
 * Given the list rappresening a word found in the playing field, return the score of that word considering the all bonus
 * @param l the list rappresentig the word
 * @param scores the score matrix where to find the bonus
 * @return the score of the given word
 */
int get_word_score(List l);

/**
 * Given the list rappresening a word found in the playing field, saves the word, the score and the path into a file
 * @param output_path the path where to save the output file
 * @param l the list rappresentig the word 
 * @param scores the score matrix where to find the bonus
 * @return STATE_SUCCESS (1) if prepend ok, STATE_ERROR (0) otherwise
 */
int save_on_file(char* output_path, List l, char **scores);

void print_wlist(WList l);
int prepend_wlist(WList *words_list, List current_word_list);
List get_last_item(List list);
void free_wlist(WList wlist);
void list_copy(List source, List *dest);
List get_item(List l, int index);
#endif /* LIST_DIRECTIVE_H */

