// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: Parsing and directory functions library.
//

#include "dir_lib.h"

/**
 * Implementations
 */

char *parse_file(char *filename, char *pathname)
{
    // Reading file
    FILE *file_ptr;

    //printf("Opening: %s/%s\n", filename, pathname);
    char *path = malloc(strlen(pathname) + strlen(filename) + 2);

    // If pathname
    if (strlen(pathname) > 0)
    {
        strcpy(path, pathname);

        // Concatenating the path
        // Check if the last char is a slash
        if (path[strlen(path) - 1] != '/')
        {
            strcat(path, "/");
        }
        strcat(path, filename);
    }
    else
    {
        strcpy(path, filename);
    }

    // Read file
    file_ptr = fopen(path, "r");
    if (file_ptr == NULL)
    {
        printf("Erro ao abrir o arquivo: %s\n", path);
        exit(0);
    }

    // Get size of file
    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    rewind(file_ptr);

    // Allocs array of char of file_size
    char *file_content = (char *)malloc(sizeof(char) * file_size);
    if (file_content == NULL)
    {
        printf("Erro ao alocar memória\n");
        exit(0);
    }

    // Read file to array
    fread(file_content, sizeof(char), file_size, file_ptr);

    // Close file
    fclose(file_ptr);

    free(path);
    return file_content;
}

L_String *parse_dir(char *dir_name)
{
    DIR *dir;
    struct dirent *lsdir;

    dir = opendir(dir_name);

    if (dir == NULL)
    {
        fprintf(stderr, "Erro ao abrir o diretório %s\n", dir_name);
        exit(1);
    }

    L_String *dir_names = str_create();
    while ((lsdir = readdir(dir)) != NULL)
    {

        if (lsdir->d_type == DT_REG)
        {
            str_push(lsdir->d_name, dir_names);
        }
        else
        {
            if (lsdir->d_type == DT_DIR && strcmp(lsdir->d_name, ".") != 0 && strcmp(lsdir->d_name, "..") != 0)
            {
                printf("DIR: %s\n", lsdir->d_name);

                // strcat fucks up the memory
                char *new_dir = malloc(sizeof(char) * (strlen(dir_name) + strlen(lsdir->d_name) + 2));
                strcpy(new_dir, dir_name);
                strcat(new_dir, "/");
                strcat(new_dir, lsdir->d_name);

                L_String *new_dir_names = parse_dir(new_dir);

                for (int i = 0; i < new_dir_names->pos; i++)
                {
                    // I hate strcat
                    char *test = malloc(sizeof(char) * (strlen(dir_name) + strlen(lsdir->d_name) + strlen(new_dir_names->str[i]) + 2));
                    strcpy(test, dir_name);
                    strcat(test, "/");
                    strcat(test, lsdir->d_name);
                    strcat(test, "/");
                    strcat(test, new_dir_names->str[i]);

                    str_push(test, dir_names);

                    free(test);
                }

                str_clear(new_dir_names);
                free(new_dir);
            }
        }
    }
    closedir(dir);
    return dir_names;
}

L_String *get_all_tags_value(char *file_content, char *tag, char *prop)
{
    // Copy of file_content ptr
    char *file_content_ptr = file_content;

    // Create a list of strings
    L_String *list = str_create();

    // While tag is found
    while ((file_content_ptr = strstr(file_content_ptr, tag)) != NULL)
    {
        // Search substring prop in tag
        char *prop_ptr = strstr(file_content_ptr, prop);
        if (prop_ptr == NULL)
        {
            printf("Não foi encontrado a propriedade\n");
            break;
        }

        // Offsets file_content after tag
        prop_ptr = prop_ptr + strlen(prop);

        // Removes '"' and = tag
        prop_ptr += 2;

        // Reads until '"'
        char *prop_value = malloc(sizeof(char) * 1);
        int i = 0;
        while (*prop_ptr != '"')
        {
            prop_value[i] = *prop_ptr;
            prop_ptr++;

            // Reallocs memory
            prop_value = realloc(prop_value, sizeof(char) * (i + 2));
            i++;
        }

        // Check if prop_value is in list already
        // if (!str_contains(prop_value, list))
        // {
        // Adds to list
        prop_value[i] = '\0';
        str_push(str_to_lower(prop_value), list);
        // }

        // Offsets file_content_ptr
        file_content_ptr = prop_ptr;

        free(prop_value);
    }

    return list;
}