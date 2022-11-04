#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LINESIZE 500000

int parse_file_tag(char *filename);
void program_params(char **cvs, char **q_conf, char **q_period, int argc, char *argv[]);

int main(int argc, char *argv[])
{
    char *cvs = NULL;
    char *q_conf = NULL;
    char *q_period = NULL;

    program_params(&cvs, &q_conf, &q_period, argc, argv);

    // printf("flag_a = %s, flag_b = %s, value_c = %s\n", cvs, q_conf, q_period);
    return 0;
}

// Parse the command line parameters
void program_params(char **cvs, char **q_conf, char **q_period, int argc, char *argv[])
{
    int option;

    opterr = 0;

    while ((option = getopt(argc, argv, "d:c:p:")) != -1)
    {
        switch (option)
        {
        case 'd': // option -a was set
            *cvs = optarg;
            break;
        case 'c': // option -b was set
            *q_conf = optarg;
            break;
        case 'p': // option -c was set with value
            *q_period = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s -a -b -c value\n", argv[0]);
            exit(1);

            parse_file_tag(argv[1]);
        }
    }

    // printf("flag_a = %s, flag_b = %s, value_c = %s\n", cvs, q_conf, q_period);

    // Param test
    if (cvs == NULL || q_conf == NULL || q_period == NULL)
    {

        fprintf(stderr, "Parâmetros insuficientes, uso: %s -d <diretorio com os CVs> -c <arquivo com a lista Qualis Conf> -p <arquivo com a lista Qualis Periódicos>\n", argv[0]);
        exit(1);
    }
}

// Reads file and sets to array
char *parse_file(char *filename)
{
    // Reading file
    FILE *file_ptr;

    // Read file
    file_ptr = fopen(filename, "r");
    if (file_ptr == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(0);
    }

    // Get size of file
    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    rewind(file_ptr);

    // Allocs array of char of file_size
    char *file_content = (char *)malloc(sizeof(char) * file_size);
    if (file_content == NULL)
    {
        printf("Erro ao alocar memória\n");
        exit(0);
    }

    // Read file to array
    fread(file_content, sizeof(char), file_size, file_ptr);

    // Close file
    fclose(file_ptr);
    return file_content;
}

int parse_file_tag(char *file_content)
{
    // Search substring
    char *tag_ = "DETALHAMENTO-DO-ARTIGO";

    char *tag_ptr = strstr(file_content, tag);
    if (tag_ptr == NULL)
    {
        printf("Não foi encontrado a tag\n");
        exit(0);
    }

    // Offsets file_content after tag
    tag_ptr = tag_ptr + strlen(tag);

    // Print content
    printf("%s\n", tag_ptr);

    return 0;
}
