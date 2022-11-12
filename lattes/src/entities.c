// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: Entities are the objects that will be used in the logic
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entities.h"

t_researcher *ent_create_researcher(char *name, int id)
{
    // Allocates memory for the object
    t_researcher *r = (t_researcher *)malloc(sizeof(t_researcher));

    // Set id
    r->id = id;

    // Allocates memory for the name
    r->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    // Copy name
    strcpy(r->name, name);

    // Set publications count
    r->publications_count = 0;

    // Set conferences count
    r->conferences_count = 0;

    return r;
}

void ent_destroy_researcher(t_researcher *researcher)
{
    // Free name
    free(researcher->name);

    // Free object
    free(researcher);
}

t_abstract_data *ent_create_abstract_data(char *c_name, char *c_code, int c_year, t_data_type c_data_type)
{
    // Allocates memory for the object
    t_abstract_data *a = (t_abstract_data *)malloc(sizeof(t_abstract_data));

    // Set id
    a->id = 0;

    // Allocates memory for the name
    a->c_name = (char *)malloc(sizeof(char) * (strlen(c_name) + 1));
    // Copy name
    strcpy(a->c_name, c_name);

    // Allocates memory for the code
    a->c_code = (char *)malloc(sizeof(char) * (strlen(c_code) + 1));
    // Copy code
    strcpy(a->c_code, c_code);

    // Set year
    a->c_year = c_year;

    // Set data type
    a->c_type = c_data_type;

    return a;
}

void ent_destroy_abstract_data(t_abstract_data *abstract_data)
{
    // Free name
    free(abstract_data->c_name);

    // Free code
    free(abstract_data->c_code);

    // Free object
    free(abstract_data);
}

t_data *ent_create_data()
{
    // Allocates memory for the object
    t_data *d = (t_data *)malloc(sizeof(t_data));

    // Set cardinality
    d->cardinality = 0;

    // Set data
    d->data = NULL;

    return d;
}

void ent_push_data(t_data *data, t_reserch_data *reserch_data)
{
    // Allocates memory for the object
    data->data = (t_reserch_data *)realloc(data->data, sizeof(t_reserch_data) * (data->cardinality + 1));

    // Set id
    data->data[data->cardinality] = *reserch_data;

    // Increment cardinality
    data->cardinality++;
}

t_reserch_data *ent_create_relation(t_data *data, t_researcher *researcher, t_abstract_data *abstract_data, int id)
{
    // Allocates memory for the object
    t_reserch_data *r = (t_reserch_data *)malloc(sizeof(t_reserch_data));

    // Set id
    r->id = id;

    // Set researcher
    r->id_resercher = researcher->id;

    // Increment researcher publications count
    if (abstract_data->c_type == PUBLICATION)
    {
        researcher->publications_count++;
    }
    // Increment researcher conferences count
    else
    {
        researcher->conferences_count++;
    }

    // Set abstract data
    r->id_data = abstract_data->id;

    // Push relation to db
    ent_push_data(data, r);
    return r;
}

void ent_destroy_data(t_data *data){
    // Free data
    free(data->data);

    // Free object
    free(data);
}
