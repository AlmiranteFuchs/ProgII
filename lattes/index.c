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

L_String *get_all_tags_value(char *file_content, char *tag, char *prop);

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

    // Get all data from researcher
    researcher *r = get_researcher_by_id(db, 1);

    //Print the researcher
    printf("Researcher: %s\n", r->name);

    list_t *data = get_data_of_researcher_id(db, CONFERENCE, r->id);

    // Corvert to data
    node_t *node = data->head;

    // Print all data
    while (node != NULL)
    {
        abstract_data *data = (abstract_data *)node->data;
        printf("Data: %s", data->c_name);
        printf(" %s\n", data->c_code);
        node = node->next;
    }

    printf("Size: %d\n", data->size);

    // Print the data
    //print_list(data);
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
        prop_value[i] = '\0';

        // Adds to list
        str_push(prop_value, list);

        // Offsets file_content_ptr
        file_content_ptr = prop_ptr;
    }

    return list;
}

// Logic data + relations --> database
void parse_cvs_to_data(database *db, char *cvs_file_content)
{
    // Tags
    char *name_tag = "DADOS-GERAIS";
    char *name_prop = "NOME-COMPLETO";

    // Periods
    char *period_tag = "DETALHAMENTO-DO-ARTIGO"; // Revista...
    char *period_prop = "TITULO-DO-PERIODICO-OU-REVISTA";
    char *period_year_tag = "DADOS-BASICOS-DO-ARTIGO";
    char *period_year_prop = "ANO-DO-ARTIGO";

    // Conferences
    char *conference_tag = "DETALHAMENTO-DA-PARTICIPACAO-EM-CONGRESSO";
    char *conference_prop = "NOME-DO-EVENTO";
    char *conference_year_tag = "DADOS-BASICOS-DA-PARTICIPACAO-EM-CONGRESSO";
    char *conference_year_prop = "ANO";

    /***
     * Tag searching
     */

    // Search the name of the researcher
    L_String *researcher_list = get_all_tags_value(cvs_file_content, name_tag, name_prop);

    // Get just the first
    char *researcher_name = malloc(sizeof(char) * strlen(researcher_list->str[0]));
    strcpy(researcher_name, researcher_list->str[0]);

    // Clear researcher list
    str_clear(researcher_list);

    // New researcher
    int res_id = db->researcher_count;
    researcher *res = create_researcher(res_id, researcher_name);

    // Insert researcher in database
    insert_researcher_database(db, res);



    // TODO: Migrate this to function
    // Periods name list
    L_String *periods = get_all_tags_value(cvs_file_content, period_tag, period_prop);

    // Periods year list
    L_String *periods_year = get_all_tags_value(cvs_file_content, period_year_tag, period_year_prop);

    // Conferences name list
    L_String *conferences = get_all_tags_value(cvs_file_content, conference_tag, conference_prop);

    // Conferences year list
    L_String *conferences_year = get_all_tags_value(cvs_file_content, conference_year_tag, conference_year_prop);

    /***
     * Creating data
     */

    //Periods
    for (int i = 0; i < periods->pos; i++)
    {
        // 1. Check if period exists in data base
        // 2. If exists get id and create a relation
        // 3. If not exists create a period and a relation

        // Check if period exists in data base
        abstract_data* period = get_data_by_name(db, PUBLICATION, periods->str[i]);
        if(period == NULL)
        {
            // Create a period
            int period_id = db->period_count;
            period = create_data(period_id, periods->str[i], "D", atoi(periods_year->str[i]), PUBLICATION);

            // Create a relation between researcher and period
            researcher_data *res_period = create_relation(period_id, res_id, PUBLICATION);

            // Insert period in database
            insert_data_database(db, period);

            // Insert relation in database
            insert_researcher_data_database(db, res_period);
        }else{
            // Create a relation between researcher and period
            researcher_data *res_period = create_relation(period->id, res_id, PUBLICATION);

            // Edit period year
            period->c_year = atoi(periods_year->str[i]);

            // Insert relation in database
            insert_researcher_data_database(db, res_period);
        }
    }

    // Conferences
    for (int i = 0; i < conferences->pos; i++)
    {
        // 1. Check if conference exists in data base
        // 2. If exists get id and create a relation
        // 3. If not exists create a conference and a relation

        // Check if conference exists in data base
        abstract_data* conference = get_data_by_name(db, CONFERENCE, conferences->str[i]);
        if(conference == NULL)
        {
            // Create a conference
            int conference_id = db->conference_count;
            conference = create_data(conference_id, conferences->str[i], "D", atoi(conferences_year->str[i]), CONFERENCE);

            // Create a relation between researcher and conference
            researcher_data *res_conference = create_relation(conference_id, res_id, CONFERENCE);

            // Insert conference in database
            insert_data_database(db, conference);

            // Insert relation in database
            insert_researcher_data_database(db, res_conference);
        }else{
            // Create a relation between researcher and conference
            researcher_data *res_conference = create_relation(conference->id, res_id, CONFERENCE);

            // Edit conference year
            conference->c_year = atoi(conferences_year->str[i]);

            // Insert relation in database
            insert_researcher_data_database(db, res_conference);
        }
    }
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