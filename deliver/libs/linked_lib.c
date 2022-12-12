// Author: B. Fuchs Santos da Silva
// Date: 2022-11-21
// Description: Linked ordered List Implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_lib.h"
#include "../src/entities.h"

// Create a new List
list_t *create_list()
{
    // Allocates memory for the object
    list_t *list = (list_t *)malloc(sizeof(list_t));

    // Set pos
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

// Copies a list
list_t *copy_list(list_t *list)
{
    // Allocates memory for the object
    list_t *new_list = create_list();

    // For every node in the list create a new node and insert in the new list
    node_t *node = list->head;
    while (node != NULL)
    {
        insert_list(new_list, node->data, node->data_id);
        node = node->next;
    }

    return new_list;
}

// Create a new Node
node_t *create_node(void *data, int data_id)
{
    // Allocates memory for the object
    node_t *node = (node_t *)malloc(sizeof(node_t));

    // Set pos
    node->data_id = data_id;
    // Set data
    node->data = data;

    // researcher *r = (researcher *)(node)->data;

    // printf("Debug %d\n>: ", r->id);
    // printf("Debug %s\n>: ", r->name);

    node->next = NULL;
    node->prev = NULL;

    return node;
}

// Insert a new Node in the List, ordered by data_id
void insert_list(list_t *list, void *data, int data_id)
{
    // Create a new Node
    node_t *new_node = create_node(data, data_id);

    // If the list is empty
    if (list->size == 0)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    // If the list is not empty
    else
    {
        // If the new node is the first
        if (data_id <= list->head->data_id)
        {
            new_node->next = list->head;
            list->head->prev = new_node;
            list->head = new_node;
        }
        // If the new node is the last
        else if (data_id >= list->tail->data_id)
        {
            new_node->prev = list->tail;
            list->tail->next = new_node;
            list->tail = new_node;
        }
        // If the new node is in the middle
        else
        {
            node_t *current = list->head;

            while (current->data_id < data_id)
            {
                current = current->next;
            }

            new_node->next = current;
            new_node->prev = current->prev;
            current->prev->next = new_node;
            current->prev = new_node;
        }
    }
    list->size++;
}

// Insert a new Node in the List, ordered by data_id, if the data_id is unique
void insert_list_unique(list_t *list, void *data, int data_id)
{
    // If the data_id is unique
    if (search_list(list, data_id) == NULL)
    {
        insert_list(list, data, data_id);
    }
}

// Search a Node in the List by data_id, binary search
node_t *search_list(list_t *list, int data_id)
{
    // If the list is empty
    if (list->size == 0)
    {
        return NULL;
    }
    // If the list is not empty
    else
    {
        // Binary search by data_id
        int left = 0;
        int right = list->size - 1;
        int middle = (left + right) / 2;
        node_t *current = list->head;

        while (left <= right)
        {
            middle = (left + right) / 2;
            current = list->head;
            for (int i = 0; i < middle; i++)
            {
                current = current->next;
            }
            if (current->data_id == data_id)
            {
                return current;
            }
            else if (current->data_id < data_id)
            {
                left = middle + 1;
            }
            else
            {
                right = middle - 1;
            }
        }
        return NULL;
    }
}

// Remove from the List a Node with the data_id
void remove_list(list_t *list, int data_id)
{
    // If the list is empty
    if (list->size == 0)
    {
        printf("The list is empty!\n");
    }
    // If the list is not empty
    else
    {
        // Search the node
        node_t *node = search_list(list, data_id);

        // If the node is not in the list
        if (node == NULL)
        {
            printf("The node is not in the list!\n");
        }
        // If the node is in the list
        else
        {
            // If the node is the head
            if (node == list->head)
            {
                list->head = node->next;
                if (list->head != NULL)
                {
                    list->head->prev = NULL;
                }
                else
                {
                    list->tail = NULL;
                }
            }
            // If the node is the tail
            else if (node == list->tail)
            {
                list->tail = node->prev;
                if (list->tail != NULL)
                {
                    list->tail->next = NULL;
                }
                else
                {
                    list->head = NULL;
                }
            }
            // If the node is in the middle
            else
            {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            // Free the node
            // free(node);
            list->size--;
        }
    }
}

// Prints the List ids
void print_list(list_t *list)
{
    node_t *current_node = list->head;
    while (current_node != NULL)
    {
        printf("%d\n", current_node->data_id);
        current_node = current_node->next;
    }
}

// Returns if list is empty
int empty_list(list_t *list)
{
    return list->size == 0;
}

// Returns the size of the List
int size_list(list_t *list)
{
    return list->size;
}

// Destroy Node
void destroy_node(node_t *node)
{
    free(node);
}

// Destroy List
void destroy_list(list_t *list)
{
    node_t *current_node = list->head;
    while (current_node != NULL)
    {
        node_t *next_node = current_node->next;
        destroy_node(current_node);
        current_node = next_node;
    }
    free(list);
}
