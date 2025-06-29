#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>
#include <string.h>

int hashing(void * chave){
    return (((TAtleta*)(chave))->anoNascimento)%10;
}

int order(void * a, void* b){

    return strcmp(((TAtleta*)(a))->nome, ((TAtleta*)(b))->nome);
}

int main(){

    HASH_inicializa("../arquivos/tennis_players.txt", "bmbers.txt", 10, hashing, order);



    TAtleta atleta;
    strcpy(atleta.nome, "Mikael Pernfors");
    atleta.anoNascimento = 1963;
    // Anders Järryd\1961\-\Sweden\5\1985


    // HASH_remove("bmbers.txt", atleta, hashing);

    // HASH_print("bmbers.txt", 10);
    char name[50];
    HASH_busca("bmbers.txt", atleta, hashing, name);

    printf("%s\n",name);

    return 0;
}