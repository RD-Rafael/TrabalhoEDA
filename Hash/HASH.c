#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"

/*A fazer:
    Planejar diferencição entre as funções hash(ou não);
    Precisamos lidar com a quantidade de semanas no melhor ranking
    Dar fclose nos arquivos
*/

typedef struct data{
    char nome[25];
    int prox;
}Data;

int get_hash(TAtleta atleta, int hash_size){
    return (atleta.anoNascimento - 1960)%hash_size;
}

void print_atleta(TAtleta atleta){
    printf("%s - ", atleta.nome);
    printf("%d - ", atleta.anoNascimento);
    printf("%d - ", atleta.anoMorte);
    printf("%s - ", atleta.nacionalidade);
    printf("%d - ", atleta.rank);
    printf("%d - ", atleta.anoMelhorRank);
    printf("%d\n", atleta.x);

}

void HASH_inserir(FILE* arq_hash, TAtleta atleta, int hash_size){

    int hash = get_hash(atleta, hash_size);


    char nome[25];

    Data new;
    fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
    fread(&new, sizeof(Data), 1, arq_hash);



    if(strcmp(new.nome, "-") == 0){
        fseek(arq_hash, sizeof(Data)*hash, SEEK_SET);
        strcpy(new.nome, atleta.nome);
        new.prox = -1;
        fwrite(&new, sizeof(Data), 1, arq_hash);
    }


    // printf("%d\n", get_hash(atleta));
}

void inicializa_arq_hash_vazio(FILE* arq_hash, int hash_size){

    Data new = {.nome = "-", .prox = -1};

    for (int i = 0; i < hash_size; i++)
    {
        fwrite(&new, sizeof(Data), 1, arq_hash);
    }

    fseek(arq_hash, 0, SEEK_SET);

    printf("Arquivo Hash vazio inicializado\n");


}

void HASH_inicializa(char* nome_arq_dados, char* nome_arq_hash, int hash_size){

    FILE* arq_dados = fopen(nome_arq_dados, "r");
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

        // print_atleta(atleta);

        HASH_inserir(arq_hash, atleta, hash_size);


    }

    Data k;

    fseek(arq_hash, 0, SEEK_SET);

    for (int i = 0; i < hash_size; i++)
    {
        fread(&k, sizeof(Data), 1, arq_hash);

        printf("%s\n", k.nome);
    }
    
    
    

   



}