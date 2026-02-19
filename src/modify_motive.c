#include "motivation_maker_struct.h"
#include "modify_motive_struct.h"

// search for all motive Key and replace by the replacers, they may be bigger or smaller, you have to resize the string accordingly

// not uselful but still good to have
int calc_diff(const char *word1, const char *word2)
{
    // replaced => pierre / replacer => cailloux
    // diff => +2 so the new allocation should have 2 more sizeof(char)
    return strlen(word2) - strlen(word1);
}

// change for linked list
motive_edit_t **find_all_edits(char *content, char *replaced, char *replacer)
{
    // find occurence of "replaced" and diff with replacer
    char *ptr = NULL;
    int offset = strlen(replaced);
    motive_edit_t **list_edits = NULL;
    int count_occurence = 0;
    motive_edit_t *single_edit = NULL;
    int content_size = strlen(content);
    char *temp_content = malloc(sizeof(char) * content_size);

    strcpy(temp_content, content);

    while ((ptr = strstr(temp_content, replaced)) != NULL) {
        count_occurence += 1;
        list_edits = realloc(list_edits, sizeof(motive_edit_t*) * count_occurence);
        if (list_edits == NULL) {
            return NULL;
        }
        single_edit = malloc(sizeof(motive_edit_t) * 1);
        if (single_edit == NULL) {
            return NULL;
        }
        single_edit->ptr_to_content=content;
        single_edit->ptr_to_edit=ptr;
        single_edit->replaced=replaced;
        single_edit->replacer=replacer;
        temp_content = ptr + offset;
    }

    return list_edits;
}


// Would be faster in mutli-threading
edit_ll_t *find_all_replaced_ll(char *content, char *replaced, char *replacer)
{
    // find occurence of "replaced" and diff with replacer

    //Linked list
    edit_ll_t *result = NULL;
    edit_ll_t *curr_link = NULL;
    edit_ll_t *next_link = NULL;

    // strstr => offset => strstr until null
    char *ptr = NULL;
    int offset = strlen(replaced);
    int count_occurence = 0;
    int diff = strlen(replacer) - strlen(replaced);

    motive_edit_t *single_edit = NULL;

    while ((ptr = strstr(content, replaced)) != NULL) {
        count_occurence += 1;
        single_edit = malloc(sizeof(motive_edit_t) * 1);
        if (single_edit == NULL) {
            return NULL;
        }
        single_edit->diff = diff;
        single_edit->ptr_to_content=content;
        single_edit->ptr_to_edit=ptr;
        single_edit->replaced=replaced;
        single_edit->replacer=replacer;
        content = ptr + offset;
        next_link = malloc(sizeof(edit_ll_t));
        if (next_link == NULL) {
            return NULL;
        }
        next_link->edit = single_edit;
        next_link->next = NULL;
        next_link->prev = NULL;

        if (count_occurence == 1) {
            result = next_link;
            curr_link = next_link;
        } else {
            curr_link->next = next_link;
            next_link->prev = curr_link;

            curr_link = next_link;
        }
    }
    return result;
}

// lost cycle due to data type and lisibility purposes
edit_ll_t *linked_list_concatenation(edit_ll_t *l1, edit_ll_t *l2) {
    edit_ll_t *l_temp = l1;

    while (l_temp->next != NULL) {
        if (l_temp->next == NULL) {
            break;
        }
        l_temp = l_temp->next;
    }
    l_temp->next = l2;

    return l1;
}

edit_ll_t *find_all_motive_edits(char *content, moti_mak_t *motive)
{
    // strat count occurences of motive keys and diff char count, this diff with give a new strlen, malloc that strlen
    edit_ll_t *edits = NULL;
    edit_ll_t *begin = NULL;
    edit_ll_t *next_edits = NULL;

    // not good O(n*(n-1)) => not counting find edits
    for (int i = 0; motive->Replaced[i] != STOPPER; i += 1) {
        printf("current search : %s => %s\n", motive->Replaced[i], motive->Replacer_str[i]);
        if (i == 0) {
            begin = find_all_replaced_ll(content, motive->Replaced[i], motive->Replacer_str[i]);
            edits = begin;
        } else {
            next_edits = find_all_replaced_ll(content, motive->Replaced[i], motive->Replacer_str[i]);
            edits = linked_list_concatenation(edits, next_edits);
        }
    }

    return begin;
}

// uses a buffer to divide a string into, then insert the edit, before reassmbling the string
char *insert_edit(char *destination, motive_edit_t *edit)
{
    char *ptr = strstr(destination, edit->replaced);
    char *buffer = malloc(sizeof(char) * strlen(destination));

    //copy overshoot in a buffer
    strcpy(buffer, ptr + strlen(edit->replaced));

    // overwrite raplaced with replacer
    strcpy(ptr, edit->replacer);
    
    // reimport overshoot in the destination string
    strncpy(ptr + strlen(edit->replacer), buffer, strlen(buffer));
    free(buffer);
    return destination;
}

/*
because sorting the linked list is troublesome,
we instead are going to create buffer on which we will stock the string after the edit

Also instead of going through the content in char by char we are going to
use the ptr_edits to offset directly to to right area.

The side effect is that we must strncpy to ptr_to_edit address
*/
char *modify_motive(char *content, edit_ll_t *list_of_edits)
{
    char *modified_motive = NULL;
    int new_size = strlen(content);
    motive_edit_t *curr_edit;
    char *read_cursor_ptr = content;
    char *write_cursor_ptr = NULL;
    int replacer_length = 0;

    int write_count = 0;

    for (edit_ll_t *i = list_of_edits; i != NULL; i = i->next) {
        new_size += i->edit->diff;
    }
    modified_motive = malloc(sizeof(char) * new_size + 1);
    if (modified_motive == NULL) {
        fprintf(stderr, "Malloc Error modified motive,\
            alloc size : %d vs original size : %ld",
            new_size, strlen(content));
        return NULL;
    }
    write_cursor_ptr = modified_motive;

    for (edit_ll_t *i = list_of_edits; i != NULL; i = i->next) {
        curr_edit = i->edit;

        printf("curr edit : replaced: %s, replacer : %s, diff : %d\n", curr_edit->replaced, curr_edit->replacer, curr_edit->diff);
        
        if (curr_edit->ptr_to_edit >= read_cursor_ptr) {
            replacer_length = strlen(curr_edit->replacer);
            write_count = curr_edit->ptr_to_edit - read_cursor_ptr;
    
            strncpy(write_cursor_ptr, read_cursor_ptr, write_count);
            write_cursor_ptr += write_count;
            strncpy(write_cursor_ptr, curr_edit->replacer, replacer_length);
            write_cursor_ptr += replacer_length;

            read_cursor_ptr = curr_edit->ptr_to_edit + strlen(curr_edit->replaced);
        } else {
            // insert an edit here, pain in the ass
            // question is, how can we find the point of insertion
            // knowing that there could be edits before and after...
            insert_edit(modified_motive, curr_edit);
        }
    }
    strcpy(write_cursor_ptr, read_cursor_ptr);
    modified_motive[new_size] = '\0';
    return modified_motive; 
}

void free_edits(edit_ll_t *list_of_edits)
{
    edit_ll_t *temp = NULL;

    while(list_of_edits != NULL)
    {
        temp = list_of_edits;
        list_of_edits = list_of_edits->next;
        free(temp);
    }
}