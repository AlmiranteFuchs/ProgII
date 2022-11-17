#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Author: Bruno Fuchs
// Date: 2022-07-11
// Description: This program reads a text file and prints the number of words in the file. Like grep -c.

int parse_search(char *filename, char *tag);

int main(int argc, char *argv[])
{
    // Get first 2 parameters
    char *filename = argv[1];
    char *tag = argv[2];

    // Check if parameters are valid
    if (argc != 3)
    {
        printf("Parâmetros inválidos");
    }

    // Call function to parse file
    int tag_count = parse_search(filename, tag);

    // Print number of tags
    printf("%d\n",tag_count);
    return 0;
}

int parse_search(char *filename, char *tag)
{
    // Tag counter
    int tag_count = 0;

    // Open file
    FILE *file = fopen(filename, "r");

    // If file is not found
    if (file == NULL)
    {
        printf("File not found\n");
        exit(1);
    }

    // Read line by line
    char line[1000];
    while (fgets(line, sizeof(line), file))
    {
        // If tag is found
        if (strstr(line, tag) != NULL)
        {
            // Increment tag counter
            tag_count++;
        }
    }

    return tag_count;
}

