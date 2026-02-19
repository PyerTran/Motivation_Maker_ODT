#include "motivation_maker.h"

struct zip *unzip(char *filename) {
    //Open the ZIP archive
    int err = 0;
    struct zip *z = zip_open(filename, 0, &err);
    if (z == NULL) {
        fprintf(stderr, "Unzip Error type : %d\n", err);
    }
    return z;
}

char *get_content(struct zip *z, const char *sub_filename)
{
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, sub_filename, 0, &st);

    char *contents = malloc(sizeof(char)* st.size+1);
    contents[st.size] = '\0';

    //Read the compressed file
    struct zip_file *f = zip_fopen(z, sub_filename, 0);
    zip_fread(f, contents, st.size);
    zip_fclose(f);

    return contents;
}

void end_exploitation(struct zip *z, char *content)
{
    free(content);
    zip_close(z);
}

// parse through bit shift ?

int overwrite_in_zip_text(const char *content, const char *sub_filename, struct zip *z)
{
    size_t content_size = strlen(content);
    struct zip_source *content_as_source;
    int ret = 0;
    
    content_as_source = zip_source_buffer(z, content, content_size, 1);

    ret = zip_file_add(z, sub_filename, content_as_source, ZIP_FL_OVERWRITE);
    
    // if ret = -1 big trouble
    return ret;   
}