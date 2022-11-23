// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: LATTES

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "libs/string_lib.h"
#include "libs/linked_lib.h"
#include "src/entities.h"

L_String *parse_dir(char *dir_name);
char *parse_file(char *filename, char *cvs);
char *get_tag_value(char *filename, char *tag);
void program_params(char **cvs, char **q_conf, char **q_period, int argc, char *argv[]);
void parse_cvs_to_data(database *db, char *cvs_file_content);
void treat_cvs_files(char *cvs, database *db);
void treat_data_files(char *filename, data_type data_type, database *db);
void treat_period_files(char *q_period, database *db);

int main(int argc, char *argv[])
{
    //./lattes -d <diretorio com os CVs> -c <arquivo com a lista Qualis Conf> -p <arquivo com a lista Qualis Periódicos>

    // Program parameters
    char *cvs = NULL;
    char *q_conf = NULL;
    char *q_period = NULL;

    // All data of application
    database *db = create_database();

    // Set the program params
    program_params(&cvs, &q_conf, &q_period, argc, argv);

    // Treat conference files
    treat_data_files(q_conf, CONFERENCE, db);

    // Treat period files
    treat_data_files(q_period, PUBLICATION, db);

    // Treat cvs files
    treat_cvs_files(cvs, db);

    // TODO: Free memory

    return 0;
}

// Parse the command line parameters
void program_params(char **cvs, char **q_conf, char **q_period, int argc, char *argv[])
{
    int option;

    opterr = 0;

    while ((option = getopt(argc, argv, "d:c:p:")) != -1)
    {
        switch (option)
        {
        case 'd': // option -a was set
            *cvs = optarg;
            break;
        case 'c': // option -b was set
            *q_conf = optarg;
            break;
        case 'p': // option -c was set with value
            *q_period = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s -a -b -c value\n", argv[0]);
            exit(1);
        }
    }

    // printf("flag_a = %s, flag_b = %s, value_c = %s\n", cvs, q_conf, q_period);

    // Param test
    if (cvs == NULL || q_conf == NULL || q_period == NULL)
    {

        fprintf(stderr, "Parâmetros insuficientes, uso: %s -d <diretorio com os CVs> -c <arquivo com a lista Qualis Conf> -p <arquivo com a lista Qualis Periódicos>\n", argv[0]);
        exit(1);
    }
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
    }

    closedir(dir);
    return dir_names;
}

// Reads file and sets to array
char *parse_file(char *filename, char *pathname)
{
    // Reading file
    FILE *file_ptr;

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

char *get_tag_value(char *file_content, char *tag)
{
    // Search substring
    // char *tag = "DETALHAMENTO-DO-ARTIGO";

    char *tag_ptr = strstr(file_content, tag);
    if (tag_ptr == NULL)
    {
        printf("Não foi encontrado a tag\n");
        return NULL;
    }

    // Offsets file_content after tag
    tag_ptr = tag_ptr + strlen(tag);

    // Removes '"' and = tag
    tag_ptr++;
    tag_ptr++;

    // Reads until '"'
    char *tag_value = malloc(sizeof(char) * 1);
    int i = 0;
    while (*tag_ptr != '"')
    {
        tag_value[i] = *tag_ptr;
        tag_ptr++;

        // Reallocs memory
        tag_value = realloc(tag_value, sizeof(char) * (i + 2));
        i++;
    }
    tag_value[i] = '\0';

    // Print size
    return tag_value;
}

// Logic data + relations --> database
void parse_cvs_to_data(database *db, char *cvs_file_content)
{
    // Tags
    char *name_tag = "DADOS-GERAIS NOME-COMPLETO";
    char* period_tag = "DETALHAMENTO-DO-ARTIGO TITULO-DO-PERIODICO-OU-REVISTA"; // Wrong
    char* conference_tag = "DADOS-BASICOS-DA-PARTICIPACAO-EM-CONGRESSO"; // Wrong


    // Search the name of the researcher
    char *name = get_tag_value(cvs_file_content, name_tag);

    // New researcher
    researcher *res = create_researcher(db->researcher_count, name);

    // Insert researcher in database
    insert_researcher_database(db, res);

    // TODO: Create researcher data
    // Periodic publications

    // Free memory
    free(name);
}

// Params to file --> Logic data
void treat_cvs_files(char *cvs, database *db)
{
    // File content of cv
    char *cvs_file_content = NULL;

    // String of all directory names of cvs files
    L_String *dir_names = parse_dir(cvs);

    // For all directories
    for (int i = 0; i < dir_names->pos; i++)
    {
        char *filename = dir_names->str[i];

        // Checks if the file is a .xml
        if (strstr(filename, ".xml") != NULL)
        {
            // Parse the file
            cvs_file_content = parse_file(filename, cvs);
            // Call logic to data
            parse_cvs_to_data(db, cvs_file_content);
        }
    }

    free(cvs_file_content);
    str_clear(dir_names);
}

// Params to file --> Logic ata
void treat_data_files(char *filename, data_type data_type, database *db)
{
    // File content of cv
    char *q_conf_file_content = NULL;

    // Parse the file
    q_conf_file_content = parse_file(filename, "");

    // Copy of file
    char *q_conf_file_content_copy = q_conf_file_content;

    // For each line
    char *line = NULL;
    while ((line = strtok(q_conf_file_content_copy, "\n")) != NULL)
    {
        // printf("%s\n", line);

        // Offsets q_conf_file_content after line
        q_conf_file_content_copy = q_conf_file_content_copy + strlen(line) + 1;

        // Save last 2 chars
        char *code = malloc(sizeof(char) * 3);
        code[0] = line[strlen(line) - 2];
        code[1] = line[strlen(line) - 1];
        code[2] = '\0';

        // Remove last 2 chars
        line[strlen(line) - 2] = '\0';

        // Create new conference
        abstract_data *conf = create_data(db->conference_count, line, code, 2022, data_type);

        // Insert conference in database
        insert_data_database(db, conf);
    }

    // Free memory
    free(q_conf_file_content);
}