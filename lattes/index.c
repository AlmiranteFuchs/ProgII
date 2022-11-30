// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: LATTES

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "libs/string_lib.h"
#include "libs/linked_lib.h"
#include "src/entities.h"
#include "utils/dir_lib.h"

void program_params(char **cvs, char **q_conf, char **q_period, int argc, char *argv[]);
void parse_cvs_to_data(database *db, char *cvs_file_content);
void treat_cvs_files(char *cvs, database *db);
void treat_data_files(char *filename, data_type data_type, database *db);
void treat_period_files(char *q_period, database *db);

// List functions
void group_data_by_code(list_t *data, char *code);

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
    printf("Processando Qualis Conference Files...");
    treat_data_files(q_conf, CONFERENCE, db);
    printf(" OK\n");

    // Treat period files
    printf("Processando Qualis Period Files...");
    treat_data_files(q_period, PUBLICATION, db);
    printf(" OK\n");

    // Treat cvs files
    printf("Processando CVs Files...");
    printf("\n");
    treat_cvs_files(cvs, db);
    printf(" OK\n\n");

    //
    // INITIAL SETUP OK
    //
    printf("-------------------# Data #---------------------\n\n");

    // 1) Apresentar a produção sumarizada do grupo por ordem de periódicos discriminando os estratos;
    list_t *researchers = NULL;
    list_t *group_periods = create_list();
    list_t *group_conferences = create_list();
    node_t *node = NULL;

    // Get all researchers
    researchers = db->researcher_db;

    // For each researcher, get researcher_data
    node = researchers->head;
    while (node != NULL)
    {
        researcher *r = (researcher *)(node)->data;

        list_t *periods = get_data_of_researcher_id(db, PUBLICATION, r->id);
        list_t *conferences = get_data_of_researcher_id(db, CONFERENCE, r->id);

        node_t *generic_node = NULL;

        // For each period, insert unique in group_periods
        generic_node = periods->head;
        while (generic_node != NULL)
        {
            abstract_data *p = (abstract_data *)(generic_node)->data;

            insert_list_unique(group_periods, p, p->id);

            generic_node = generic_node->next;
        }

        // For each conference, insert unique in group_conferences
        generic_node = conferences->head;
        while (generic_node != NULL)
        {
            abstract_data *c = (abstract_data *)(generic_node)->data;

            insert_list_unique(group_conferences, c, c->id);

            generic_node = generic_node->next;
        }

        node = node->next;
    }

    // Print stratum 
    printf("...::: Produção sumarizada do grupo por ordem de periódicos discriminando os estratos :::...\n\n");
    group_data_by_code(group_periods, "a1");
    printf("\n");
    group_data_by_code(group_periods, "a2");
    printf("\n");
    group_data_by_code(group_periods, "a3");
    printf("\n");
    group_data_by_code(group_periods, "a4");
    printf("\n");
    group_data_by_code(group_periods, "b1");
    printf("\n");
    group_data_by_code(group_periods, "b2");
    printf("\n");
    group_data_by_code(group_periods, "b3");
    printf("\n");
    group_data_by_code(group_periods, "b4");
    printf("\n");
    group_data_by_code(group_periods, "c");
    printf("\nNão qualificado: ");
    group_data_by_code(group_periods, "d");

    printf("\n\n...::: Produção sumarizada do grupo por ordem de conferências discriminando os estratos :::...\n\n");
    group_data_by_code(group_conferences, "a1");
    printf("\n");
    group_data_by_code(group_conferences, "a2");
    printf("\n");
    group_data_by_code(group_conferences, "a3");
    printf("\n");
    group_data_by_code(group_conferences, "a4");
    printf("\n");
    group_data_by_code(group_conferences, "b1");
    printf("\n");
    group_data_by_code(group_conferences, "b2");
    printf("\n");
    group_data_by_code(group_conferences, "b3");
    printf("\n");
    group_data_by_code(group_conferences, "b4");
    printf("\n");
    group_data_by_code(group_conferences, "c");
    printf("\nNão qualificado: ");
    group_data_by_code(group_conferences, "d");


    // TODO: free all data
    return 0;
}

// Print group periods by code and stratum
void group_data_by_code(list_t *data, char *code)
{
    // New list to store filtered periods
    list_t *filtered_group_periods = NULL;

    // Node for iteration
    node_t *node = NULL;

    // Copy periods to filtered_group_periods
    filtered_group_periods = copy_list(data);

    // Filter periods by property
    filter_data_by_props(filtered_group_periods, code, NULL, -1);

    char *code_upper = str_to_upper(code);
    printf("Estrato %s:\n", code_upper);

    // For each period, print
    node = filtered_group_periods->head;
    while (node != NULL)
    {
        abstract_data *p = (abstract_data *)(node)->data;

        printf("%s: %d\n", p->c_name, p->data_count);

        node = node->next;
    }

    free(code_upper);
    destroy_list(filtered_group_periods);
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

    // Periods
    for (int i = 0; i < periods->pos; i++)
    {
        // 1. Check if period exists in data base
        // 2. If exists get id and create a relation
        // 3. If not exists create a period and a relation

        // Check if period exists in data base
        abstract_data *period = get_data_by_name(db, PUBLICATION, periods->str[i]);
        if (period == NULL)
        {

            // Create a period
            int period_id = db->period_count;
            period = create_data(period_id, periods->str[i], "d", atoi(periods_year->str[i]), PUBLICATION, 0);

            // Data general count
            period->data_count = 1;

            // Create a relation between researcher and period
            researcher_data *res_period = create_relation(period_id, res_id, PUBLICATION);

            // Insert period in database
            insert_data_database(db, period);

            // Insert relation in database
            insert_researcher_data_database(db, res_period);
        }
        else
        {
            // Check if relation already exists, if exists increment count
            researcher_data *res_period = get_relation_by_id(db, res_id, period->id, PUBLICATION);

            if (res_period == NULL)
            {
                // Create a relation between researcher and period
                res_period = create_relation(period->id, res_id, PUBLICATION);

                // Edit period year
                period->c_year = atoi(periods_year->str[i]);

                // Insert relation in database
                insert_researcher_data_database(db, res_period);

                // Increment period count
                period->data_count++;
            }
            else
            {
                // Increment count only if data is not from qualis
                if (period->from_qualis == 0)
                {
                    res_period->data_count++;
                }
            }
        }
    }

    // Conferences
    for (int i = 0; i < conferences->pos; i++)
    {
        // 1. Check if conference exists in data base
        // 2. If exists get id and create a relation
        // 3. If not exists create a conference and a relation

        // Check if conference exists in data base
        abstract_data *conference = get_data_by_name(db, CONFERENCE, conferences->str[i]);
        if (conference == NULL)
        {
            // Create a conference
            int conference_id = db->conference_count;
            conference = create_data(conference_id, conferences->str[i], "d", atoi(conferences_year->str[i]), CONFERENCE, 0);

            // Create a relation between researcher and conference
            researcher_data *res_conference = create_relation(conference_id, res_id, CONFERENCE);

            // Insert conference in database
            insert_data_database(db, conference);

            // Insert relation in database
            insert_researcher_data_database(db, res_conference);
        }
        else
        {
            // Check if relation already exists, if exists increment count
            researcher_data *res_conference = get_relation_by_id(db, res_id, conference->id, CONFERENCE);

            if (res_conference == NULL)
            {
                // Create a relation between researcher and conference
                res_conference = create_relation(conference->id, res_id, CONFERENCE);

                // Edit conference year
                conference->c_year = atoi(conferences_year->str[i]);

                // Insert relation in database
                insert_researcher_data_database(db, res_conference);

                // Increment general data show up
                conference->data_count++;
            }
            else
            {
                // Increment count if data is not from qualis
                if (conference->from_qualis == 0)
                {
                    res_conference->data_count++;
                }
            }
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

        // Remove last 3 chars
        line[strlen(line) - 3] = '\0';

        // str to lower

        // Create new conference
        abstract_data *conf = create_data((data_type == CONFERENCE ? db->conference_count : db->period_count), str_to_lower(line), str_to_lower(code), 2022, data_type, 1);

        free(code);

        // Insert conference in database
        insert_data_database(db, conf);
    }

    // Free memory
    free(q_conf_file_content);
}