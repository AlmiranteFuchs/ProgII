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

    // Set name
    r->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(r->name, name);

    // Set id
    r->id = id;

    // Set other attributes
    r->publications_list = NULL;
    r->conferences_list = NULL;

    r->publications_count = 0;
    r->conferences_count = 0;

    return r;
}

void ent_destroy_researcher(t_researcher *researcher)
{
    // Free name
    free(researcher->name);

    // Free data
    ent_destroy_abstract_data(researcher->publications_list);
    ent_destroy_abstract_data(researcher->conferences_list);

    // Free object
    free(researcher);
}

t_abstract_data *ent_create_abstract_data(char *c_name, char *c_code, int c_year, t_data_type c_data_type)
{
    // Allocates memory for the object
    t_abstract_data *a = (t_abstract_data *)malloc(sizeof(t_abstract_data));

    // Set conference name
    a->c_name = (char *)malloc(sizeof(char) * (strlen(c_name) + 1));
    strcpy(a->c_name, c_name);

    // Set conference code
    a->c_code = (char *)malloc(sizeof(char) * (strlen(c_code) + 1));
    strcpy(a->c_code, c_code);

    // Set conference year
    a->c_year = c_year;

    // Set other attributes
    // a->researchers_list = NULL;
    // a->researchers_count = 0;
    a->c_type = c_data_type;

    return a;
}

void ent_destroy_abstract_data(t_abstract_data *abstract_data)
{
    // Free conference name
    free(abstract_data->c_name);

    // Free conference code
    free(abstract_data->c_code);

    // Free object
    free(abstract_data);
}

void ent_add_conference(t_researcher *researcher, t_abstract_data *conference)
{
    // Add conference to researcher
    researcher->conferences_list = (t_abstract_data *)realloc(researcher->conferences_list, sizeof(t_abstract_data) * (researcher->conferences_count + 1));
    researcher->conferences_list[researcher->conferences_count] = *conference;
    researcher->conferences_count++;
}

void ent_add_publication(t_researcher *researcher, t_abstract_data *publication)
{
    // Add publication to researcher
    researcher->publications_list = (t_abstract_data *)realloc(researcher->publications_list, sizeof(t_abstract_data) * (researcher->publications_count + 1));
    researcher->publications_list[researcher->publications_count] = *publication;
    researcher->publications_count++;
}


