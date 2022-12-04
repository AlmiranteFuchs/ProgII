// Author: Bruno Fuchs Santos da Silva
// Date 03.11.2022
// Version 1.0
// Description: AVL Tree Library Implementation

#include "avl_lib.h"
#include <stdio.h>
#include <string.h>

// Define max
#define max(a, b) ((a) > (b) ? (a) : (b))

// Define min
#define min(a, b) ((a) < (b) ? (a) : (b))

int avl_tree_height(avl_node *node)
{
    // Simple failsafe if node null
    return node == NULL ? 0 : node->height;
}
//
// AVL Operations
//
// Create a new AVL Tree
avl_node *avl_create(int key)
{
    // Create a new node
    avl_node *new = (avl_node *)malloc(sizeof(avl_node));

    // Properties
    new->key = key;
    new->height = 1;
    new->left_child = NULL;
    new->right_child = NULL;
    new->parent = NULL;

    return new;
}
// Rotate the AVL Tree to the right
avl_node *avl_tree_rotate_right(avl_node *y)
{
    avl_node *x = y->left_child;
    avl_node *T2 = x->right_child;

    // Perform rotation
    x->right_child = y;
    y->left_child = T2;

    // Update heights
    y->height = max(avl_tree_height(y->left_child), avl_tree_height(y->right_child)) + 1;
    x->height = max(avl_tree_height(x->left_child), avl_tree_height(x->right_child)) + 1;

    // Return new root
    return x;
}

// Rotate the AVL Tree to the left
avl_node *avl_tree_rotate_left(avl_node *x)
{
    avl_node *y = x->right_child;
    avl_node *T2 = y->left_child;

    // Perform rotation
    y->left_child = x;
    x->right_child = T2;

    // Update heights
    x->height = max(avl_tree_height(x->left_child), avl_tree_height(x->right_child)) + 1;
    y->height = max(avl_tree_height(y->left_child), avl_tree_height(y->right_child)) + 1;

    // Return new root
    return y;
}

// Balance factor
int avl_tree_balance_factor(avl_node *node)
{
    if (node == NULL)
        return 0;
    return avl_tree_height(node->left_child) - avl_tree_height(node->right_child);
}

// Insert a new Node in the AVL Tree
avl_node *avl_insert(avl_node *node, int key)
{

    // Find position
    if (node == NULL)
        return (avl_create(key));

    if (key < node->key)
        // Insert in left subtree
        node->left_child = avl_insert(node->left_child, key);
    else if (key > node->key)
        // Insert in right subtree
        node->right_child = avl_insert(node->right_child, key);
    else
        // Leaf insertion
        return node;

    // Balance the tree
    node->height = 1 + max(avl_tree_height(node->left_child), avl_tree_height(node->right_child));

    int balance = avl_tree_balance_factor(node);

    // Left Left Case
    if (balance < -1 && key > node->right_child->key)
        return avl_tree_rotate_left(node);

    // Right Right Case
    if (balance < -1 && key < node->right_child->key)
    {
        node->right_child = avl_tree_rotate_right(node->right_child);
        return avl_tree_rotate_left(node);
    }

    // Left Right Case
    if (balance > 1 && key < node->left_child->key)
        return avl_tree_rotate_right(node);

    // Right Left Case
    if (balance > 1 && key > node->left_child->key)
    {
        node->left_child = avl_tree_rotate_left(node->left_child);
        return avl_tree_rotate_right(node);
    }

    return node;
}

// Search for a Node in the AVL Tree with value key
avl_node *avl_search(avl_node *node, int key)
{
    // Simple failsafe if node null, recursion base case
    if (node == NULL)
        return NULL;

    // Binary search
    if (key < node->key)
        return avl_search(node->left_child, key);
    else if (key > node->key)
        return avl_search(node->right_child, key);
    else
        return node;
}


// Print Functions
void avl_print_pre_order(avl_node *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%d,%d\n", root->key, root->height - 1);
    avl_print_pre_order(root->left_child);
    avl_print_pre_order(root->right_child);
}

void avl_print_in_order(avl_node *root)
{
    if (root == NULL)
    {
        return;
    }
    avl_print_in_order(root->left_child);
    printf("%d,%d\n", root->key, root->height - 1);
    avl_print_in_order(root->right_child);
}

void avl_print_post_order(avl_node *root)
{
    if (root == NULL)
    {
        return;
    }
    avl_print_post_order(root->left_child);
    avl_print_post_order(root->right_child);
    printf("%d,%d\n", root->key, root->height - 1);
}