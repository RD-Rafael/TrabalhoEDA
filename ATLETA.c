#include "ATLETA.h"

TAtleta* AtletaInit(){
    return NULL;
}

TAtleta* novoAtleta(char* nome, int anoNascimento, int anoMorte, char* nacionalidade, int rank, int anoMelhorRank, int semanasTop1){
    TAtleta* atleta = (TAtleta*) malloc(sizeof(TAtleta));
    memset(atleta, 0, sizeof(TAtleta));
    strcpy(atleta->nome, nome);
    atleta->anoNascimento = anoNascimento;
    atleta->anoMorte = anoMorte;
    strcpy(atleta->nacionalidade, nacionalidade);
    atleta->rank = rank;
    atleta->anoMelhorRank = anoMelhorRank;
    atleta->semanasTop1 = semanasTop1;

    //Pegar o último sobrenome para a chave

    //codigo para pegar a posição do ultimo sobrenome (tem pessoas com o mesmo último sobrenome)
    int inicioSobrenome = 0;
    int aux = -1;
    for(int i = 0; i < 35; i++){
        if(nome[i] == '\0') break;
        else if(nome[i] == ' '){
            aux = i;
        } else{
            inicioSobrenome = aux+1;
        }
    }
    // if(strcmp(nome, "Carl-Uwe Steeb") == 0) printf("\nAqui: %s\n\n", &nome[inicioSobrenome]);

    strcpy(atleta->chave, &nome[inicioSobrenome]);

    //strcpy(atleta->chave, nome);



    return atleta;
}

void liberaAtleta(TAtleta* atleta){
    free(atleta);
}