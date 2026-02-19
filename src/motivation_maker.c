#include "motivation_maker.h"
#include "motivation_maker_struct.h"

moti_mak_t init_moti_mak() {
    int max_scan_count = 0;
    char buffer[127] = "";
    int buffer_real_len = 0;

    INIT_MOTIVATION(new_motive);
    
    while(new_motive.Replaced[max_scan_count] != STOPPER) {
        max_scan_count += 1;
    }
    new_motive.Replacer_str = malloc(sizeof(char*) * max_scan_count);
    new_motive.arg_count = max_scan_count;

    for (int i = 0; i < max_scan_count; i += 1) {
        printf( "Veuillez saisir un texte pour remplace %s : ", new_motive.Replaced[i]);
        fflush( stdout );
        fscanf(stdin, "%[^\n]", buffer);
        fgetc( stdin );
        buffer_real_len = strlen(buffer);
        new_motive.Replacer_str[i] = malloc(sizeof(char) * buffer_real_len + 1);
        strncpy(new_motive.Replacer_str[i], buffer, buffer_real_len);
        new_motive.Replacer_str[i][buffer_real_len] = '\0';
    }

    return new_motive;
}

void free_motive_content(moti_mak_t *motive)
{
    for (int i = 0; i < motive->arg_count; i += 1) {
        free(motive->Replacer_str[i]);
        
    }
    free(motive->Replacer_str);

}

