#ifndef UNZIP_H
#define UNZIP_H

#include <zip.h>
#include "motivation_maker.h"

struct zip *unzip(char *filename);
char *get_content(struct zip *z, const char *sub_filename);
void end_exploitation(struct zip *z, char *content);
int overwrite_in_zip_text(const char *content, const char *sub_filename, struct zip *z);

#endif