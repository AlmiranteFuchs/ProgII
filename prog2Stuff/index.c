/***
 * Author: Fuchs
 * Date: 17.11.22
 * Desc: Material para estudos
 *
 */

#include <stdio.h>  // I/O Library
#include <stdlib.h> // Malloc library
#include <string.h> // String Library

#include <unistd.h> // ?? params
#include <ctype.h>  // ?? params
#include <stdlib.h> // ?? params

#include "libs/test_write.h"

// Read a string then, reverse it, returns a copy
char *reverse_string(char *string)
{

    // Malloc string of string size
    char *reverse_string = (char *)malloc(sizeof(char) * strlen(string));

    // Copies content
    strcpy(reverse_string, string);

    // Reverse add
    int i, j = 0;
    for (i = strlen(string) - 1; i >= 0; i--)
    {
        reverse_string[j] = string[i];
        j++;
    }
    printf("%s\n", reverse_string);

    return reverse_string;
}

int main(int argc, char **argv, char **envp)
{
    char *value;
    int option;

    opterr = 0;

    while ((option = getopt(argc, argv, "a:")) != -1)
    {
        switch (option)
        {
        case 'a':   
            value = optarg;
            break;

        default:
            fprintf(stderr, "Usage: %s -a -b -c value\n", argv[0]);
            exit(1);
        }
    }

    // escreva("Serve para testar libs");
    // reverse_string("Paspagaio!");
}