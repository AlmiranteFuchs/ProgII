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

//
// AVL Operations
//
// Create a new AVL Tree
// Eu refiz essa biblioteca 5 vezes
avl_node *avl_create(int key)
{
    avl_node *node = NULL;

    // Allocate memory for the node
    if ((node = (avl_node *)malloc(sizeof(avl_node))) == NULL)
    {
        perror("Could not allocate memory for the node");
        return NULL;
    }

    // Set the node properties
    node->key = key;
    node->left_child = NULL;
    node->right_child = NULL;
    node->parent = NULL;
    node->height = 0;

    return node;
}

// Insert the node
avl_node *avl_insert(avl_node *node, int key)
{
    // Base case of recursion
    if (node == NULL)
    {
        return avl_create(key);
    }

    if (key > node->key)
        node->right_child = avl_insert(node->right_child, key);
    else if (key < node->key)
        node->left_child = avl_insert(node->left_child, key);
    else
        return node;

    // Update the height
    node->height = 1 + max(avl_height(node->left_child), avl_height(node->right_child));

    int bal = avl_balance_factor(node);
    if (bal > 1)
    {
        if (key < node->left_child->key)
        {
            return avl_rot_right(node);
        }
        else
        {
            node->left_child = avl_rot_left(node->left_child);
            return avl_rot_right(node);
        }
    }
    else if (bal < -1)
    {
        if (key > node->right_child->key)
        {
            return avl_rot_left(node);
        }
        else
        {
            node->right_child = avl_rot_right(node->right_child);
            return avl_rot_left(node);
        }
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

// Height
int avl_height(avl_node *node)
{
    return node == NULL ? -1 : node->height;
}

// Balance Factor
int avl_balance_factor(avl_node *node)
{
    if (node == NULL)
        return 0;
    return avl_height(node->left_child) - avl_height(node->right_child);
}

// Remove node
avl_node *avl_remove(avl_node *remove, int key)
{
    // Base case
    if (remove == NULL)
        return remove;

    // Search for the node
    if (key < remove->key)
        remove->left_child = avl_remove(remove->left_child, key);
    else if (key > remove->key)
        remove->right_child = avl_remove(remove->right_child, key);
    else
    {
        // Node with only one child or no child
        if ((remove->left_child == NULL) || (remove->right_child == NULL))
        {
            avl_node *temp = NULL;
            if (remove->left_child != NULL)
                temp = remove->left_child;
            else
                temp = remove->right_child;

            if (temp == NULL)
            {
                // No child case
                temp = remove;
                remove = NULL;
            }
            else
            {
                // One child case, copy the child to the node
                *remove = *temp;
            }

            // Free the memory
            free(temp);
        }
        else
        {
            // Node with two children
            avl_node *temp = avl_max(remove->left_child);
            remove->key = temp->key;
            remove->left_child = avl_remove(remove->left_child, temp->key);
        }
    }

    if (remove == NULL)
        return remove;

    // Update the height
    remove->height = max(avl_height(remove->left_child), avl_height(remove->right_child)) + 1;

    // Verifica o fator de balanceamento entre os dois filhos do nó.
    int bal = avl_balance_factor(remove);
    if (bal > 1)
    {
        if (avl_balance_factor(remove->left_child) >= 0)
        {
            // Left Left Case
            return avl_rot_right(remove);
        }
        else
        {
            // Left Right Case
            remove->left_child = avl_rot_left(remove->left_child);
            return avl_rot_right(remove);
        }
    }
    else if (bal < -1)
    {
        if (avl_balance_factor(remove->right_child) <= 0)
        {
            // Right Right Case
            return avl_rot_left(remove);
        }
        else
        {
            // Right Left Case
            remove->right_child = avl_rot_right(remove->right_child);
            return avl_rot_left(remove);
        }
    }

    // Return the node
    return remove;
}

// Rotate to the right
avl_node *avl_rot_right(avl_node *y)
{
    avl_node *x = y->left_child;
    avl_node *T2 = x->right_child;

    // Perform rotation
    x->right_child = y;
    y->left_child = T2;

    // Update heights
    y->height = max(avl_height(y->left_child), avl_height(y->right_child)) + 1;
    x->height = max(avl_height(x->left_child), avl_height(x->right_child)) + 1;

    // Return new root
    return x;
}

// Rotate to the left
avl_node *avl_rot_left(avl_node *x)
{
    avl_node *y = x->right_child;
    avl_node *T2 = y->left_child;

    // Perform rotation
    y->left_child = x;
    x->right_child = T2;

    // Update heights
    x->height = max(avl_height(x->left_child), avl_height(x->right_child)) + 1;
    y->height = max(avl_height(y->left_child), avl_height(y->right_child)) + 1;

    // Return new root
    return y;
}

// Max of subtree
avl_node *avl_max(avl_node *node)
{
    avl_node *current = node;

    // Loop down to find the rightmost leaf
    while (current->right_child != NULL)
        current = current->right_child;

    return current;
}

// Min of subtree
avl_node *avl_min(avl_node *node)
{
    avl_node *current = node;

    // Loop down to find the leftmost leaf
    while (current->left_child != NULL)
        current = current->left_child;

    return current;
}

// Print Functions
void avl_print_pre_order(avl_node *node, int level)
{
    if (node != NULL)
    {
        printf("%d ", node->key);
        avl_print_pre_order(node->left_child, level + 1);
        avl_print_pre_order(node->right_child, level + 1);
    }
}

void avl_print_in_order(avl_node *root, int level)
{
    // This was the reason I had to remake this lib 5 times, I forgot to add the level parameter and was printing height instead of level
    if (root)
    {
        avl_print_in_order(root->left_child, level + 1);
        printf("%d,%d\n", root->key, level);
        avl_print_in_order(root->right_child, level + 1);
    }
}

void avl_print_post_order(avl_node *root, int level)
{
    if (root)
    {
        avl_print_post_order(root->left_child, level + 1);
        avl_print_post_order(root->right_child, level + 1);
        printf("%d,%d\n", root->key, level);
    }
}

// Untested
// Successor
avl_node *avl_successor(avl_node *node){
    if(node->right_child != NULL){
        return avl_min(node->right_child);
    }
    avl_node *y = node->parent;
    while(y != NULL && node == y->right_child){
        node = y;
        y = y->parent;
    }
    return y;
}

// Predecessor
avl_node *avl_predecessor(avl_node *node){
    if(node->left_child != NULL){
        return avl_max(node->left_child);
    }
    avl_node *y = node->parent;
    while(y != NULL && node == y->left_child){
        node = y;
        y = y->parent;
    }
    return y;
}



