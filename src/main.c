#include "motivation_maker.h"
#include "motivation_maker_struct.h"
#include "unzip.h"
#include "modify_motive_struct.h"

char *formulate_copy_cmd(char *filename, char *new_directory)
{
    char *buffer = malloc(sizeof(char) * 127);

    sprintf(buffer, "cp -f %s ./%s/%s", filename, new_directory, filename);
    printf("%s\n", buffer);
    return buffer;
}

char *formulate_filepath(char *filename, char *new_directory)
{
    char *buffer = malloc(sizeof(char) * 127);

    sprintf(buffer, "./%s/%s", new_directory, filename);
    printf("%s\n", buffer);
    return buffer;
}

// exec => file
int main(int argc, char**argv)
{
    char *cpy_cmd = NULL;
    char *new_filename_path = NULL;

    struct zip *archive = NULL;
    char *content = NULL;
    char *modified_content = NULL;
    edit_ll_t *list_of_edits = NULL;

    if (argc < 2) {
        return -1;
    }
    cpy_cmd = formulate_copy_cmd(argv[1], BUILD_DIRECTORY);
    system(cpy_cmd);
    free(cpy_cmd);

    moti_mak_t motivation_maker = init_moti_mak();
    
    new_filename_path = formulate_filepath(argv[1], BUILD_DIRECTORY);
    //unzip ./build/filename return struct zip * here
    archive = unzip(new_filename_path);
    free(new_filename_path);
    // get the content
    content = get_content(archive, CONTENT_FILENAME);
    
    // modify motive && keep pointer consistent
    list_of_edits = find_all_motive_edits(content, &motivation_maker);
    modified_content = modify_motive(content, list_of_edits);
    // overwrite zip text
    overwrite_in_zip_text(modified_content, CONTENT_FILENAME, archive);
    // end_exploitation
    end_exploitation(archive, content);
    // free motive
    free_motive_content(&motivation_maker);
    free_edits(list_of_edits);

    return 0;
}