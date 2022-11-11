// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: entities Header File

#ifndef ENTITIES_H
#define ENTITIES_H

//Prototype of struct
//typedef enum t_data_type t_data_type;


// inumerate the types of data
typedef enum
{
    CONFERENCE,
    PUBLICATION
} t_data_type;

typedef struct
{
    char *c_name;
    char *c_code;
    int c_year;

    t_data_type c_type;             // 0 = Conference, 1 = Publication
    //t_researcher *researchers_list; // List of researchers with this data
    //int researchers_count;          // Number of researchers with this data

} t_abstract_data;

// Researcher object
typedef struct
{
    char *name;
    int id;
    t_abstract_data *conferences_list;
    t_abstract_data *publications_list;
    int conferences_count;
    int publications_count;
} t_researcher;





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
void ent_add_conference(t_researcher *researcher, t_abstract_data *conference);
// Add a publication to a researcher
void ent_add_publication(t_researcher *researcher, t_abstract_data *publication);

#endif