#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"
#include <limits.h>

/*A fazer:
    Planejar diferencição entre as funções hash(ou não);
    Precisamos lidar com a quantidade de semanas no melhor ranking
    Dar fclose nos arquivos
    Inserir ordenado de acordo com a característica sendo avaliada
*/

typedef struct data{
    char nome[25];
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

void print_atleta(TAtleta atleta){
    printf("%s - ", atleta.nome);
    printf("%d - ", atleta.anoNascimento);
    printf("%d - ", atleta.anoMorte);
    printf("%s - ", atleta.nacionalidade);
    printf("%d - ", atleta.rank);
    printf("%d - ", atleta.anoMelhorRank);
    printf("%d\n", atleta.x);

}



void HASH_inserir(FILE* arq_hash, TAtleta atleta, int hash_size, int hash_func(void* chave)){

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
    while (aux.prox != -1)
    {   
        fseek(arq_hash, aux.prox, SEEK_SET);

        fread(&aux, sizeof(Data), 1, arq_hash);
    }

    fseek(arq_hash, -sizeof(Data), SEEK_CUR);
    int ant = ftell(arq_hash);
    fseek(arq_hash, 0, SEEK_END);

    aux.prox = ftell(arq_hash); 
    fseek(arq_hash, ant, SEEK_SET);
    fwrite(&aux, sizeof(Data), 1, arq_hash);


    strcpy(aux.nome, atleta.nome);
    aux.prox = -1;

    fseek(arq_hash, 0, SEEK_END);
    fwrite(&aux, sizeof(Data), 1, arq_hash);
    


}



void HASH_inicializa(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int hash_func(void* chave)){

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

        
        // print_atleta(atleta);

        HASH_inserir(arq_hash, atleta, hash_size, hash_func);

        i++;

        // if(i >= 2) break;

    }

    printf("Qtd de atletas: %d\n", i);


    Data k;

    fseek(arq_hash, 0, SEEK_SET);


    while(fread(&k, sizeof(Data), 1, arq_hash) != 0)
    {
        printf("%s\n", k.nome);
    
    }

    
    
    

   



}