/**
 * @brief struct containing informations relating to edits in the motivation content
 * 
 */
#ifndef MODIFY_MOTIVE_H
#define MODIFY_MOTIVE_H


#include <stdarg.h>
#include <string.h>
#include <stdio.h>

typedef struct motive_edit {
    char *ptr_to_content;
    char *ptr_to_edit; 
    char *replaced;
    char *replacer;
    int diff;
} motive_edit_t;

typedef struct edit_linked_list {
    motive_edit_t *edit;
    struct edit_linked_list *next;
    struct edit_linked_list *prev;
} edit_ll_t;

edit_ll_t *find_all_replaced_ll(char *content, char *replaced, char *replacer);
edit_ll_t *linked_list_concatenation(edit_ll_t *l1, edit_ll_t *l2);
edit_ll_t *find_all_motive_edits(char *content, moti_mak_t *motive);
char *insert_edit(char *destination, motive_edit_t *edit);
char *modify_motive(char *content, edit_ll_t *list_of_edits);
void free_edits(edit_ll_t *list_of_edits);

#endif