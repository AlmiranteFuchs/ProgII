// Author: B. Fuchs Santos da Silva
// Date: 2022-11-21
// Description: Linked ordered List Header

#ifndef LINKED_LIB_H
#define LINKED_LIB_H

// List nodes
typedef struct node
{
    void *data;
    int data_id;
    struct node *next;
    struct node *prev;
} node_t;

// List
typedef struct list
{
    node_t *head;
    node_t *tail;
    int size;
} list_t;

// List functions
list_t *create_list();
list_t *copy_list(list_t *list);
void destroy_list(list_t *list);
void insert_list(list_t *list, void *data, int data_id);        // OK
void insert_list_unique(list_t *list, void *data, int data_id); // OK
void remove_list(list_t *list, int data_id);                    // OK
void print_list(list_t *list);                                  // OK
int empty_list(list_t *list);                                   // OK
int size_list(list_t *list);                                    // OK
node_t *search_list(list_t *list, int data_id);                 // OK

// Node functions
node_t *create_node(void *data, int data_id); // OK
void destroy_node(node_t *node);

#endif