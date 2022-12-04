// Author: Bruno Fuchs Santos da Silva
// Date 03.11.2022
// Version 1.0
// Description: AVL Tree test file

#include <stdio.h>
#include "libs/avl_lib.h"

int main()
{
    printf("Creating a new AVL Tree...\n");
    avl_node* root = avl_create(10);

    printf("Inserting the new Node in the AVL Tree...\n");
    root = avl_insert(root, 10);
    root = avl_insert(root, 20);
    root = avl_insert(root, 30);
    root = avl_insert(root, 40);
    root = avl_insert(root, 50);
    root = avl_insert(root, 25);


    // Print
    printf("Printing the AVL Tree Pre Order...\n");
    avl_print_pre_order(root);

    printf("Printing the AVL Tree In Order...\n");
    avl_print_in_order(root);

    printf("End of the program...\n");

    return 0;
}