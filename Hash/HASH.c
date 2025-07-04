#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"
#include <limits.h>

/*A fazer:
    Precisamos lidar com a quantidade de semanas no melhor ranking
*/

typedef struct data{
    char nome[35];
    int prox;
}Data;


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
        strcpy(aux.nome, atleta.nome);
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



        strcpy(aux.nome, atleta.nome);
        aux.prox = -1;

        fseek(arq_hash, 0, SEEK_END);
        fwrite(&aux, sizeof(Data), 1, arq_hash);

    }
    else{
        Data new;

        strcpy(new.nome, atleta.nome);

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
               &atleta.x);

       
        if(qtd_res == 6) atleta.x = -1;


        if(strcmp(anoMorte_buffer, "-") == 0) atleta.anoMorte = -1;
        else atleta.anoMorte = atoi(anoMorte_buffer);

        if(strcmp(rank_buffer, "-") == 0) atleta.rank = -1;
        else atleta.rank = atoi(rank_buffer);

        if(strcmp(anoRank_buffer, "-") == 0) atleta.anoMelhorRank = -1;
        else atleta.anoMelhorRank = atoi(anoRank_buffer);

        HASH_inserir(arq_hash, atleta, hash_func, ord_func);

        i++;

    if(i >= 20) break;

    }

    printf("Qtd de atletas adicionados: %d\n", i);

    fclose(arq_dados);
    fclose(arq_hash);

}

void HASH_remove(char* nome_arq_hash, TAtleta atleta, int hash_func(void* chave)){

    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    int hash = hash_func(&atleta);

    Data aux;
    Data ant;
    strcpy(ant.nome, "-");
    ant.prox = INT_MIN;

    fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
    fread(&aux, sizeof(Data), 1, arq_hash);

    int offset_atual = 0;
    int offset_ant = 0;
    while(strcmp(aux.nome, atleta.nome) != 0 && aux.prox != -1 && aux.prox != INT_MIN){

        strcpy(ant.nome, aux.nome);
        ant.prox = aux.prox;

        offset_ant = offset_atual;
        offset_atual = aux.prox;

        fseek(arq_hash, offset_atual, SEEK_SET);
        fread(&aux, sizeof(Data), 1, arq_hash);


    }


    if(strcmp(aux.nome, atleta.nome) == 0){
        printf("\nOlha aqui o ant! Nome: %s\n", ant.nome);
        printf("\nAchei! Nome: %s\n", aux.nome);

        ant.prox = aux.prox;
        fseek(arq_hash, offset_ant, SEEK_SET);
        fwrite(&ant, sizeof(Data), 1, arq_hash);
        printf("Removido\n");

        strcpy(aux.nome, "-");
        aux.prox = INT_MIN; //Marcando como dado morto
        fseek(arq_hash, offset_atual, SEEK_SET);
        fwrite(&aux, sizeof(Data), 1, arq_hash);

        
        

    }
    else{
        printf("\nAtleta não encontrado na tabela %s\n", nome_arq_hash);

    }

    fclose(arq_hash);
}

void HASH_busca(char* nome_arq_hash, TAtleta atleta, int hash_func(void* chave), char * name){

    FILE* arq_hash = fopen(nome_arq_hash, "rb+");


    int hash = hash_func(&atleta);

    Data aux;

    fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
    fread(&aux, sizeof(Data), 1, arq_hash);

    int offset = 0;
    while(strcmp(aux.nome, atleta.nome) != 0 && aux.prox != -1 && aux.prox != INT_MIN){


        offset = aux.prox;

        fseek(arq_hash, offset, SEEK_SET);
        fread(&aux, sizeof(Data), 1, arq_hash);


    }


    if(strcmp(aux.nome, atleta.nome) == 0){
        printf("\nAchei! Nome: %s\n", aux.nome);
        strcpy(name, aux.nome);
    }
    else{
        printf("\nAtleta não encontrado na tabela %s\n", nome_arq_hash);
        strcpy(name, "Not found");
    }

    fclose(arq_hash);



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
