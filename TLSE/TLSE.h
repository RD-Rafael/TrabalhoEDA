#ifndef TLSE_H
#define TLSE_H
#include "../ATLETA.h"

typedef struct tlse{
    void* info;
    struct tlse * prox;
}TLSE;

TLSE* TLSE_inicializa();
TLSE* TLSE_insere_inicio(TLSE* lse, void* info);
// TLSE* TLSE_busca(TLSE* lse, int n);
void TLSE_print(TLSE* lse);
int TLSE_conta(TLSE* l);
void TLSE_free(TLSE* lse);
void TLSE_print_champion(TLSE* lse);
TLSE* TLSE_ordena(TLSE* lse, int (*compara)(void* a, void* b));


#endif
