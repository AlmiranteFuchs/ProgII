// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: entities Header File

#ifndef ENTITIES_H
#define ENTITIES_H

#include "../libs/linked_lib.h"

// inumerate the types of data
typedef enum
{
    CONFERENCE,
    PUBLICATION
} data_type;

// Data object
typedef struct
{
    int id;
    char *c_name;
    char *c_code;
    int c_year;

    data_type c_type; // 0 = Conference, 1 = Publication

} abstract_data;

// Researcher object
typedef struct
{
    int id;
    char *name;

    int publications_count;
    int conferences_count;

} researcher;

typedef struct
{
    int id;
    int id_data;
    int id_researcher;

    data_type data_type;

} researcher_data;

// Data object, responsible to link the researcher with the data
typedef struct
{
    list_t *data_perid_db; // All periods
    list_t *data_conf_db;  // All conferences

    list_t *researcher_db; // All researchers

    list_t *researcher_data; // All researcher_data relationships

    int period_count;     // Number of periods
    int conference_count; // Number of conferences
    int researcher_count; // Number of researchers

    int cardinality;
} database;

// Database
database *create_database();                                            // Create a database
void delete_database(database *db);                                     // Delete a database and registry
int insert_data_database(database *db, abstract_data *data);            // Insert a data in the database
int insert_researcher_database(database *db, researcher *r);            // Insert a researcher in the database
int insert_researcher_data_database(database *db, researcher_data *rd); // Insert a researcher_data in the database

// Database consults
list_t *get_data_of_researcher_id(database *db, data_type data_type, int id_researcher); // Get all data from a researcher
list_t *get_researchers_of_data_id(database *db, data_type data_type, int id_data);      // Get all researchers from a data

list_t *filter_data_by_code(list_t *list, char *code); // Get all data from a code

researcher *get_researcher_by_id(database *db, int id_researcher);             // Get a researcher by id
abstract_data *get_data_by_id(database *db, data_type data_type, int id_data); // Get a data by id

researcher *get_researcher_by_name(database *db, char *name);                   // Get a researcher by name
abstract_data *get_data_by_name(database *db, data_type data_type, char *name); // Get a data by name

// Data
abstract_data *create_data(int id, char *c_name, char *c_code, int c_year, data_type c_type); // Create a data
void delete_data(abstract_data *data);                                                        // Delete a data

// Researcher
researcher *create_researcher(int id, char *name); // Create a researcher
void delete_researcher(researcher *researcher);    // Delete a researcher

// Researcher Data
researcher_data *create_relation(int id_data, int id_researcher, data_type data_type); // Create a researcher_data
void delete_relation(researcher_data *rd);                                             // Delete a researcher_data

#endif