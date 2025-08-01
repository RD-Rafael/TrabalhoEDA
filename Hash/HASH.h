#ifndef HASH_H
#define HASH_H

#include "../ATLETA.h"
#include "../TLSE/TLSE.h"

typedef struct data{
    char chave[35];
    int prox;
}Data;

typedef struct champion{
    char chave[35];
    int ano[34];
    int prox;
}Champion;

typedef struct championsbyyear{
    char chave[35];
    char torneio[35];
    int pontos;
    int prox;
}ChampionsByYear;

typedef struct championsbyyearteste{
    char chave[35];
    char torneio[15][35];
    int pontos[15];
    int prox;
}ChampionsByYearTeste;



void HASH_print(char* nome_arq, int hash_size, int register_size, int prox_offset);

int hash_nacionalidade(void* chave);
int hash_ano(void* chave);


void HASH_remove_global(void* data);

void HASH_inicializa_generica(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int register_size, void* sentinela, void (*preenche_hash)(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)), int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b));

void HASH_inserir_generica(char* nome_arq_hash, void* data, int prox_offset, int register_size,  int hash, int ord_func(void* a, void* b), int (*deal_with_same_input)(FILE* arq_hash, void* new_data, void* file_data));

TLSE* HASH_busca_generica(char* nome_arq_hash, void* data, int register_size, int prox_offset, int hash_func(void* chave));

TLSE* HASH_busca_com_hash(char* nome_arq_hash, int register_size, int prox_offset, int hash);

void cria_hash_campeoes_por_ano_teste(char* caminho_arq_dados, char* caminho_dest_hash);
void cria_hash_por_ano(char* caminho_arq_dados, char* caminho_dest_hash);
void cria_hash_por_torneio(char* caminho_arq_dados, char* caminho_dest_hash);
void cria_hash_nacionalidade(char* caminho_arq_dados, char* caminho_dest_hash);

#endif 