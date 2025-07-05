#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>
#include <string.h>


int order(void * a, void* b){

    return strcmp(((TAtleta*)(a))->nome, ((TAtleta*)(b))->nome);
}

int main(){

    HASH_inicializa("../arquivos/tennis_players.txt", "paises", 38, hash_nacionalidade, order);
    // HASH_champions_inicializa("../arquivos/champions.txt", "teste_champions", 10, hashing, order);



    // TAtleta atleta;
    // strcpy(atleta.nome, "Mikael Pernfors");
    // atleta.anoNascimento = 1963;
    // // Anders Järryd\1961\-\Sweden\5\1985


    // // HASH_remove("bmbers.txt", atleta, hashing);

    HASH_print("paises", 38);
    // char name[50];
    // HASH_busca("bmbers.txt", atleta, hashing, name);

    // printf("%s\n",name);

    return 0;
}