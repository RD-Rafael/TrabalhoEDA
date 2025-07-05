#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"
#include "../TLSE/TLSE.h"
#include <limits.h>


typedef struct hashs{
    char nome_arq_hash[30];
    int (*hash_func)(void* chave);
}Hash;

typedef struct data{
    char nome[35];
    int prox;
}Data;

int hash_nacionalidade(void* chave) {
    TAtleta* atleta = (TAtleta*)chave;
    
    if (strcmp(atleta->nacionalidade, "Argentina") == 0) return 1;
    if (strcmp(atleta->nacionalidade, "Australia") == 0) return 2;
    if (strcmp(atleta->nacionalidade, "Austria") == 0) return 3;
    if (strcmp(atleta->nacionalidade, "Belgium") == 0) return 4;
    if (strcmp(atleta->nacionalidade, "Brazil") == 0) return 5;
    if (strcmp(atleta->nacionalidade, "Canada") == 0) return 6;
    if (strcmp(atleta->nacionalidade, "Chile") == 0) return 7;
    if (strcmp(atleta->nacionalidade, "Croatia") == 0) return 8;
    if (strcmp(atleta->nacionalidade, "Czech Republic") == 0) return 9;
    if (strcmp(atleta->nacionalidade, "Ecuador") == 0) return 10;
    if (strcmp(atleta->nacionalidade, "Finland") == 0) return 11;
    if (strcmp(atleta->nacionalidade, "France") == 0) return 12;
    if (strcmp(atleta->nacionalidade, "Georgia") == 0) return 13;
    if (strcmp(atleta->nacionalidade, "Germany") == 0) return 14;
    if (strcmp(atleta->nacionalidade, "Great Britain") == 0) return 15;
    if (strcmp(atleta->nacionalidade, "Greece") == 0) return 16;
    if (strcmp(atleta->nacionalidade, "Haiti") == 0) return 17;
    if (strcmp(atleta->nacionalidade, "Hungary") == 0) return 18;
    if (strcmp(atleta->nacionalidade, "India") == 0) return 19;
    if (strcmp(atleta->nacionalidade, "Israel") == 0) return 20;
    if (strcmp(atleta->nacionalidade, "Italy") == 0) return 21;
    if (strcmp(atleta->nacionalidade, "Latvia") == 0) return 22;
    if (strcmp(atleta->nacionalidade, "Morocco") == 0) return 23;
    if (strcmp(atleta->nacionalidade, "Netherlands") == 0) return 24;
    if (strcmp(atleta->nacionalidade, "Norway") == 0) return 25;
    if (strcmp(atleta->nacionalidade, "Peru") == 0) return 26;
    if (strcmp(atleta->nacionalidade, "Poland") == 0) return 27;
    if (strcmp(atleta->nacionalidade, "Romania") == 0) return 28;
    if (strcmp(atleta->nacionalidade, "Russia") == 0) return 29;
    if (strcmp(atleta->nacionalidade, "Serbia") == 0) return 30;
    if (strcmp(atleta->nacionalidade, "Slovakia") == 0) return 31;
    if (strcmp(atleta->nacionalidade, "South Africa") == 0) return 32;
    if (strcmp(atleta->nacionalidade, "Spain") == 0) return 33;
    if (strcmp(atleta->nacionalidade, "Sweden") == 0) return 34;
    if (strcmp(atleta->nacionalidade, "Switzerland") == 0) return 35;
    if (strcmp(atleta->nacionalidade, "United States") == 0) return 36;
    if (strcmp(atleta->nacionalidade, "Uruguay") == 0) return 37;
    if (strcmp(atleta->nacionalidade, "Zimbabwe") == 0) return 38;
    
    // País não encontrado - tratamento de erro
    // printf("ERRO: País não reconhecido: %s\n", atleta->nacionalidade);
    return 0;
}



Data* new_Data(){
    Data* new = (Data*)malloc(sizeof(Data));
    return new;
}


void inicializa_arq_hash_vazio(FILE* arq_hash, int hash_size){

    Data new = {.nome = "-", .prox = INT_MIN};

    for (int i = 0; i < hash_size; i++)
    {
        fwrite(&new, sizeof(Data), 1, arq_hash);
    }

    fseek(arq_hash, 0, SEEK_SET);

    printf("Arquivo Hash vazio inicializado\n");


}

void HASH_inserir(FILE* arq_hash, TAtleta atleta, int hash_func(void* chave), int ord_func(void* a, void* b)){

    int hash = hash_func(&atleta);

    Data aux;
    fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
    fread(&aux, sizeof(Data), 1, arq_hash);

    //Espaço vazio, pode escrever ali
    if(aux.prox == INT_MIN){
        fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
        strcpy(aux.nome, atleta.chave);
        aux.prox = -1;
        fwrite(&aux, sizeof(Data), 1, arq_hash);
        fseek(arq_hash, 0, SEEK_SET);
        return;
    }

    //Colisão, percorrer lista até o fim
    while (ord_func(&atleta, &aux) > 0 && aux.prox != -1)
    {
        fseek(arq_hash, aux.prox, SEEK_SET);

        fread(&aux, sizeof(Data), 1, arq_hash);
    }


    fseek(arq_hash, -sizeof(Data), SEEK_CUR);
    int ant = ftell(arq_hash);
    fseek(arq_hash, 0, SEEK_END);



    if(aux.prox == -1 && ord_func(&atleta, &aux) > 0){

        aux.prox = ftell(arq_hash);
        fseek(arq_hash, ant, SEEK_SET);
        fwrite(&aux, sizeof(Data), 1, arq_hash);
        fseek(arq_hash, 0, SEEK_END);



        strcpy(aux.nome, atleta.chave);
        aux.prox = -1;

        fseek(arq_hash, 0, SEEK_END);
        fwrite(&aux, sizeof(Data), 1, arq_hash);

    }
    else{
        Data new;

        strcpy(new.nome, atleta.chave);

        new.prox = ftell(arq_hash);

        fwrite(&aux, sizeof(Data), 1, arq_hash);

        fseek(arq_hash, ant, SEEK_SET);

        fwrite(&new, sizeof(Data), 1, arq_hash);

    }
}

void HASH_inicializa(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int hash_func(void* chave), int ord_func(void* a, void* b)){

    FILE* arq_dados = fopen(nome_arq_dados, "r");
    // FILE* arq_hash = fopen(nome_arq_hash, "rb+");
    FILE* arq_hash = fopen(nome_arq_hash, "wb+");

    if(!arq_dados) {
        printf("ERRO: Arquivo de dados não encontrado, abortando...\n");
        exit(1);
    }

    // if(arq_hash) {
    //     printf("AVISO: Já existe um arquivo de hashing com esse nome, deseja substituí-lo? \nDigite 'S' para continuar ou 'N' para abortar\n");

    //     char res = ' ';

    //     scanf("%c", &res);

    //     if(res == 'N') exit(1);

    //     fclose(arq_hash);
    //     arq_hash = fopen(nome_arq_hash, "wb+");

    // }

    inicializa_arq_hash_vazio(arq_hash, hash_size);



    TAtleta atleta;

    int x = 0;

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo

    char anoMorte_buffer[10];
    char rank_buffer[10];
    char anoRank_buffer[10];

    char linha[300];

    int i = 0;

    while(fgets(linha, sizeof(linha)/sizeof(linha[0]),arq_dados ) != NULL){

        int qtd_res = sscanf(linha, "%[^\\]\\%d\\%[^\\]\\%[^\\]\\%[^\\]\\%s (%d)",
               atleta.nome,
               &atleta.anoNascimento,
               anoMorte_buffer,
               atleta.nacionalidade,
               rank_buffer,
               anoRank_buffer,
               &atleta.semanasTop1);


        if(qtd_res == 6) atleta.semanasTop1 = -1;

        int inicioSobrenome = 0;
        int aux = -1;
        for(int i = 0; i < 35; i++){
            if(atleta.nome[i] == '\0') break;
            else if(atleta.nome[i] == ' '){
                aux = i;
            } else{
                inicioSobrenome = aux+1;
            }
        }

        strcpy(atleta.chave, &atleta.nome[inicioSobrenome]);

        // printf("%s\n", atleta.chave);


        if(strcmp(anoMorte_buffer, "-") == 0) atleta.anoMorte = -1;
        else atleta.anoMorte = atoi(anoMorte_buffer);

        if(strcmp(rank_buffer, "-") == 0) atleta.rank = -1;
        else atleta.rank = atoi(rank_buffer);

        if(strcmp(anoRank_buffer, "-") == 0) atleta.anoMelhorRank = -1;
        else atleta.anoMelhorRank = atoi(anoRank_buffer);

        HASH_inserir(arq_hash, atleta, hash_func, ord_func);

        i++;

    // if(i >= 20) break;

    }

    printf("Qtd de atletas adicionados: %d\n", i);

    fclose(arq_dados);
    fclose(arq_hash);

}

void HASH_remove(char* nome_arq_hash, TAtleta* atleta, int hash_func(void* chave)){

    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    int hash = hash_func(atleta);

    Data aux;
    Data ant;
    strcpy(ant.nome, "-");
    ant.prox = INT_MIN;

    fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
    fread(&aux, sizeof(Data), 1, arq_hash);

    int offset_atual = sizeof(Data)*hash;  
    int offset_ant = -1; 
    
    while(strcmp(aux.nome, atleta->chave) != 0 && aux.prox != -1 && aux.prox != INT_MIN){

        strcpy(ant.nome, aux.nome);
        ant.prox = aux.prox;

        offset_ant = offset_atual;  
        offset_atual = aux.prox;  

        fseek(arq_hash, offset_atual, SEEK_SET);
        fread(&aux, sizeof(Data), 1, arq_hash);
    }

    if(strcmp(aux.nome, atleta->chave) == 0){

        if(offset_ant != -1) {
            ant.prox = aux.prox;
            fseek(arq_hash, offset_ant, SEEK_SET);
            fwrite(&ant, sizeof(Data), 1, arq_hash);
        } 
        else {
            Data vazio = {.nome = "-", .prox = INT_MIN};
            fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
            fwrite(&vazio, sizeof(Data), 1, arq_hash);
        }

        if(offset_atual != sizeof(Data)*hash) {
            strcpy(aux.nome, "-");
            aux.prox = INT_MIN;
            fseek(arq_hash, offset_atual, SEEK_SET);
            fwrite(&aux, sizeof(Data), 1, arq_hash);
        }

    }
    else{
        printf("\nAtleta não encontrado na tabela %s\n", nome_arq_hash);
    }

    fclose(arq_hash);
}

void HASH_remove_global(TAtleta* atleta){

    Hash tabelas_sistema[] = {
    {"Hash/paises.hash", hash_nacionalidade},
};

    for (int i = 0; i < sizeof(tabelas_sistema)/sizeof(tabelas_sistema[0]); i++)
    {
        HASH_remove(tabelas_sistema[i].nome_arq_hash, atleta, tabelas_sistema[i].hash_func);
    }


}

TLSE* HASH_busca(char* nome_arq_hash, TAtleta atleta, int hash_func(void* chave)){

    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    if(!arq_hash){
        printf("Arquivo Hash não existe\n");
        exit(1);
    }


    TLSE* lse = TLSE_inicializa();


    int hash = hash_func(&atleta);

    Data* aux = new_Data();

    fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
    fread(aux, sizeof(Data), 1, arq_hash);

    lse = TLSE_insere_inicio(lse, aux);


    int offset = 0;
    while(aux->prox != -1 && aux->prox != INT_MIN ){


        offset = aux->prox;

        fseek(arq_hash, offset, SEEK_SET);

        aux = new_Data();

        fread(aux, sizeof(Data), 1, arq_hash);

        lse = TLSE_insere_inicio(lse, aux);

    }


    fclose(arq_hash);

    return lse;


}

void HASH_print(char* nome_arq, int hash_size){
    FILE* arq_hash = fopen(nome_arq, "rb");

    Data aux;

    for (int i = 0; i < hash_size; i++)
    {
        fseek(arq_hash, sizeof(Data)*i, SEEK_SET);
        fread(&aux, sizeof(Data), 1, arq_hash);
        printf("%s", aux.nome);

        while (aux.prox != INT_MIN && aux.prox != -1 )
        {
            printf("-->");
            fseek(arq_hash, aux.prox, SEEK_SET);
            fread(&aux, sizeof(Data), 1, arq_hash);
            printf("%s", aux.nome);
        }

        printf("\n");
    }

    fclose(arq_hash);

}

