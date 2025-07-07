#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../ATLETA.h"
#include "../TLSE/TLSE.h"
#include <limits.h>
#include <stddef.h>



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
    memset(aux, 0, register_size);
    memset(ant, 0, register_size);

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
            memset(vazio, 0, register_size);
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

    free(aux);
    free(ant);

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


    FILE* arq_hash = fopen(nome_arq_hash, "rb+");


    void* aux = malloc(register_size);

    fseek(arq_hash, register_size*hash, SEEK_SET);
    fread(aux, register_size, 1, arq_hash);

    int* aux_prox = ((int*)((char*)aux + prox_offset));


    if(*(aux_prox) == INT_MIN){        
        fseek(arq_hash, register_size*hash, SEEK_SET);
        fwrite(data, register_size, 1, arq_hash);
        fseek(arq_hash, 0, SEEK_SET);
        free(aux);
        fclose(arq_hash);
        return;
    }

    //Colisão, percorrer lista até o fim
    while (ord_func(data, aux) > 0 && *aux_prox != -1)
    {   
     
        if(strcmp((char*)data, (char*)aux) == 0) break;

        fseek(arq_hash, *aux_prox, SEEK_SET);

        fread(aux, register_size, 1, arq_hash);

        aux_prox = ((int*)((char*)aux + prox_offset));

        if(strcmp((char*)data, (char*)aux) == 0) break;



    }

    if(strcmp((char*)data, (char*)aux) == 0){
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

        *aux_prox = ftell(arq_hash);
        fseek(arq_hash, ant, SEEK_SET);
        fwrite(aux, register_size, 1, arq_hash);
        fseek(arq_hash, 0, SEEK_END);

        fseek(arq_hash, 0, SEEK_END);
        fwrite(data, register_size, 1, arq_hash);

    }
    else{
    
        int * data_prox = ((int*)((char*)data + prox_offset));
        *data_prox = *aux_prox;

        *aux_prox = ftell(arq_hash);

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

        //free(aux);
    }


    fclose(arq_hash);

    return lse;


}

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

int deal_with_same_input_campeoes_ano(FILE* arq_hash, void* new_data, void* file_data){

    ChampionsByYearTeste* new = (ChampionsByYearTeste*)new_data;
    ChampionsByYearTeste* old = (ChampionsByYearTeste*)file_data;

    

    for (int i = 0; i < 15; i++)
    {   
        if(old->pontos[i] == 0){
            old->pontos[i] = new->pontos[0];
            strcpy(old->torneio[i], new->torneio[0]);
            break;
        }
    }
    

    long current_pos = ftell(arq_hash);
    fseek(arq_hash, current_pos - sizeof(ChampionsByYearTeste), SEEK_SET);
    fwrite(old, sizeof(ChampionsByYearTeste), 1, arq_hash);
    
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

void preenche_hash_por_ano(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)){

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


    while (fgets(linha, sizeof(linha), arq_dados))
    {   
        substituir_traco(linha);

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

              
                HASH_inserir_generica(nome_arq_hash, &champions[i], offsetof(ChampionsByYearTeste, prox), sizeof(ChampionsByYearTeste), hash_func(&ano), ord_func, deal_with_same_input_campeoes_ano);


            }
            
        }

    }
    

    fclose(arq_dados);


}



void cria_hash_campeoes_por_ano(char* caminho_arq_dados, char* caminho_dest_hash){
    ChampionsByYearTeste sentinela2 = { 
        .chave = "-", 
        .prox = INT_MIN 
    };
    
     for(int i = 0; i < 15; i++) {
        strcpy(sentinela2.torneio[i], "-");
        sentinela2.pontos[i] = 0;
    }

    HASH_inicializa_generica(caminho_arq_dados, caminho_dest_hash, 35, sizeof(ChampionsByYearTeste), &sentinela2, preenche_hash_por_ano, hash_ano, order);
}


int deal_with_same_input_por_torneio(FILE* arq_hash, void* new_data, void* file_data){

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


void preenche_hash_campeoes_por_torneio(char* nome_arq_dados, char* nome_arq_hash, int (*hash_func)(void* chave), int (*ord_func)(void* a, void* b)){

    FILE* arq_dados = fopen(nome_arq_dados, "r");

    

    int ano;
    Champion champions[15] = {0};

    char linha[1000];

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo


    while (fgets(linha, sizeof(linha), arq_dados))
    {   
        substituir_traco(linha);

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

              
                HASH_inserir_generica(nome_arq_hash, &champions[i], offsetof(Champion, prox), sizeof(Champion), i, ord_func, deal_with_same_input_por_torneio);


            }
            
        }
    }
    
    fclose(arq_dados);
}

int hashing(void* chave){
    return 1;
}


void cria_hash_por_torneio(char* caminho_arq_dados, char* caminho_dest_hash){

    Champion sentinela2 = { 
        .chave = "-", 
        .ano = {0},           //Inicializa todo o array com 0
        .prox = INT_MIN 
    };
    


    HASH_inicializa_generica(caminho_arq_dados, caminho_dest_hash, 15, sizeof(Champion), &sentinela2, preenche_hash_campeoes_por_torneio, hashing, order);
}


