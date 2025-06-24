#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>

int hashing(void * chave){
    return (((TAtleta*)(chave))->anoNascimento - 1960)%101;
}

int main(){

    HASH_inicializa("../arquivos/tennis_players.txt", "bmbers.txt", 100, hashing);

    HASH_print("bmbers.txt", 100);
    return 0;
}