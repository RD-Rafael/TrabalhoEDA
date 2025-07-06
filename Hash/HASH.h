#ifndef HASH_H
#define HASH_H

#include "../ATLETA.h"
#include "../TLSE/TLSE.h"


void HASH_inicializa(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int hash_func(void* chave), int ord_func(void* a, void* b));

void HASH_inicializa_champions(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int hash_func(void* chave), int ord_func(void* a, void* b));
// void HASH_remove(char* nome_arq_hash, TAtleta* atleta, int hash_func(void* chave));

TLSE* HASH_busca(char* nome_arq_hash, TAtleta atleta, int hash_func(void* chave));

void HASH_print(char* nome_arq, int hash_size);

int hash_nacionalidade(void* chave);

void HASH_remove_global(void* data);

void HASH_inicializa_generica(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int register_size, void* sentinela, void (*preenche_hash)(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)), int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b));

void HASH_inserir_generica(char* nome_arq_hash, void* data, int prox_offset, int register_size,  int hash, int ord_func(void* a, void* b));

#endif 