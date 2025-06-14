#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ATLETA_H
#define ATLETA_H
typedef struct _atleta{
    char nome[25];
    int anoNascimento;
    int anoMorte; // -1 significa que está vivo
    char nacionalidade[20];
    // rank == -1 <=> anoMelhorRank == -1
    //-1 significa que nunca entrou no rank
    int rank;
    int anoMelhorRank;
} TAtleta;

TAtleta* AtletaInit(){
    return NULL;
}
TAtleta* novoAtleta(char* nome, int anoNascimento, int anoMorte, char* nacionalidade, int rank, int anoMelhorRank){
    TAtleta* atleta = (TAtleta*) malloc(sizeof(TAtleta));
    strcpy(atleta->nome, nome);
    atleta->anoNascimento = anoNascimento;
    atleta->anoMorte = anoMorte;
    strcpy(atleta->nacionalidade, nacionalidade);
    atleta->rank = rank;
    atleta->anoMelhorRank = anoMelhorRank;
    return atleta;
}

void liberaAtleta(TAtleta* atleta){
    free(atleta);
}




#endif