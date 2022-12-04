// Author: Bruno Fuchs Santos da Silva
// Date 03.11.2022
// Version 1.0
// Description: AVL Tree Library Header File

#ifndef AVL_LIB_H
#define AVL_LIB_H

#include <stdlib.h>

// AVL Tree Node
typedef struct avl_node
{
    // Relations
    struct avl_node *left_child;
    struct avl_node *right_child;
    struct avl_node *parent;

    // Properties
    int key;
    int height;
} avl_node;



//
// AVL Operations
//
// Create a new AVL Tree
avl_node *avl_create(int key);
//Insert a new Node in the AVL Tree
avl_node *avl_insert(avl_node *root, int key);
// Delete a Node from the AVL Tree
avl_node *avl_remove(avl_node *root, int key);
// Search a Node in the AVL Tree
avl_node* avl_search(avl_node*root, int key);

//
// Balance
//
// Rotate the AVL Tree to the left
avl_node *avl_rot_right(avl_node *x);

// Rotate the AVL Tree to the right
avl_node *avl_rot_left(avl_node *y);

// Get the balance factor of the AVL Tree
int avl_balance_factor(avl_node *node);

//
// Print
//
// Print the AVL Tree
void avl_print_pre_order(avl_node *node, int level);

// Print the AVL Tree in order
void avl_print_in_order(avl_node *node, int level);

// Print the AVL Tree in post order
void avl_print_post_order(avl_node *node, int level);

//
// Properties
//
// Get the successor of the AVL Tree
avl_node *avl_successor(avl_node *node);

// Get the predecessor of the AVL Tree
avl_node *avl_predecessor(avl_node *node);

// Get the minimum value of the AVL Tree
avl_node *avl_min(avl_node *node);

// Get the maximum value of the AVL Tree
avl_node *avl_max(avl_node *node);

// Get the height of the AVL Tree
int avl_height(avl_node *node);

#endif