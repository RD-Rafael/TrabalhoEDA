#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>
#include <string.h>

int hashing(void * chave){
    return (((TAtleta*)(chave))->anoNascimento)%100;
}

int order(void * a, void* b){

    return strcmp(((TAtleta*)(a))->nome, ((TAtleta*)(b))->nome);
}

int main(){

    HASH_inicializa("../arquivos/tennis_players.txt", "bmbers.txt", 100, hashing, order);

    HASH_print("bmbers.txt", 100);
    return 0;
}