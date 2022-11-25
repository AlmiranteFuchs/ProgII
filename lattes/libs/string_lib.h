// Author: B. Fuchs Santos da Silva 
// Date: 2022-11-10
// Description: L_String Header

#ifndef L_STRING_LIB_H
#define L_STRING_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct l_string {
    char **str;
    int pos;
} L_String;

// Creates a new L_string
L_String *str_create();
// Push a new L_string to the end of the array
void str_push(char *str, L_String *L_string);
// Tells if the L_string already contains the string
int str_contains(char *str, L_String *L_string);
// Frees L_string
void str_clear(L_String *L_string);
// Prints L_string
void str_out(L_String *L_string);

// Not list functions but useful
// Returns the string to lowercase
char *str_to_lower(char *str);
// Compare two strings 
char *str_compare_distance(char *str);


#endif

