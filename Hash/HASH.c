#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"

/*A fazer:
    Planejar diferencição entre as funções hash(ou não);
    Precisamos lidar com a quantidade de semanas no melhor ranking
*/

int get_hash(TAtleta atleta){
    return atleta.anoNascimento - 1985;
}

void HASH_inicializa(char* nome_arq_dados, char* nome_arq_hash, int hash_size){

    FILE* arq_dados = fopen(nome_arq_dados, "rb");
    FILE* arq_hash = fopen(nome_arq_hash, "rb+");

    if(!arq_dados) {
        printf("ERRO: Arquivo de dados não encontrado, abortando...\n");
        exit(1);
    }

    if(arq_hash) {
        printf("AVISO: Já existe um arquivo de hashing com esse nome, deseja substituí-lo? \nDigite 'S' para continuar ou 'N' para abortar\n");

        char res = ' ';

        scanf("%c", &res);

        if(res == 'N') exit(1);
    }

    TAtleta atleta;

    int x = 0;

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo

    char anoMorte_buffer[10];
    char rank_buffer[10];
    char anoRank_buffer[10];

    while(fscanf(arq_dados, "%[^\\]\\%d\\%[^\\]\\%[^\\]\\%[^\\]\\%s", atleta.nome, &atleta.anoNascimento, anoMorte_buffer, atleta.nacionalidade, rank_buffer, anoRank_buffer) != EOF){

        fscanf(arq_dados," (%d)", &x); //Tenta capturar semanas no ranking

        fgetc(arq_dados); //Captura \n

        if(strcmp(anoMorte_buffer, "-") == 0) atleta.anoMorte = -1;
        else atleta.anoMorte = atoi(anoMorte_buffer);

        if(strcmp(rank_buffer, "-") == 0) atleta.rank = -1;
        else atleta.rank = atoi(rank_buffer);

        if(strcmp(anoRank_buffer, "-") == 0) atleta.anoMelhorRank = -1;
        else atleta.anoMelhorRank = atoi(anoRank_buffer);

        printf("%s - ", atleta.nome);
        printf("%s\n", atleta.nacionalidade);


    }



}