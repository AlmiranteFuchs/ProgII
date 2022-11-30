// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: Parsing and directory functions library Header.
//

#ifndef DIR_LIB_H
#define DIR_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../libs/string_lib.h"

// Returns a list of valid file names in a directory
L_String *parse_dir(char *dir_name);

// Returns a string with the entire file content
char *parse_file(char *filename, char *pathname);

// Returns a list of all values of a prop of a tag in a string
L_String *get_all_tags_value(char *file_content, char *tag, char *prop);

#endif