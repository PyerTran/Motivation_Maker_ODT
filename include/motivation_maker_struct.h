#ifndef MOTIVATION_MAKER_STRUCT_H
#define MOTIVATION_MAKER_STRUCT_H

#include <stdlib.h>

#define STOPPER NULL
#define JOB_TITLE "JOB_TITLE"
#define COMPANY_NAME "COMPANY_NAME"
#define SUBJECT_LIST "SUBJECT_LIST"

#define LIST_KEYS {JOB_TITLE, COMPANY_NAME, SUBJECT_LIST, STOPPER}

struct motivation_maker {
    int arg_count;
    char **Replacer_str;
    char *Replaced[4];
} ;

typedef struct motivation_maker moti_mak_t;

#define INIT_MOTIVATION(X) moti_mak_t X = {.Replaced=LIST_KEYS}

moti_mak_t init_moti_mak();
void free_motive_content(moti_mak_t *motive);

#endif