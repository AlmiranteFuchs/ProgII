// Author: Bruno Fuchs Santos da Silva
// Date 03.11.2022
// Version 1.0
// Description: AVL Tree test file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/avl_lib.h"

void stdin_to_avl_op(avl_node *root);

int main()
{
    // Create
    avl_node *root = NULL;

    // Read input file on stdin
    stdin_to_avl_op(root);

    return 0;
}

void stdin_to_avl_op(avl_node *root)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Read the input file
    while ((read = getline(&line, &len, stdin)) != -1)
    {
        // Get the operation
        char *operation = strtok(line, " ");

        // Get the key
        char *key = strtok(NULL, " ");

        // Check if the operation is insert
        if (strcmp(operation, "i") == 0)
        {
            // Insert the key
            root = avl_insert(root, atoi(key));
        }
        // Check if the operation is delete
        else if (strcmp(operation, "r") == 0)
        {
            // Delete the key
            root = avl_remove(root, atoi(key));
        }
        // Check if the operation is search
        else if (strcmp(operation, "s") == 0)
        {
            // Search the key
            avl_node *node = avl_search(root, atoi(key));

            // Check if the node is null
            if (node == NULL)
            {
                // Print the key not found
                printf("k %s n\n", key);
            }
            else
            {
                // Print the key found
                printf("k %s f\n", key);
            }
        }
    }

    // Print the AVL Tree
    avl_print_in_order(root, 0);
}