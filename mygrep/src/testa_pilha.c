/* 
 * Programa para testar a libpilha conforme .h do Bona
 * feito por Marcos Castilho em 05/04/2022
*/

#include <stdio.h>
#include "../libs/libpilha.h"
#define MAX 5

/* ################################################################################# */
/* # tive que colocar a mylibmessages.c aqui, pois os alunos nao tem essa biblioteca */
/* ################################################################################# */
void teste (char *s)
{
    printf ("> teste %s\n", s);
}

void msg (char *s)
{
    printf ("----> %s\n", s);
}

void aluno (char *s)
{
    printf ("-> saida do aluno     %s\n", s);
}

void esperado (char *s)
{
    printf ("--> resultado esperado\n");
    printf ("-------> %s\n", s);
    printf ("-------------------------------------------------------------------------\n");
}

void numero (char *s, int i)
{
    printf ("%s %d\n", s, i);
}
/* ################################################################################# */

pilha_t* teste_criar_pilha (int maximo)
{
    pilha_t* p;

    if (! (p = pilha_cria (maximo)))
        return NULL;

    return p;
}

void teste_pilha_vazia (pilha_t* p)
{
    if (pilha_vazia (p))
        msg ("pilha vazia");
    else
        msg ("pilha nao vazia");
}

void teste_push (pilha_t* p, int limite)
{
    int i;

    for (i=1; i <= limite+1; i++)
        if (! push (p, i))
            printf ("%d nao inserido: pilha cheia\n", i);
    pilha_imprime (p);
}

void teste_topo (pilha_t* p)
{
    printf ("%d\n", pilha_topo (p));
    printf ("%d\n", pilha_topo (p));
    printf ("%d\n", pilha_topo (p));
}

void teste_pop_topo_e_tamanho (pilha_t* p)
{
    int i;

    msg ("neste momento a pilha tem da base ate o topo de 1 a 5");
    msg ("vai remover todo mundo e imprimir o topo retirado");

    while (! pilha_vazia (p))
    {
        numero ("topo= ", pilha_topo (p));
        if ((i= pop(p)))
            pilha_imprime (p);
    }
}

int main (void)
{
    pilha_t *p;

    teste ("criar uma pilha");
    p= teste_criar_pilha (MAX);
    numero ("tam= ", pilha_tamanho(p));
    esperado ("torcendo para ter inicializado corretamente, o tamanho tem que ser zero");

    teste ("pilha vazia");
    teste_pilha_vazia (p);
    esperado ("pilha vazia");

    teste ("empilhar ate o maximo do vetor, mais um, para forcar pilha cheia");
    aluno ("");
    teste_push (p, MAX); 
    esperado ("tem que imprimir de 1 ate 5, e o 6 nao pode ser inserido");

    teste ("neste momento nao pode dar pilha vazia");
    aluno ("");
    teste_pilha_vazia (p);
    esperado ("pilha nao vazia");

    teste ("consulta ao topo");
    aluno ("");
    teste_topo (p); 
    esperado ("tem que imprimir 5 tres vezes");
    
    teste ("tamanho de novo");
    aluno ("");
    numero ("tam= ", pilha_tamanho(p));
    esperado ("tem que imprimir 5");

    teste ("desempilha tudo e vai consultando o topo ao mesmo tempo");
    teste_pop_topo_e_tamanho (p); 

    teste ("pilha vazia de novo");
    teste_pilha_vazia (p);
    esperado ("pilha vazia");

    teste ("tentativa de remover um elemento com a pilha vazia");
    if (pop (p))
        msg ("OK: pop na pilha vazia nao deu segfault.");
    else
        msg ("OK");
    esperado ("OK, desde que nao tenha dado segfaut...");

    teste ("destruir a pilha");
    p= pilha_destroi (p);
    msg ("Pilha vazia destruida");
    esperado ("Pilha vazia destruida E nao ter dado segfault");

    teste ("destruir uma pilha nao vazia, antes, para isso, ela sera criada de novo com alguns elementos"); 
    p= teste_criar_pilha (MAX);
    teste_push (p, MAX-1); 
    p= pilha_destroi (p);
    msg ("Pilha com elementos destruida");
    esperado ("OK, desde que nao tenha dado segfaut...");

    return 0;
}
