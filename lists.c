#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "setting.h"

int get_letter_score(char letter) {
    int score = 0;
    switch (letter) {
        case 'J':
        case 'Q':
        case 'W':
        case 'X':
            score = 10;
            break;
        case 'G':
        case 'H':
        case 'Z':
            score = 8;
            break;
        case 'B':
        case 'D':
        case 'F':
        case 'K':
        case 'P':
        case 'V':
            score = 5;
            break;
        case 'Y':
            score = 4;
            break;
        case 'L':
        case 'M':
        case 'N':
        case 'U':
            score = 3;
            break;
        case 'C':
        case 'R':
        case 'S':
        case 'T':
            score = 2;
            break;
        case 'A':
        case 'E':
        case 'I':
        case 'O':
            score = 1;
            break;
    }
    return score;
}

int append(List *l, int row, int col, char extra, char letter) {
    if (*l == NULL) {
        *l = (List) malloc(sizeof (struct node));
        if (*l) {
            (*l)->next = NULL;
            (*l)->row = row;
            (*l)->col = col;
            (*l)->extra = extra;
            (*l)->letter = letter;
            (*l)->letter_score = get_letter_score(letter);
            return STATUS_SUCCESS;
        } else
            return STATUS_FAIL;
    } else
        return append(&((*l)->next), row, col, extra, letter);
}

int prepend(List *l, int row, int col, char extra, char letter) {
    List nuova_cella = (List) malloc(sizeof (struct node));
    if (nuova_cella) {
        nuova_cella->row = row;
        nuova_cella->col = col;
        nuova_cella->extra = extra;
        nuova_cella->next = *l;
        nuova_cella->letter = letter;
        nuova_cella->letter_score = get_letter_score(letter);
        *l = nuova_cella;
        return STATUS_SUCCESS;
    } else
        return STATUS_FAIL;
}

void print_list(List l) {
    if (l) {
        if (l->next == NULL)
            printf("(%d,%d)\n\n", l->row, l->col);
        else
            printf("(%d,%d)-->", l->row, l->col);
        print_list(l->next);
    }
}

void delete_last(List *l) {
    if ((*l && (*l)->next == NULL) || *l == NULL) {
        /*un solo elemento oppure nessuno */
        *l = NULL;
    } else {
        delete_last(&(*l)->next);
    }
}

void free_list(List list) {
    if (list) {
        free_list(list->next);
        free(list);
    }
}

int get_word_score(List l) {
    int double_word = 1;
    int triple_word = 1;

    int score = 0;

    while (l) {
        /*printf("Letter: %c, extra: %c, lscore: %d\n", l->letter, l->extra, get_letter_score(l->letter));*/
        switch (l->extra) {
            case DOUBLE_WORD:
                double_word++;
                score += get_letter_score(l->letter);
                break;
            case TRIPLE_WORD:
                triple_word++;
                score += get_letter_score(l->letter);
                break;
            case DOUBLE_LETTER:
                score += get_letter_score(l->letter)*2;
                break;
            case TRIPLE_LETTER:
                score += get_letter_score(l->letter)*3;
                break;
            default:
                score += get_letter_score(l->letter);
        }
        l = l->next;
    }

    score = score * ((double_word > 1) ? 2 : 1)*((triple_word > 1) ? 3 : 1);
    return score;
}

int save_on_file(char* output_path, List l, char **scores) {
    FILE *file = fopen(output_path, "a+"); /*append or create*/
    int score = 0;
    List copy = l;

    if (!file) {
        printf("Errore apertura file. [list.174]\n");
    } else {
        if (l)
            score = get_word_score(l);

        while (l) {
            fprintf(file, "%c", l->letter);
            l = l->next;
        }
        fprintf(file, " %d ", score);

        while (copy) {
            if (copy->next == NULL)
                fprintf(file, "(%d,%d)", copy->row, copy->col);
            else
                fprintf(file, "(%d,%d)->", copy->row, copy->col);
            copy = copy->next;
        }

        fprintf(file, "\n");
    }

    fclose(file);

    return STATUS_SUCCESS;
}

int get_size(List l) {
    if (l)
        return 1 + get_size(l->next);
    else
        return 0;
}

int prepend_wlist(WList *words_list, List current_word_list) {
    char *str;
    List pc = current_word_list;
    int i = 0;
    int size = get_size(current_word_list);
    
    str = (char*) malloc(sizeof (char)*size);
    while (pc) {
        str[i] = pc->letter;
        pc = pc->next;
        i++;
    }
    str[i] = '\0';

    WList nuova_cella = (WList) malloc(sizeof (struct wnode));
    if (nuova_cella) {
        nuova_cella->word = (char*)malloc(sizeof(char)*size);
        strcpy(nuova_cella->word, str);
        nuova_cella->score = get_word_score(current_word_list);
        nuova_cella->next = *words_list;
        *words_list = nuova_cella;
        return STATUS_SUCCESS;
    } else
        return STATUS_FAIL;
}

void print_wlist(WList l) {
    if (l) {
        printf("%s %d\n", l->word, l->score);
        print_wlist(l->next);
    }
}

List get_item(List l, int index){
    List element = NULL;
    int i=0;
    int found=0;
    while(l && !found){
        if(index==i){
            element = l;
            found = 1;
        }
        i++;
        l=l->next;
    }
    return element;
}

void list_copy(List source, List *dest){
    while(source){
        append(dest,source->row,source->col,source->extra,source->letter);
        source=source->next;
    }
}

WList get_max_wlist_score(WList wlist){
    int max=0;
    int wlmax = NULL;
    while(wlist){
        if(wlist->score > max){
            max = wlist->score;
            wlmax = wlist;
        }
        wlist = wlist->next;
    }
    return wlmax;
}