// Author: B. Fuchs Santos da Silva
// Date: 2022-11-10
// Description: String Lib

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "string_lib.h"

L_String *str_create()
{
    // Allocates memory for the object
    L_String *s = (L_String *)malloc(sizeof(L_String));

    // Set pos
    s->pos = 0;
    s->str = NULL;
    
    // Realloc memory for the string
    //s->str = (char **)realloc(s->str, sizeof(char *) * (s->pos + 1));

    // Allocates memory for the string
    //s->str[s->pos] = (char *)malloc(sizeof(char) * (strlen(str) + 1));

    // Copy string
    //strcpy(s->str[s->pos], str);

    // Increment pos

    return s;
}

void str_push(char *str, L_String *string)
{
    // Realloc memory for the string
    string->str = (char **)realloc(string->str, sizeof(char *) * (string->pos + 1));

    // Allocates memory for the string
    string->str[string->pos] = (char *)malloc(sizeof(char) * (strlen(str) + 1));

    // Copy string
    strcpy(string->str[string->pos], str);

    // Increment pos
    string->pos++;
}

void str_out(L_String *string)
{
    for (int i = 0; i < string->pos; i++)
    {
        printf("%s\n", string->str[i]);
    }
}

void str_clear(L_String *string)
{
    for (int i = 0; i < string->pos; i++)
    {
        free(string->str[i]);
    }
    free(string->str);
    free(string);
}
