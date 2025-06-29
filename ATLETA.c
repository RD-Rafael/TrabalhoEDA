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

    //Pegar o último sobrenome para a chave


    int inicioSobrenome = 0;
    int aux = -1;
    for(int i = 0; i < 25; i++){
        if(nome[i] == '\0') break;
        else if(nome[i] == ' '){
            aux = i;
        } else{
            inicioSobrenome = aux+1;
        }
    }
    strcpy(atleta->chave, &nome[inicioSobrenome]);



    return atleta;
}

void liberaAtleta(TAtleta* atleta){
    free(atleta);
}