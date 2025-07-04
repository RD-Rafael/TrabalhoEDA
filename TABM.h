#include "ATLETA.h"

#ifndef TABM_H
#define TABM_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ATLETA.h"

#define T 2
typedef struct no_arvbm{
    int nchaves;
    char chaves[(T*2)-1][25];
    int folha;
    int filhos[T*2];
    int prox;
} TABM;

void TABM_leitura_arq(char* nomeArq, char* nomeIdx);

void TABM_retira_chave_inicio(TABM* no);
char* nomeFolhaAtleta(char* nome_indice, char* chaveAtleta);
void TABM_print_no(TABM* no);
int TABM_ppl(FILE* fp);
int pos_novo_no_int(char chaves[T][25], int nchaves, char* nome_no);
int pos_novo_no_folha(char* nomeFolha, char* chaveAtleta);
int nchaves_em_folha(char* nomeFolha);
void inicializar_folha(char* nomeFolha);
TAtleta* buscaAtletaFolha(char* nomeFolha, char* chaveAtleta);
TAtleta* buscaAtletaNFolha(char* nomeFolha, int n);
int insere_atleta_folha(char* nomeFolha, TAtleta* atleta);
int folha_cheia(char* nomeFolha);
void removeAtletaFolha(char* nomeFolha, char* chaveAtleta);
void divideFolha(char * nomeFolha);
void mergeFolha(char* nomeFolhaEsq, char* nomeFolhaDir);
void TABM_ins_aux(char* nome_indice, int posAtual, int posAntiga, TAtleta* atleta);
void TABM_insere(char* nome_indice, TAtleta* atleta);
void imprimeFolha(char* nomeFolha) ;
void imprimeTABM_aux(FILE* fp, int posAtual, int nivel);
void imprimeTABM(char* nome_indice);
void TABM_inicializa(char *nome_indice);
void TABM_retira_aux(FILE* fp, long posAtual, char* chaveAtleta);
void TABM_retira(char* nome_indice, char* chaveAtleta);
TAtleta* TABM_busca(char* nome_indice, char* chaveAtleta);
#endif