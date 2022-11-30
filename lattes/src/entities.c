// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: Entities are the objects that will be used in the logic
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entities.h"
#include "../libs/string_lib.h"
#include "../libs/linked_lib.h"

/**
 * Database functions
 */

database *create_database()
{
    // Create a database
    database *db = (database *)malloc(sizeof(database));

    // Create the lists
    db->data_perid_db = create_list();
    db->data_conf_db = create_list();
    db->researcher_db = create_list();
    db->researcher_data = create_list();

    // Set the cardinality
    db->cardinality = 0;

    return db;
}

void delete_database(database *db)
{
    // Destroy the lists
    destroy_list(db->data_perid_db);
    destroy_list(db->data_conf_db);
    destroy_list(db->researcher_db);
    destroy_list(db->researcher_data);

    // Destroy the database
    free(db);
}

int insert_data_database(database *db, abstract_data *data)
{
    // Check if the data is valid
    if (data == NULL)
    {
        return 0;
    }

    // TODO: Check if the researcher already exists

    // Check if the data is a conference
    if (data->c_type == CONFERENCE)
    {
        // Insert the data in the conference list
        insert_list(db->data_conf_db, data, data->id);
        db->conference_count++;
    }
    else
    {
        // Insert the data in the periodical list
        insert_list(db->data_perid_db, data, data->id);
        db->period_count++;
    }

    // Increment the cardinality
    db->cardinality++;

    return 1;
}

researcher_data *get_relation_by_id(database *db, int id_researcher, int id_data, data_type data_type){
    // Create a new node
    node_t *node = db->researcher_data->head;

    // Iterate over the list
    while (node != NULL)
    {
        // Get the researcher_data
        researcher_data *rd = (researcher_data *)node->data;

        // Check if the researcher_data is the one we are looking for
        if (rd->id_researcher == id_researcher && rd->id_data == id_data && rd->data_type == data_type)
        {
            return rd;
        }

        // Go to the next node
        node = node->next;
    }

    return NULL;
}

int insert_researcher_database(database *db, researcher *r)
{
    // Check if the researcher is valid
    if (r == NULL)
    {
        return 0;
    }

    // TODO: Check if the researcher already exists

    // Insert the researcher in the researcher list
    insert_list(db->researcher_db, r, r->id);
    db->researcher_count++;

    // Increment the cardinality
    db->cardinality++;

    return 1;
}

int insert_researcher_data_database(database *db, researcher_data *rd)
{
    // Check if the researcher_data is valid
    if (rd == NULL)
    {
        return 0;
    }

    // Validates the researcher
    researcher *r = get_researcher_by_id(db, rd->id_researcher);
    if (r == NULL)
    {
        return 0;
    }
    // Incremet the researcher count
    if (rd->data_type == CONFERENCE)
    {
        r->conferences_count++;
    }
    else
    {
        r->publications_count++;
    }

    // Validates the data
    abstract_data *data = get_data_by_id(db, rd->data_type, rd->id_data);
    if (data == NULL)
    {
        return 0;
    }

    // Insert the researcher_data in the researcher_data list
    insert_list(db->researcher_data, rd, rd->id);

    // Increment the cardinality
    db->cardinality++;

    return 1;
}

/**
 * Consult functions
 */

// Returns research by id
researcher *get_researcher_by_id(database *db, int id)
{
    // Get the researcher
    researcher *r = (researcher *)search_list(db->researcher_db, id)->data;

    // Check if the researcher exists
    if (r == NULL)
    {
        return NULL;
    }

    return r;
}

abstract_data *get_data_by_id(database *db, data_type data_type, int id)
{
    // Get the data

    if (data_type == CONFERENCE)
    {
        abstract_data *data = (abstract_data *)search_list(db->data_conf_db, id)->data;

        // Check if the data exists
        if (data == NULL)
        {
            return NULL;
        }

        return data;
    }
    else
    {
        abstract_data *data = (abstract_data *)search_list(db->data_perid_db, id)->data;

        // Check if the data exists
        if (data == NULL)
        {
            return NULL;
        }

        return data;
    }
}

list_t *get_data_of_researcher_id(database *db, data_type data_type, int id_researcher)
{
    // List of data
    list_t *data_of_researcher = create_list();

    // Get the researcher_data
    node_t *node = db->researcher_data->head;

    // Iterate over the researcher_data
    while (node != NULL)
    {
        // Get the researcher_data
        researcher_data *rd = (researcher_data *)node->data;

        // Check if the researcher_data is valid
        if (rd != NULL)
        {
            // Check if the researcher_data is of the researcher and data type specified
            if ((rd->id_researcher == id_researcher) && (rd->data_type == data_type))
            {
                // Get the data
                abstract_data *data = get_data_by_id(db, rd->data_type, rd->id_data);

                // Check if the data is valid
                if (data != NULL)
                {
                    // Insert the data in the list
                    insert_list(data_of_researcher, data, data->id);
                }
            }
        }

        // Get the next node
        node = node->next;
    }

    return data_of_researcher;
}

list_t *get_researchers_of_data_id(database *db, data_type data_type, int id_data)
{
    // List of researchers
    list_t *researchers_of_data = create_list();

    // Get the researcher_data
    node_t *node = db->researcher_data->head;

    // Iterate over the researcher_data
    while (node != NULL)
    {
        // Get the researcher_data
        researcher_data *rd = (researcher_data *)node->data;

        // Check if the researcher_data is valid
        if (rd != NULL)
        {
            // Check if the researcher_data is of the researcher and data type specified
            if ((rd->id_data == id_data) && (rd->data_type == data_type))
            {
                // Get the researcher
                researcher *r = get_researcher_by_id(db, rd->id_researcher);

                // Check if the researcher is valid
                if (r != NULL)
                {
                    // Insert the researcher in the list
                    insert_list(researchers_of_data, r, r->id);
                }
            }
        }

        // Get the next node
        node = node->next;
    }

    return researchers_of_data;
}

researcher *get_researcher_by_name(database *db, char *name)
{
    // Get the researcher
    node_t *node = db->researcher_db->head;

    // Iterate over the researcher
    while (node != NULL)
    {
        // Get the researcher
        researcher *r = (researcher *)node->data;

        // Check if the researcher is valid
        if (r != NULL)
        {
            // Check if the researcher is the one specified
            if (strcmp(r->name, name) == 0)
            {
                return r;
            }
        }

        // Get the next node
        node = node->next;
    }

    return NULL;
}

abstract_data *get_data_by_name(database *db, data_type data_type, char *name)
{
    // Get the data
    node_t *node;

    if (data_type == CONFERENCE)
    {
        node = db->data_conf_db->head;
    }
    else
    {
        node = db->data_perid_db->head;
    }

    int smaller_distance = 1000000; // Used for the Levenshtein distance
    int current;
    abstract_data *closest_node = NULL;
    // Iterate over the data
    while (node != NULL)
    {
        // Get the data
        abstract_data *data = (abstract_data *)node->data;

        // Check if the data is valid
        if (data != NULL)
        {

            // Try strcmp first
            if (strcmp(data->c_name, name) == 0)
            {
                return data;
            }

            // Terrible way to do it, but it works
            current = str_compare_distance(data->c_name, name, strlen(name) / 2.5);
            if (current != -1)
            {
                if (current < smaller_distance)
                {
                    smaller_distance = current;
                    closest_node = data;
                }
            }
        }

        // Get the next node
        node = node->next;
    }

    return closest_node;
}

// Iterates over the list and removes the node without specified prop
void filter_data_by_props(list_t *list, char *code, char *name, int year)
{
    // Iterate over the list
    node_t *node = list->head;

    while (node != NULL)
    {
        // Get the data
        abstract_data *data = (abstract_data *)node->data;

        // Check if the data is valid
        if (data != NULL)
        {
            // Check if the data is of the specified code
            if ((code != NULL) && (strcmp(data->c_code, code) != 0))
            {
                // Remove the node
                remove_list(list, data->id);
            }
            else if ((name != NULL) && (strcmp(data->c_name, name) != 0))
            {
                // Remove the node
                remove_list(list, node->data_id);
            }
            else if ((year != -1) && (data->c_year != year))
            {
                // Remove the node
                remove_list(list, node->data_id);
            }
            else
            {
                //printf("Not removing anything\n");
            }
        }

        // Get the next node
        node = node->next;
    }
}

/**
 * Researcher functions
 */

researcher *create_researcher(int id, char *name)
{
    // Create a researcher
    researcher *r = (researcher *)malloc(sizeof(researcher));

    // Set the id
    r->id = id;

    // Set the name
    r->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(r->name, name);

    // Set the publications count
    r->publications_count = 0;

    // Set the conferences count
    r->conferences_count = 0;

    return r;
}

void delete_researcher(researcher *researcher)
{
    // Free the name
    free(researcher->name);

    // Free the researcher
    free(researcher);
}

/**
 * Data functions
 */

abstract_data *create_data(int id, char *c_name, char *c_code, int c_year, data_type c_type, int from_qualis)
{
    // Create a data
    abstract_data *data = (abstract_data *)malloc(sizeof(abstract_data));

    // Set the id
    data->id = id;

    // Set the name
    data->c_name = (char *)malloc(sizeof(char) * (strlen(c_name) + 1));
    strcpy(data->c_name, c_name);

    // Set the code
    data->c_code = (char *)malloc(sizeof(char) * (strlen(c_code) + 1));
    strcpy(data->c_code, c_code);

    // Set the year
    data->c_year = c_year;

    // Set the type
    data->c_type = c_type;

    // Set the count 
    data->data_count = 0;

    // Set the from_qualis
    data->from_qualis = from_qualis;

    return data;
}

void delete_data(abstract_data *data)
{
    // Free the name
    free(data->c_name);

    // Free the code
    free(data->c_code);

    // Free the data
    free(data);
}

/**
 * Researcher data functions
 */

researcher_data *create_relation(int id_data, int id_researcher, data_type data_type)
{
    // Create a relation
    researcher_data *rd = (researcher_data *)malloc(sizeof(researcher_data));

    // Set the id
    rd->id_data = id_data;

    // Set the id
    rd->id_researcher = id_researcher;

    // Set the data type
    rd->data_type = data_type;

    // Set the count of researchers

    return rd;
}

void delete_relation(researcher_data *rd)
{
    // Free the relation
    free(rd);
}
