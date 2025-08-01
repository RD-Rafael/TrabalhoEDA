#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>



int order(void * a, void* b){
    return 1; //Só adiciona ao final
}

int deal_with_same_input(FILE* arq_hash, void* new_data, void* file_data){

    Champion* new = (Champion*)new_data;
    Champion* old = (Champion*)file_data;


    int i = 0;
    for (i = 0; i < 34; i++)
    {
        if(old->ano[i] == 0) break;
    }

    old->ano[i] = new->ano[0];


    long current_pos = ftell(arq_hash);
    fseek(arq_hash, current_pos - sizeof(Champion), SEEK_SET);
    fwrite(old, sizeof(Champion), 1, arq_hash);
    
    return 1; 
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
    Champion champions[15] = {0};

    char linha[1000];

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo

    int i = 0;

    while (fgets(linha, sizeof(linha), arq_dados))
    {   
        substituir_traco(linha);
        // printf("%s\n", linha);

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
                champions[i].ano[0] = ano;
                champions[i].prox = -1; 

              
                HASH_inserir_generica(nome_arq_hash, &champions[i], offsetof(Champion, prox), sizeof(Champion), i, ord_func, deal_with_same_input);


            }
            
        }

        // break;

        i++;

        // if(i > 20) break;
    }
    
    // HASH_print(nome_arq_hash, 15, sizeof(Champion), offsetof(Champion, prox));

    // printf("%d e %d", sizeof(Champion), offsetof(Champion, prox));
    

    fclose(arq_dados);


}

int hashing(void* chave){
    return 1;
}


int main(){

    Champion sentinela2 = { 
        .chave = "-", 
        .ano = {0},           //Inicializa todo o array com 0
        .prox = INT_MIN 
    };
    


    HASH_inicializa_generica("../arquivos/champions.txt", "hash_por_torneio.hash", 15, sizeof(Champion), &sentinela2, preenche_hash_campeoes_por_torneio, hashing, order);

    HASH_print("hash_por_torneio.hash", 15, sizeof(Champion), offsetof(Champion, prox));
    

    return 0;
}