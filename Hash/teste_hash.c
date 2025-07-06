#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

 typedef struct data{
    char nome[35];
    int prox;
    }Data;



int order(void * a, void* b){

    // Data* data_a = (Data*)a;
    // Data* data_b = (Data*)b;
    
    // return strcmp(data_a->nome, data_b->nome);
    return 1;
}

int hashing(void* chave){return 10;}


void preenche_hash(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)){
    printf("kkk");

    FILE* arq_dados = fopen(nome_arq_dados, "r");

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


        if(strcmp(anoMorte_buffer, "-") == 0) atleta.anoMorte = -1;
        else atleta.anoMorte = atoi(anoMorte_buffer);

        if(strcmp(rank_buffer, "-") == 0) atleta.rank = -1;
        else atleta.rank = atoi(rank_buffer);

        if(strcmp(anoRank_buffer, "-") == 0) atleta.anoMelhorRank = -1;
        else atleta.anoMelhorRank = atoi(anoRank_buffer);

        Data data;

        strcpy(data.nome, atleta.chave);
        data.prox = -1;

        int hash = hash_func(&atleta);

        int prox_offset = offsetof(Data, prox);

        HASH_inserir_generica(nome_arq_hash, &data, prox_offset, sizeof(Data), hash, ord_func);

        i++;

    }

    fclose(arq_dados);


}

int main(){


    Data sentinela = {.nome = "-", .prox = INT_MIN};


    HASH_inicializa_generica("../tennis_players.txt", "teste_hash_gen.hash", 53, sizeof(Data), &sentinela, preenche_hash, hash_nacionalidade, order);

    return 0;
}