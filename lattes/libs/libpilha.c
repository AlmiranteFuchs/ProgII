/* libpilha.c, feito por Marcos Castilho em 30/03/2022 */
/* refeito em 05/04/2022 conforme novo .h do Bona      */

#include <stdio.h>
#include <malloc.h>
#include "libpilha.h"

int pilha_cheia (pilha_t* p)
{
    return pilha_tamanho (p) == p->nelem;
}

pilha_t* pilha_cria (int nelem)
{
    pilha_t* p;

    /* aloca espaco para a pilha_t */
    if (! (p = (pilha_t *) malloc (sizeof (pilha_t))))
        return NULL;

    /* aloca espaco para o vetor */
    if (! (p->elems = malloc (sizeof (int) * nelem)))
    {
        /* libera struct e retorna falha */
        free (p);
        return NULL;
    }
    p->topo = -1;     /* o tamanho eh topo + 1           */
    p->nelem = nelem; /* guarda o tamanho total do vetor */
    
    return p;
}

pilha_t* pilha_destroi (pilha_t* p)
{
    int i;

    /* zera o vetor */
    for (i=0; i < p->nelem; i++)
        p->elems[i]= 0;

    /* zera os campos da struct */
    p->nelem= 0;
    p->topo= 0;
    
    /* libera os espacos alocados */
    free (p->elems);
    free (p);

    return NULL;
}

int push (pilha_t* p, int n)
{
    /* se pilha cheia retorna falha */
    if (pilha_cheia (p))
        return 0;

    (p->topo)++;
    p->elems[p->topo] = n;
    
    return 1;
}

int pop (pilha_t* p)
{
    /* se pilha vazia retorna falha */
    if (pilha_vazia (p))
        return 0;

    (p->topo)--;
    return p->elems[p->topo + 1];
}

int pilha_topo (pilha_t* p)
{
    /* se pilha vazia retorna falha */
    if (pilha_vazia (p))
        return 0;

    return p->elems[p->topo];
}

int pilha_tamanho (pilha_t* p)
{
    return (p->topo) + 1;
}

int pilha_vazia (pilha_t* p)
{
    if (! pilha_tamanho (p))
        return 1;

    return 0;
}

void pilha_imprime (pilha_t* p)
{
    int i;

    /* se pilha vazia nao imprime nada, nem um \n */
    if (pilha_vazia (p))
        return;

    /* imprime o ultimo separado sem espaco */
    for (i=0; i < pilha_tamanho (p) - 1; i++)
        printf ("%d ", p->elems[i]);
    printf ("%d\n", p->elems[p->topo]);
}
