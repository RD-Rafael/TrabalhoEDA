#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>

int hashing(void * chave){
    return (((TAtleta*)(chave))->anoNascimento - 1960);
}

int main(){

    HASH_inicializa("../arquivos/tennis_players.txt", "bmbers.txt", 400, hashing);
    return 0;
}