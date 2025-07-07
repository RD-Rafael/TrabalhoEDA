#include "../ATLETA.h"
#include "HASH.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

int obter_pontuacao_torneio(int indice_torneio) {
    switch(indice_torneio) {
        // Grand Slams
        case 0:  // Australian Open
        case 1:  // French Open
        case 2:  // Wimbledon
        case 3:  // US Open
            return 2000;
        
        // ATP Finals
        case 4:  // ATP Finals
            return 0;    // Pontuação especial/variável
        
        // Olympic Games
        case 5:  // Olympic games
            return 0;    // Sem pontuação ATP
        
        // ATP Masters 1000
        case 6:  // Indian Wells
        case 7:  // Miami
        case 8:  // Monte Carlo
        case 9:  // Madrid
        case 10: // Rome
        case 11: // Canada
        case 12: // Cincinnati
        case 13: // Shanghai
        case 14: // Paris
            return 1000;
        
        default:
            return 0;
    }
}

int order(void * a, void* b){
    return 1; //Só adiciona ao final
}

int deal_with_same_input(FILE* arq_hash, void* new_data, void* file_data){

    ChampionsByYearTeste* new = (ChampionsByYearTeste*)new_data;
    ChampionsByYearTeste* old = (ChampionsByYearTeste*)file_data;

    printf("Novo: %s | Antigo: %s\n", new->chave, old->chave);

    for (int i = 0; i < 15; i++)
    {
        printf("O que tem %s - %d", old->torneio[i], old->pontos[i]);
    }

        printf("\nQuero adicionar %s - %d", new->torneio[0], new->pontos[0]);



    printf("\n\n\n");
    

    for (int i = 0; i < 15; i++)
    {   
        if(old->pontos[i] == 0){
        printf("Adicionou o %s no %s\n", new->torneio[0], old->chave);

            old->pontos[i] = new->pontos[0];
            strcpy(old->torneio[i], new->torneio[0]);
            break;
        }
    }
    

    long current_pos = ftell(arq_hash);
    fseek(arq_hash, current_pos - sizeof(ChampionsByYearTeste), SEEK_SET);
    fwrite(old, sizeof(ChampionsByYearTeste), 1, arq_hash);
    
    return 1; 

    // return 0;
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

void preenche_hash(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)){

    const char* nomes_torneios[15] = {
    "Australian Open",
    "French Open", 
    "Wimbledon",
    "US Open",
    "ATP Finals",
    "Olympic games",
    "Indian Wells",
    "Miami",
    "Monte Carlo",
    "Madrid",
    "Rome",
    "Canada",
    "Cincinnati",
    "Shanghai",
    "Paris"
};

    FILE* arq_dados = fopen(nome_arq_dados, "r");

    

    ChampionsByYearTeste champions[15] = {0};

    for (int j = 0; j < 15; j++)
    {   
        strcpy(champions[j].chave, "INEXISTENTE");
        champions[j].prox = -1;
        for(int i = 0; i < 15; i++) {
        strcpy(champions[j].torneio[i], "-");
                champions[j].pontos[i] = 0;
    }
    }
    

    char linha[1000];

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo

    int ano;

    int i =0;

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
                strcpy(champions[i].torneio[0], nomes_torneios[i]);
                champions[i].pontos[0] = obter_pontuacao_torneio(i);
                champions[i].prox = -1; 

              
                HASH_inserir_generica(nome_arq_hash, &champions[i], offsetof(ChampionsByYearTeste, prox), sizeof(ChampionsByYearTeste), hash_func(&ano), ord_func, deal_with_same_input);


            }
            
        }

        i++;

        // if(i > 1) break;
    }
    

    fclose(arq_dados);


}




int main(){

    ChampionsByYearTeste sentinela2 = { 
        .chave = "-", 
        .prox = INT_MIN 
    };
    
     for(int i = 0; i < 15; i++) {
        strcpy(sentinela2.torneio[i], "-");
        sentinela2.pontos[i] = 0;
    }


    // HASH_inicializa_generica("../arquivos/champions.txt", "hash_campeoes_por_ano.hash", 35, sizeof(ChampionsByYear), &sentinela2, preenche_hash, hash_ano, order);

    // HASH_print("hash_campeoes_por_ano.hash", 35, sizeof(ChampionsByYear), offsetof(ChampionsByYear, prox));

    HASH_inicializa_generica("../arquivos/champions.txt", "hash_campeoes_por_ano_teste.hash", 35, sizeof(ChampionsByYearTeste), &sentinela2, preenche_hash, hash_ano, order);

    HASH_print("hash_campeoes_por_ano_teste.hash", 35, sizeof(ChampionsByYearTeste), offsetof(ChampionsByYearTeste, prox));
    

    return 0;
}