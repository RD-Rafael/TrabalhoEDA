#include "ATLETA.h"

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