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
int search_tag(char *filename, char *tag);
void program_params(char **cvs, char **q_conf, char **q_period, int argc, char *argv[]);

int main(int argc, char *argv[])
{

    // Create DB
    database *db = create_database();

    // Create data
    abstract_data *data = create_data(db->conference_count, "Data", "Teste", 2022, CONFERENCE);
    insert_data_database(db, data);

    abstract_data *data2 = create_data(db->conference_count, "Data2", "Teste2", 2022, CONFERENCE);
    insert_data_database(db, data2);

    abstract_data *data3 = create_data(db->conference_count, "Data3", "Teste3", 2022, CONFERENCE);
    insert_data_database(db, data3);

    // Create Researcher
    researcher *res = create_researcher(db->researcher_count, "Nome do Pesquisador");
    researcher *res2 = create_researcher(db->researcher_count, "Nome do Pesquisador2");
    
    
    insert_researcher_database(db, res);
    insert_researcher_database(db, res2);


    // Create researcher_data
    researcher_data *res_data = create_relation(0, 0, CONFERENCE);
    researcher_data *res_data2 = create_relation(1, 0, CONFERENCE);
    researcher_data *res_data3 = create_relation(2, 1, CONFERENCE);

    // Insert researcher_data in DB
    insert_researcher_data_database(db, res_data);
    insert_researcher_data_database(db, res_data2);
    insert_researcher_data_database(db, res_data3);

    // Printf
    printf("data by name data3: %s\n", get_data_by_name(db, CONFERENCE, "Data3")->c_name);
    printf("Researcher by name Nome do Pesquisador: %s\n", get_researcher_by_name(db, "Nome do Pesquisador2")->name);


    return 1;

    // Create Researcher Data

    // char *cvs = NULL;
    // char *q_conf = NULL;
    // char *q_period = NULL;

    // // String of all directory names
    // L_String *dir_names = parse_dir(cvs);

    // // For all directories
    // for (int i = 0; i < dir_names->pos; i++)
    // {
    //     // String of all file names
    //     char *file_content = parse_file(dir_names->str[i], cvs);

    //     // Logic

    //     char *file_content = parse_file(dir_names->str[i], cvs);
    //             // Logic

    //     free(file_content);
    // }

    // // TODO: Free memory
    // str_clear(dir_names);
    // return 0;
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
char *parse_file(char *filename, char *cvs)
{
    // Reading file
    FILE *file_ptr;

    // Concatenating the path
    char *path = malloc(strlen(cvs) + strlen(filename) + 2);
    strcpy(path, cvs);
    strcat(path, "/");
    strcat(path, filename);

    // Read file
    file_ptr = fopen(path, "r");
    if (file_ptr == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
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

int search_tag(char *file_content, char *str_target)
{
    // Search substring
    char *tag = "DETALHAMENTO-DO-ARTIGO";

    printf("%s\n", str_target);
    return 1;

    char *tag_ptr = strstr(file_content, tag);
    if (tag_ptr == NULL)
    {
        printf("Não foi encontrado a tag\n");
        exit(0);
    }

    // Offsets file_content after tag
    tag_ptr = tag_ptr + strlen(tag);

    // Print content
    printf("%s\n", tag_ptr);

    return 0;
}
