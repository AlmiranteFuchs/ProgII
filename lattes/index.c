#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LINESIZE 500000

int menu();

int main(int argc, char *argv[])
{
    // TEST
    // Path Test
    if (argc <= 1)
    {
        printf("Sem argumentos especificados, especifique o caminho para diretório\n");
        exit(0);
    }

    // Reading file
    FILE *file_ptr;

    // Read file
    file_ptr = fopen(argv[1], "r");
    if (file_ptr == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
        exit(0);
    }

    // Contents of File

    char character;

    while ((character = fgetc(file_ptr)) != EOF)
    {
        if (character == '<')
        {
            char tag[99999] = "";
            int i = 0;

            tag[i] = character;
            i++;

            while (character != '>' && character != EOF)
            {
                character = fgetc(file_ptr);
                tag[i] = character;
                i++;
            }
            printf("%s\n\n", tag);
        }
    }
    fclose(file_ptr);
    return 1;
    // TEST
}

int convert_utf8()
{
    return 0;
}

int menu()
{
    unsigned int menu_choice;
    int menu_exited = 0;

    printf("\n\n\t\tAnálise Currículo - Lattes Qualli\n\n\n");
    while (!menu_exited)
    {
        printf("\n\n\t\t\tMenu Principal\n\n");
        printf("\t\t1 - Periódicos por estrato\n");
        printf("\t\t2 - Conferências por estrato\n");
        printf("\t\t3 - Autoria por estrato\n");
        printf("\t\t4 - Sair\n\n");
        printf("\t\tDigite a opção desejada: ");
        scanf("%d", &menu_choice);
        switch (menu_choice)
        {
        case 1:
            printf("\n\n\t\t\tNão implementado\n\n");
            menu_exited = 1;
            break;
        case 2:
            printf("\n\n\t\t\tNão implementado\n\n");
            menu_exited = 1;
            break;
        case 3:
            printf("\n\n\t\t\tNão implementado\n\n");
            menu_exited = 1;
            break;
        case 4:
            printf("\n\n\t\t\tNão implementado\n\n");
            menu_exited = 1;
        default:
            printf("\n\n\t\t\tOpcção Inválida\n\n");
            break;
        }
    }
    return 0;
}