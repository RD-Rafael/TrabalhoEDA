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

typedef struct champion{
    char chave[35];
    int ano;
    int prox;
}Champion;


int order(void * a, void* b){

    // Data* data_a = (Data*)a;
    // Data* data_b = (Data*)b;
    
    // return strcmp(data_a->nome, data_b->nome);
    return 1;
}



int hashing_ganhadores_por_ano(void* chave){
    Champion* data = (Champion*)chave;

    return (data->ano - 1990)%34;
}


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

// Função para substituir todas as ocorrências de "-" por "INEXISTENTE"
void substituir_traco(char* linha) {
    char* pos;
    char buffer[1000];
    char* src = linha;
    char* dst = buffer;
    
    while ((pos = strstr(src, "\\-\\")) != NULL) {
        // Copia tudo até o "-"
        int len_antes = pos - src;
        memcpy(dst, src, len_antes);
        dst += len_antes;
        
        // Adiciona "INEXISTENTE" em vez de "-"
        strcpy(dst, "\\INEXISTENTE ()\\");
        dst += strlen("\\INEXISTENTE ()\\");
        
        // Move para depois do "-"
        src = pos + 3; // pula "\\-\\"
    }
    
    // Copia o resto da string
    strcpy(dst, src);
    
    // Copia de volta para a linha original
    strcpy(linha, buffer);
}

void preenche_hash_campeoes_por_torneio(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)){

    FILE* arq_dados = fopen(nome_arq_dados, "r");

    

    int ano;
    Champion champions[15];

    char linha[1000];

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo

    

    while (fgets(linha, sizeof(linha), arq_dados))
    {   
        substituir_traco(linha);
        printf("%s\n", linha);

         int ok = sscanf(linha,
            "%d"                      /* ano */
            "\\%[^\\ (]%*[^\\]"      /* 0  Australian Open  */
            "\\%[^\\ (]%*[^\\]"      /* 1  French Open      */
            "\\%[^\\ (]%*[^\\]"      /* 2  Wimbledon        */
            "\\%[^\\ (]%*[^\\]"      /* 3  US Open          */
            "\\%[^\\ (]%*[^\\]"      /* 4  ATP Finals       */
            "\\%[^\\ (]%*[^\\]"      /* 5  Olympic Games    */
            "\\%[^\\ (]%*[^\\]"      /* 6  Indian Wells     */
            "\\%[^\\ (]%*[^\\]"      /* 7  Miami            */
            "\\%[^\\ (]%*[^\\]"      /* 8  Monte Carlo      */
            "\\%[^\\ (]%*[^\\]"      /* 9  Madrid           */
            "\\%[^\\ (]%*[^\\]"      /* 10 Rome            */
            "\\%[^\\ (]%*[^\\]"      /* 11 Canada          */
            "\\%[^\\ (]%*[^\\]"      /* 12 Cincinnati      */
            "\\%[^\\ (]%*[^\\]"      /* 13 Shanghai        */
            "\\%[^\\ (]%*[^\\]"      /* 14 Paris           */
            , &ano,
            champions[0].chave, champions[1].chave, champions[2].chave, champions[3].chave, champions[4].chave,
            champions[5].chave, champions[6].chave, champions[7].chave, champions[8].chave, champions[9].chave,
            champions[10].chave, champions[11].chave, champions[12].chave, champions[13].chave, champions[14].chave);

        for (int i = 0; i < 15; i++)
        {   
            if(strcmp(champions[i].chave, "INEXISTENTE") != 0){
                champions[i].ano = ano;
                champions[i].prox = -1; 

              
                HASH_inserir_generica(nome_arq_hash, &champions[i], offsetof(Champion, prox), sizeof(Champion), i, ord_func);


            }
            
        }

        // break;

    }
    
    // HASH_print(nome_arq_hash, 15, sizeof(Champion), offsetof(Champion, prox));

    printf("%d e %d", sizeof(Champion), offsetof(Champion, prox));
    

    fclose(arq_dados);


}


int main(){


    Data sentinela1 = {.nome = "-", .prox = INT_MIN};
    Champion sentinela2 = { .chave = "-", .ano = -1, .prox = INT_MIN };

//    HASH_inicializa_generica("../arquivos/tennis_players.txt", "hash_por_nacionalidade.hash", 50, sizeof(Data), &sentinela1, preenche_hash, hash_nacionalidade, order);


    HASH_inicializa_generica("../arquivos/champions.txt", "hash_por_torneio.hash", 15, sizeof(Champion), &sentinela2, preenche_hash_campeoes_por_torneio, hashing_ganhadores_por_ano, order);

    HASH_print("hash_por_torneio.hash", 15, sizeof(Champion), offsetof(Champion, prox));
    

    return 0;
}