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
void print_data_by_code(list_t *data, char *code);
int count_data_of_researcher_with_id(database *db, char *code, data_type data_type, researcher *r);
int count_data_of_group_with_year(list_t *group_data, char *code, int year);

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

    // Principal get loop
    list_t *researchers = NULL;
    list_t *group_periods = create_list();
    list_t *group_conferences = create_list();
    node_t *node = NULL;

    list_t *years = create_list();

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
            abstract_data *p = (abstract_data *)(generic_node)->data; // This parses all data to abstract_data of researcher each loop

            insert_list_unique(group_periods, p, p->id); // All periods of all researchers, with no copies of data

            // Insert unique year
            insert_list_unique(years, NULL, p->c_year);

            generic_node = generic_node->next;
        }

        // For each conference, insert unique in group_conferences
        generic_node = conferences->head;
        while (generic_node != NULL)
        {
            abstract_data *c = (abstract_data *)(generic_node)->data; // This parses all data to abstract_data of researcher each loop

            insert_list_unique(group_conferences, c, c->id); // All conferences of all researchers, with no copies of data

            // Insert unique year
            insert_list_unique(years, NULL, c->c_year);

            generic_node = generic_node->next;
        }

        node = node->next;
    }

    // All possible data codes
    L_String *codes = str_create();
    str_push("a1", codes);
    str_push("a2", codes);
    str_push("a3", codes);
    str_push("a4", codes);
    str_push("b1", codes);
    str_push("b2", codes);
    str_push("b3", codes);
    str_push("b4", codes);
    str_push("c", codes);
    str_push("d", codes);

    // Print stratum Per
    printf("\n\n...::: Produção sumarizada do grupo por ordem de periódicos discriminando os estratos :::...\n\n");

    // For each code
    for (int i = 0; i < codes->pos; i++)
    {
        print_data_by_code(group_periods, codes->str[i]);
        printf("\n");
    }

    // Print stratum Conf
    printf("\n\n...::: Produção sumarizada do grupo por ordem de conferências discriminando os estratos :::...\n\n");

    // For each code
    for (int i = 0; i < codes->pos; i++)
    {
        print_data_by_code(group_conferences, codes->str[i]);
        printf("\n");
    }

    // Print stratum Per Researcher
    printf("\n\n...::: produção dos pesquisadores do grupo por ordem de autoria discriminando os estratos; Em periódicos. Em conferências :::...\n\n");

    // For each researcher
    node = researchers->head;

    while (node != NULL)
    {
        researcher *r = (researcher *)(node)->data;

        printf("Pesquisador: %s\n", r->name);
        printf(".:+-----------+-------------+:.\n");
        printf(" | Periódicos | Conferências |\n");
        printf(".:+-----------+-------------+:.\n");

        // For each code
        for (int i = 0; i < codes->pos; i++)
        {
            int count_per = count_data_of_researcher_with_id(db, codes->str[i], PUBLICATION, r);
            int count_conf = count_data_of_researcher_with_id(db, codes->str[i], CONFERENCE, r);

            char *code_upper = str_to_upper(codes->str[i]);
            // If code is one digit, add a space to align
            if (strlen(codes->str[i]) == 1)
            {
                printf(" | %s:  %d      |   %s:  %d      |\n", code_upper, count_per, code_upper, count_conf);
            }
            else
            {
                printf(" | %s: %d      |   %s: %d      |\n", code_upper, count_per, code_upper, count_conf);
            }

            free(code_upper);
        }

        printf(".:+------------+------------+:.\n\n");

        node = node->next;
    }
    // Print stratum Per year
    printf("\n\n...::: produção sumarizada do grupo por ano discriminando os estratos; Em periódicos; Em conferências :::...\n\n");

    // For each year, starting last to first
    node = years->tail;

    while (node != NULL)
    {
        int year = node->data_id;

        printf("Ano: %d\n", year);
        printf(".:+-----------+-------------+:.\n");
        printf(" | Periódicos | Conferências |\n");
        printf(".:+-----------+-------------+:.\n");

        // For each code
        for (int i = 0; i < codes->pos; i++)
        {
            int count_per = count_data_of_group_with_year(group_periods, codes->str[i], year);
            int count_conf = count_data_of_group_with_year(group_conferences, codes->str[i], year);

            char *code_upper = str_to_upper(codes->str[i]);
            // If code is one digit, add a space to align
            if (strlen(codes->str[i]) == 1)
            {
                printf(" | %s:  %d      |   %s:  %d      |\n", code_upper, count_per, code_upper, count_conf);
            }
            else
            {
                printf(" | %s: %d      |   %s: %d      |\n", code_upper, count_per, code_upper, count_conf);
            }

            free(code_upper);
        }

        printf(".:+------------+------------+:.\n\n");

        node = node->prev;
    }

    printf("\n\n...::: Listar aqueles periódicos e eventos classificados no nível C  :::...\n\n");
    printf("Periódicos no estrato C:\n");

    // Create copies of lists to not modify original
    list_t *group_periods_copy = copy_list(group_periods);
    list_t *group_conferences_copy = copy_list(group_conferences);

    // Filter
    filter_data_by_props(group_periods_copy, "c", NULL, -1);
    filter_data_by_props(group_conferences_copy, "c", NULL, -1);

    // Print
    node_t *node_periods = group_periods_copy->head;
    while (node_periods != NULL)
    {
        abstract_data *p = (abstract_data *)(node_periods)->data;
        printf("\t - %s\n", p->c_name);
        node_periods = node_periods->next;
    }

    printf("\nConferências no estrato C:\n");

    node_t *node_conferences = group_conferences_copy->head;
    while (node_conferences != NULL)
    {
        abstract_data *c = (abstract_data *)(node_conferences)->data;
        printf("\t - %s\n", c->c_name);
        node_conferences = node_conferences->next;
    }

    destroy_list(group_periods_copy);
    destroy_list(group_conferences_copy);

    printf("\n\n...::: Listar os periódicos e eventos não classificados :::...\n\n");

    printf("Periódicos não classificados:\n");
    group_periods_copy = copy_list(group_periods);
    group_conferences_copy = copy_list(group_conferences);

    // Filter
    filter_data_by_props(group_periods_copy, "d", NULL, -1);
    filter_data_by_props(group_conferences_copy, "d", NULL, -1);

    // Print
    node_periods = group_periods_copy->head;
    while (node_periods != NULL)
    {
        abstract_data *p = (abstract_data *)(node_periods)->data;
        printf("\t - %s\n", p->c_name);
        node_periods = node_periods->next;
    }

    printf("\nConferências não classificados:\n");

    node_conferences = group_conferences_copy->head;
    while (node_conferences != NULL)
    {
        abstract_data *c = (abstract_data *)(node_conferences)->data;
        printf("\t - %s\n", c->c_name);
        node_conferences = node_conferences->next;
    }
    printf("\n");

    // # FREE #
    destroy_list(group_periods_copy);
    destroy_list(group_conferences_copy);

    destroy_list(group_periods);
    destroy_list(group_conferences);

    destroy_list(years);
    str_clear(codes);

    delete_database(db);

    // TODO: free all data
    return 0;
}

// Print group periods by code and stratum
void print_data_by_code(list_t *data, char *code)
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

        printf("\t%s: %d\n", p->c_name, p->data_count);

        node = node->next;
    }

    free(code_upper);
    destroy_list(filtered_group_periods);
}

// Prints all data of a researcher by code
int count_data_of_researcher_with_id(database *db, char *code, data_type data_type, researcher *r)
{
    // New list to store filtered periods
    list_t *filtered_data = NULL;

    // Copy periods to filtered_data
    filtered_data = get_data_of_researcher_id(db, data_type, r->id);

    // Filter periods by property
    filter_data_by_props(filtered_data, code, NULL, -1);

    int code_total_data = filtered_data->size;

    destroy_list(filtered_data);

    return code_total_data;
}

// Prints all data by year
int count_data_of_group_with_year(list_t *group_data, char *code, int year)
{
    // New list to store filtered periods
    list_t *filtered_data = NULL;

    // Copy periods to filtered_data
    filtered_data = copy_list(group_data);

    // I've tried to make this modular but iterating over nods and removing them in the same time is a pain, so just filter one at a time
    // Filter periods by code
    filter_data_by_props(filtered_data, code, NULL, -1);

    // Filter periods by year
    filter_data_by_props(filtered_data, NULL, NULL, year);

    int year_total_data = filtered_data->size;

    destroy_list(filtered_data);

    return year_total_data;
}

// Print data by code and stratum of researcher

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
