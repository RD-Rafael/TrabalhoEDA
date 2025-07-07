#ifndef TLSE_H
#define TLSE_H
#include "../ATLETA.h"

// typedef struct ChampionsByYear ChampionsByYear;

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
void TLSE_print_maiores_campeoes(TLSE* lse);
void TLSE_print_teste(TLSE* lse);
TLSE* TLSE_ordena(TLSE* lse, int (*compara)(void* a, void* b));
TLSE* TLSE_ordena_ATP_Finals(TLSE* lse, TLSE* lse_ant,  int (*compara)(void* a, void* b, TLSE* lse_ant));
TLSE* TLSE_copy(TLSE* lse);
TLSE* TLSE_cria_lista_fake_1990_simples();
TLSE* TLSE_insere_nao_duplicado(TLSE* lse, void* info);


#endif
