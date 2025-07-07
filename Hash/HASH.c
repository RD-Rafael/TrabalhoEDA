#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"
#include "../TLSE/TLSE.h"
#include <limits.h>


typedef struct hashs{
    char nome_arq_hash[50];
    int (*hash_func)(void* chave);
    int register_size;
    int prox_offset;
}Hash;


typedef struct champion_delete{
    char chave[35];
    int ano;
    int prox;
}ChampionData;

int hash_nacionalidade(void *chave)
{
    const TAtleta *a = (const TAtleta *)chave;

    if (strcmp(a->nacionalidade, "Argentina")        == 0) return  1;
    else if (strcmp(a->nacionalidade, "Australia")   == 0) return  2;
    else if (strcmp(a->nacionalidade, "Austria")     == 0) return  3;
    else if (strcmp(a->nacionalidade, "Belarus")     == 0) return  4;
    else if (strcmp(a->nacionalidade, "Belgium")     == 0) return  5;
    else if (strcmp(a->nacionalidade, "Brazil")      == 0) return  6;
    else if (strcmp(a->nacionalidade, "Bulgaria")    == 0) return  7;
    else if (strcmp(a->nacionalidade, "Canada")      == 0) return  8;
    else if (strcmp(a->nacionalidade, "Chile")       == 0) return  9;
    // else if (strcmp(a->nacionalidade, "Chile Chile") == 0) return 10; /* linha duplicada no TXT */
    else if (strcmp(a->nacionalidade, "Croatia")     == 0) return 11;
    else if (strcmp(a->nacionalidade, "Cyprus")      == 0) return 12;
    else if (strcmp(a->nacionalidade, "Czech Republic") == 0) return 13;
    else if (strcmp(a->nacionalidade, "Denmark")     == 0) return 14;
    else if (strcmp(a->nacionalidade, "Ecuador")     == 0) return 15;
    else if (strcmp(a->nacionalidade, "Finland")     == 0) return 16;
    else if (strcmp(a->nacionalidade, "France")      == 0) return 17;
    else if (strcmp(a->nacionalidade, "Georgia")     == 0) return 18;
    else if (strcmp(a->nacionalidade, "Germany")     == 0) return 19;
    else if (strcmp(a->nacionalidade, "Great Britain")==0) return 20;
    else if (strcmp(a->nacionalidade, "Greece")      == 0) return 21;
    else if (strcmp(a->nacionalidade, "Haiti")       == 0) return 22;
    else if (strcmp(a->nacionalidade, "Hungary")     == 0) return 23;
    else if (strcmp(a->nacionalidade, "India")       == 0) return 24;
    else if (strcmp(a->nacionalidade, "Israel")      == 0) return 25;
    else if (strcmp(a->nacionalidade, "Italy")       == 0) return 26;
    else if (strcmp(a->nacionalidade, "Japan")       == 0) return 27;
    else if (strcmp(a->nacionalidade, "Kazakhstan")  == 0) return 28;
    else if (strcmp(a->nacionalidade, "Latvia")      == 0) return 29;
    else if (strcmp(a->nacionalidade, "Luxembourg")  == 0) return 30;
    else if (strcmp(a->nacionalidade, "Morocco")     == 0) return 31;
    else if (strcmp(a->nacionalidade, "Netherlands") == 0) return 32;
    else if (strcmp(a->nacionalidade, "Norway")      == 0) return 33;
    else if (strcmp(a->nacionalidade, "Peru")        == 0) return 34;
    else if (strcmp(a->nacionalidade, "Poland")      == 0) return 35;
    else if (strcmp(a->nacionalidade, "Romania")     == 0) return 36;
    else if (strcmp(a->nacionalidade, "Russia")      == 0) return 37;
    else if (strcmp(a->nacionalidade, "Serbia")      == 0) return 38;
    else if (strcmp(a->nacionalidade, "Slovakia")    == 0) return 39;
    else if (strcmp(a->nacionalidade, "South Africa")== 0) return 40;
    else if (strcmp(a->nacionalidade, "Spain")       == 0) return 41;
    else if (strcmp(a->nacionalidade, "Sweden")      == 0) return 42;
    else if (strcmp(a->nacionalidade, "Switzerland") == 0) return 43;
    else if (strcmp(a->nacionalidade, "Taipei")      == 0) return 44;
    else if (strcmp(a->nacionalidade, "Thailand")    == 0) return 45;
    else if (strcmp(a->nacionalidade, "Ukraine")     == 0) return 46;
    else if (strcmp(a->nacionalidade, "United States")==0) return 47;
    else if (strcmp(a->nacionalidade, "Uruguay")     == 0) return 48;
    else if (strcmp(a->nacionalidade, "Yugoslavia")  == 0) return 49;
    else if (strcmp(a->nacionalidade, "Zimbabwe")    == 0) return 50;
    else if (strcmp(a->nacionalidade, "15")          == 0) return 51;
    else if (strcmp(a->nacionalidade, "25")          == 0) return 52;
    else return 0;  
}

int hash_ano(void* chave){

    int ano = *(int*)chave;
    return (ano - 1990);
}

int hash_torneio(void *chave){
    return 1;
}



void HASH_remove(char* nome_arq_hash, void* data, int register_size, int prox_offset, int hash_func(void* chave)){

    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    int hash = hash_func(data);

    void* aux = malloc(register_size);
    void* ant = malloc(register_size);

    // strcpy(ant.nome, "-");
    // ant.prox = INT_MIN;

    fseek(arq_hash, register_size*hash, SEEK_SET);
    fread(aux, register_size, 1, arq_hash);

    int offset_atual = register_size*hash;  
    int offset_ant = -1; 
    
    int* aux_prox = (int*)((char*)aux + prox_offset);
    int* ant_prox = (int*)((char*)ant + prox_offset);

    while(strcmp(aux, data) != 0 && *aux_prox != -1 && *aux_prox != INT_MIN){

        strcpy(ant, aux);
        *ant_prox = *aux_prox;

        offset_ant = offset_atual;  
        offset_atual = *aux_prox;  

        fseek(arq_hash, offset_atual, SEEK_SET);
        fread(aux, register_size, 1, arq_hash);
        aux_prox = (int*)((char*)aux + prox_offset);
    }

    if(strcmp(aux, data) == 0){
        if(offset_ant != -1) {
            *ant_prox = *aux_prox;
            fseek(arq_hash, offset_ant, SEEK_SET);
            fwrite(ant, register_size, 1, arq_hash);
        } 
        else {
            void* vazio = malloc(register_size);
            int* vazio_prox = (int*)((char*)vazio + prox_offset);
            strcpy(vazio, "-");
            *vazio_prox = INT_MIN;
            fseek(arq_hash, register_size*hash, SEEK_SET);
            fwrite(vazio, register_size, 1, arq_hash);
            free(vazio);
        }

        if(offset_atual != register_size*hash) {
            strcpy(aux, "-");
            *aux_prox = INT_MIN;
            fseek(arq_hash, offset_atual, SEEK_SET);
            fwrite(aux, register_size, 1, arq_hash);
        }

    }
    else{
        printf("\nAtleta não encontrado na tabela %s\n", nome_arq_hash);
    }

    fclose(arq_hash);
}

void HASH_remove_global(void* data){


    Hash tabelas_sistema[] = {
    {"Hash/hash_por_nacionalidade.hash", hash_nacionalidade, 40, 36},
    // {"Hash/hash_por_torneio.hash", hash_torneio, 44, 40}, Retirar daqui faz sentido?

};

    for (int i = 0; i < sizeof(tabelas_sistema)/sizeof(tabelas_sistema[0]); i++)
    {
        HASH_remove(tabelas_sistema[i].nome_arq_hash, data, tabelas_sistema[i].register_size, tabelas_sistema[i].prox_offset, tabelas_sistema[i].hash_func);
    }


}

void HASH_print(char* nome_arq, int hash_size, int register_size, int prox_offset){
    FILE* arq_hash = fopen(nome_arq, "rb");

    void* aux = malloc(register_size);

    for (int i = 0; i < hash_size; i++)
    {
        fseek(arq_hash, register_size*i, SEEK_SET);
        fread(aux, register_size, 1, arq_hash);
        printf("%s", (char*)aux);

        int* aux_prox = (int*)((char*)aux + prox_offset);
        while (*aux_prox != INT_MIN && *aux_prox != -1 )
        {
            printf("-->");
            fseek(arq_hash, *aux_prox, SEEK_SET);
            fread(aux, register_size, 1, arq_hash);
            printf("%s", (char*)aux);
        }

        printf("\n\n\n");
    }

    fclose(arq_hash);

}

void inicializa_arq_hash_vazio_generica(char* nome_arq_hash, int hash_size, int register_size, void* sentinela){

    FILE* arq_hash = fopen(nome_arq_hash, "wb+");

    for (int i = 0; i < hash_size; i++)
    {
        fwrite(sentinela, register_size, 1, arq_hash);
    }

    fclose(arq_hash);

    printf("Arquivo Hash vazio inicializado\n");


}

void HASH_inserir_generica(char* nome_arq_hash, void* data, int prox_offset, int register_size,  int hash, int ord_func(void* a, void* b), int (*deal_with_same_input)(FILE* arq_hash, void* new_data, void* file_data)){


    // printf("%d\n", hash);
    // printf("Insercao de: %s\n", (char*)data);


    FILE* arq_hash = fopen(nome_arq_hash, "rb+");


    void* aux = malloc(register_size);

    fseek(arq_hash, register_size*hash, SEEK_SET);
    fread(aux, register_size, 1, arq_hash);

    int* aux_prox = ((int*)((char*)aux + prox_offset));
    // printf("\n\n%d\n\n", *aux_prox);


    if(*(aux_prox) == INT_MIN){

    // printf("%s entrou aqui no INT_MIN\n", (char*)data);


        
        fseek(arq_hash, register_size*hash, SEEK_SET);
        fwrite(data, register_size, 1, arq_hash);
        fseek(arq_hash, 0, SEEK_SET);
        free(aux);
        fclose(arq_hash);
        return;
    }
    printf("\n\nStrings 1: %s = %s\n\n", (char*)data, (char*)aux);
    //Colisão, percorrer lista até o fim
    while (ord_func(data, aux) > 0 && *aux_prox != -1)
    {   
    printf("\n\nStrings 2: %s = %s\n\n", (char*)data, (char*)aux);
    // printf("%s entrou aqui no INT_MIN\n", (char*)data);
     
        if(strcmp((char*)data, (char*)aux) == 0) break;

        fseek(arq_hash, *aux_prox, SEEK_SET);

        fread(aux, register_size, 1, arq_hash);

        aux_prox = ((int*)((char*)aux + prox_offset));

        if(strcmp((char*)data, (char*)aux) == 0) break;



    }

    if(strcmp((char*)data, (char*)aux) == 0){
            // printf("\n\nStrings analise: %s = %s\n\n", (char*)data, (char*)aux);

            int res = deal_with_same_input(arq_hash, data, aux);
            if(res) {
                fclose(arq_hash);
                free(aux);
                return;
            } //Usuário decide na função o que ele quer fazer nesse ponto
        }


    fseek(arq_hash, -register_size, SEEK_CUR);
    int ant = ftell(arq_hash);
    fseek(arq_hash, 0, SEEK_END);

    if(*aux_prox == -1 && ord_func(data, aux) > 0){
    // printf("%s entrou aqui no -1\n", (char*)data);


        *aux_prox = ftell(arq_hash);
        fseek(arq_hash, ant, SEEK_SET);
        fwrite(aux, register_size, 1, arq_hash);
        fseek(arq_hash, 0, SEEK_END);

        fseek(arq_hash, 0, SEEK_END);
        fwrite(data, register_size, 1, arq_hash);

    }
    else{

    // printf("%s entrou aqui no else\n", (char*)data);

    
        int * data_prox = ((int*)((char*)data + prox_offset));
        *data_prox = *aux_prox;

        *aux_prox = ftell(arq_hash);

        // printf("ftell = %d\n", *aux_prox/register_size);



        fwrite(data, register_size, 1, arq_hash);

        


        fseek(arq_hash, ant, SEEK_SET);

        fwrite(aux, register_size, 1, arq_hash);



    }

    free(aux);
    fclose(arq_hash);
}

void HASH_inicializa_generica(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int register_size, void* sentinela, void (*preenche_hash)(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)), int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)){


    inicializa_arq_hash_vazio_generica(nome_arq_hash, hash_size,register_size, sentinela);

    preenche_hash(nome_arq_dados, nome_arq_hash, hash_func, ord_func);


}

TLSE* HASH_busca_generica(char* nome_arq_hash, void* data, int register_size, int prox_offset, int hash_func(void* chave)){

    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    if(!arq_hash){
        printf("Arquivo Hash não existe\n");
        exit(1);
    }


    TLSE* lse = TLSE_inicializa();


    int hash = hash_func(data);

    void* aux = malloc(register_size);

    fseek(arq_hash, register_size*hash, SEEK_SET);
    fread(aux, register_size, 1, arq_hash);


    lse = TLSE_insere_inicio(lse, aux);

    int* aux_prox = (int*)((char*)aux + prox_offset);

    int offset = 0;
    while(*aux_prox != -1 && *aux_prox != INT_MIN ){

        offset = *aux_prox;

        fseek(arq_hash, offset, SEEK_SET);

        aux = malloc(register_size);

        fread(aux, register_size, 1, arq_hash);

        aux_prox = (int*)((char*)aux + prox_offset);

        lse = TLSE_insere_inicio(lse, aux);

        // printf("%s\n", (char*)(aux));

    }


    fclose(arq_hash);

    return lse;


}

TLSE* HASH_busca_com_hash(char* nome_arq_hash, int register_size, int prox_offset, int hash){


    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    if(!arq_hash){
        printf("Arquivo Hash não existe\n");
        exit(1);
    }


    TLSE* lse = TLSE_inicializa();



    void* aux = malloc(register_size);

    fseek(arq_hash, register_size*hash, SEEK_SET);
    fread(aux, register_size, 1, arq_hash);

    lse = TLSE_insere_inicio(lse, aux);

    int* aux_prox = (int*)((char*)aux + prox_offset);

    int offset = 0;
    while(*aux_prox != -1 && *aux_prox != INT_MIN ){

        offset = *aux_prox;

        fseek(arq_hash, offset, SEEK_SET);

        aux = malloc(register_size);

        fread(aux, register_size, 1, arq_hash);

        aux_prox = (int*)((char*)aux + prox_offset);

        lse = TLSE_insere_inicio(lse, aux);

    }


    fclose(arq_hash);

    return lse;


}


