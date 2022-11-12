// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: entities Header File

#ifndef ENTITIES_H
#define ENTITIES_H

// Prototype of struct
// typedef enum t_data_type t_data_type;

// inumerate the types of data
typedef enum
{
    CONFERENCE,
    PUBLICATION
} t_data_type;

// Data object
typedef struct
{
    int id;
    char *c_name;
    char *c_code;
    int c_year;

    t_data_type c_type; // 0 = Conference, 1 = Publication

} t_abstract_data;

// Researcher object
typedef struct
{
    int id;
    char *name;

    int publications_count;
    int conferences_count;

} t_researcher;

// Intermediary object
typedef struct
{
    int id;
    int id_resercher;
    int id_data;
} t_reserch_data;

// Data object, responsible to link the researcher with the data
typedef struct
{
    t_reserch_data *data;
    int cardinality;
} t_data;

// Eu poderia usar uma lista genérica mas não quero ;')

// Creates a empty researcher
t_researcher *ent_create_researcher(char *name, int id);
// Destroy a researcher
void ent_destroy_researcher(t_researcher *researcher);
// Creates a empty abstract data
t_abstract_data *ent_create_abstract_data(char *c_name, char *c_code, int c_year, t_data_type c_data_type);
// Destroy a abstract data
void ent_destroy_abstract_data(t_abstract_data *abstract_data);
// Add a conference to a researcher
t_reserch_data *ent_create_relation(t_data *data, t_researcher *researcher, t_abstract_data *abstract_data, int id);
// Creates a empty relation
t_data *ent_create_data();
// Push new register to the data
void ent_push_data(t_data *data, t_reserch_data *reserch_data);
// Destroy a relation
void ent_destroy_data(t_data *data);

#endif