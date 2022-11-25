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
    // s->str = (char **)realloc(s->str, sizeof(char *) * (s->pos + 1));

    // Allocates memory for the string
    // s->str[s->pos] = (char *)malloc(sizeof(char) * (strlen(str) + 1));

    // Copy string
    // strcpy(s->str[s->pos], str);

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

int str_contains(char *str, L_String *L_string)
{
    for (int i = 0; i < L_string->pos; i++)
    {
        if (strcmp(L_string->str[i], str) == 0)
        {
            return 1;
        }
    }
    return 0;
}

char *str_to_lower(char *str)
{
    char *new_str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(new_str, str);

    for (int i = 0; i < strlen(new_str); i++)
    {
        new_str[i] = tolower(new_str[i]);
    }
    return new_str;
}

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
// levenshtein algorithm, returns the distance between two strings, -1 if higher than the max_distance https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C
int str_compare_distance(char *s1, char *s2, int max_distance)
{
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len + 1];
    for (y = 1; y <= s1len; y++)
        column[y] = y;
    for (x = 1; x <= s2len; x++)
    {
        column[0] = x;
        for (y = 1, lastdiag = x - 1; y <= s1len; y++)
        {
            olddiag = column[y];
            column[y] = MIN3(column[y] + 1, column[y - 1] + 1, lastdiag + (s1[y - 1] == s2[x - 1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return column[s1len] > max_distance ? -1 : column[s1len];
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
