#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ATLETA_H
#define ATLETA_H
typedef struct _atleta{
    char chave[35];
    char nome[35];
    int anoNascimento;
    int anoMorte; // -1 significa que está vivo
    char nacionalidade[20];
    // rank == -1 <=> anoMelhorRank == -1
    //-1 significa que nunca entrou no rank
    int rank;
    int anoMelhorRank;
    int semanasTop1; // -1 se nunca foi top 1
    int x; //Experimentando
} TAtleta;

TAtleta* AtletaInit();
TAtleta* novoAtleta(char* nome, int anoNascimento, int anoMorte, char* nacionalidade, int rank, int anoMelhorRank, int semanasTop1);
void liberaAtleta(TAtleta* atleta);

#endif