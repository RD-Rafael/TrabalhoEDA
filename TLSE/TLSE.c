#include "TLSE.h"
#include "../Hash/HASH.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

TLSE* TLSE_inicializa(){
    return NULL;
}




TLSE* TLSE_insere_inicio(TLSE* lse, void* info) {
    TLSE* novo = (TLSE*)malloc(sizeof(TLSE));
    novo->info = info;
    novo->prox = lse;
    return novo;
}

TLSE* TLSE_insere_nao_duplicado(TLSE* output, void* new) {

    ChampionsByYear* new_data = (ChampionsByYear*)(new);
    TLSE* iter = output;
    TLSE* ant = NULL;
    while(iter){

        ChampionsByYear* champion = ((ChampionsByYear*)(iter->info));
        if(strcmp(champion->chave, new_data->chave)==0){
            champion->pontos += new_data->pontos;
            // free(new_data);
            return output;
        }

        ant = iter;
        iter = iter->prox;
    }

    TLSE* novo = (TLSE*)malloc(sizeof(TLSE));
    novo->info = new;
    novo->prox = NULL;
    if(!ant) return novo;

    ant->prox = novo;
    
    return output;
}

// TLSE* TLSE_busca(TLSE* lse, int n) {
//     while (lse) {
//         if (lse->info == n) return lse;
//         lse = lse->prox;
//     }
//     return NULL;
// }

void TLSE_print(TLSE* lse) {
    while (lse) {
        printf("%p\n", lse->info);
        lse = lse->prox;
    }
}

void TLSE_print_maiores_campeoes(TLSE* lse) {
    while (lse) {

        Champion* champion = ((Champion*)lse->info);
        printf("%s ", champion->chave);

        for (int i = 0; champion->ano[i] != 0; i++)
        {
            printf("- %d ", champion->ano[i]);

        }
     
        printf("\n\n");
        
        lse = lse->prox;
    }
}

void TLSE_print_teste(TLSE* lse) {
    while (lse) {

        ChampionsByYearTeste* champion = ((ChampionsByYearTeste*)lse->info);
        printf("%s ", champion->chave);

        for (int i = 0; (champion->pontos[i] != 0 && i<15); i++)
        {
            printf("- %s ", ((ChampionsByYearTeste*)lse->info)->torneio[i]);
            // printf("%d - ", ((ChampionsByYearTeste*)lse->info)->pontos[i]);
        }
        
        
       
        printf("\n\n");
        
        lse = lse->prox;
    }
}

int TLSE_conta(TLSE* l){
    if(!l) return 0;
    return 1 + TLSE_conta(l->prox);
}

void TLSE_free(TLSE* lse){
    if(!lse) return;

    TLSE_free(lse->prox);
    free(lse->info);
    free(lse);
}

TLSE* TLSE_ordena(TLSE* lse, int (*compara)(void* a, void* b)) {
    if (!lse || !lse->prox) return lse;
    
    int trocou = 1;
    
    while (trocou) {
        trocou = 0;
        TLSE* atual = lse;
        
        while (atual && atual->prox) {
            // Se atual > próximo, trocar
            if (compara(atual->info, atual->prox->info) > 0) {
                // Trocar os ponteiros info
                void* temp = atual->info;
                atual->info = atual->prox->info;
                atual->prox->info = temp;
                
                trocou = 1;
            }
            atual = atual->prox;
        }
    }
    
    return lse;
}

TLSE* TLSE_ordena_ATP_Finals(TLSE* lse, TLSE* lse_ant,  int (*compara)(void* a, void* b, TLSE* lse_ant)) {
    if (!lse || !lse->prox) return lse;
    
    int trocou = 1;
    
    while (trocou) {
        trocou = 0;
        TLSE* atual = lse;
        
        while (atual && atual->prox) {
            // Se atual > próximo, trocar
            if (compara(atual->info, atual->prox->info, lse_ant) > 0) {
                // Trocar os ponteiros info
                void* temp = atual->info;
                atual->info = atual->prox->info;
                atual->prox->info = temp;
                
                trocou = 1;
            }
            atual = atual->prox;
        }
    }
    
    return lse;
}

TLSE* TLSE_copy(TLSE* lse) {
    if (!lse) return NULL;
    TLSE* nova_lista = NULL;
    TLSE* *ptr = &nova_lista;
    while (lse) {
        TLSE* novo = (TLSE*)malloc(sizeof(TLSE));
        novo->info = lse->info;
        novo->prox = NULL;
        *ptr = novo;
        ptr = &((*ptr)->prox);
        lse = lse->prox;
    }
    return nova_lista;
}

TLSE* TLSE_cria_lista_fake_1990() {
    // Jogadores que disputaram ATP Finals em 1990 (em ordem de classificação)
    const char* jogadores_1990[8] = {
        "Stefan Edberg",     // 1º - Maior pontuação
        "André Agassi",      // 2º
        "Pete Sampras",      // 3º
        "Emílio Sanchez",    // 4º
        "Boris Becker",      // 5º
        "Ivan Lendl",        // 6º
        "Andrés Goméz",      // 7º
        "Thomas Muster"      // 8º - Menor pontuação
    };
    
    // Pontuações fake decrescentes (começando com 5000 pontos)
    int pontuacoes_fake[8] = {
        5000,  // Stefan Edberg
        4500,  // André Agassi
        4000,  // Pete Sampras
        3500,  // Emílio Sanchez
        3000,  // Boris Becker
        2500,  // Ivan Lendl
        2000,  // Andrés Goméz
        1500   // Thomas Muster
    };
    
    // Torneios fake para distribuir pontos
    const char* torneios_fake[4] = {
        "Australian Open",
        "French Open", 
        "Wimbledon",
        "US Open"
    };
    
    TLSE* lista_fake = TLSE_inicializa();
    
    printf("=== CRIANDO LISTA FAKE PARA 1990 ===\n");
    
    for (int i = 0; i < 8; i++) {
        // Alocar memória para o jogador
        ChampionsByYearTeste* jogador = malloc(sizeof(ChampionsByYearTeste));
        
        // Inicializar estrutura
        strcpy(jogador->chave, jogadores_1990[i]);
        jogador->prox = -1;
        
        // Inicializar arrays
        for (int j = 0; j < 15; j++) {
            strcpy(jogador->torneio[j], "-");
            jogador->pontos[j] = 0;
        }
        
        // Distribuir pontuação fake entre alguns torneios
        int pontos_restantes = pontuacoes_fake[i];
        int torneios_usados = 0;
        
        // Dar pontos de Grand Slams primeiro (para critério de desempate)
        for (int j = 0; j < 4 && pontos_restantes > 0; j++) {
            if (pontos_restantes >= 500) {
                strcpy(jogador->torneio[torneios_usados], torneios_fake[j]);
                jogador->pontos[torneios_usados] = 500 + (rand() % 300); // 500-800 pontos
                pontos_restantes -= jogador->pontos[torneios_usados];
                torneios_usados++;
            }
        }
        
        // Distribuir pontos restantes em outros torneios
        while (pontos_restantes > 0 && torneios_usados < 10) {
            strcpy(jogador->torneio[torneios_usados], "ATP Masters");
            int pontos_torneio = (pontos_restantes > 300) ? 200 + (rand() % 100) : pontos_restantes;
            jogador->pontos[torneios_usados] = pontos_torneio;
            pontos_restantes -= pontos_torneio;
            torneios_usados++;
        }
        
        // Armazenar total em pontos[0]
        jogador->pontos[0] = pontuacoes_fake[i];
        
        printf("Jogador %d: %s - %d pontos\n", i+1, jogador->chave, jogador->pontos[0]);
        
        // Inserir na lista
        lista_fake = TLSE_insere_inicio(lista_fake, jogador);
    }
    
    printf("=== LISTA FAKE 1990 CRIADA COM SUCESSO ===\n\n");
    
    return lista_fake;
}

// Função alternativa mais simples (apenas com pontuação total)
TLSE* TLSE_cria_lista_fake_1990_simples() {
    const char* jogadores_1990[8] = {
        "Stefan Edberg",
        "André Agassi", 
        "Pete Sampras",
        "Emílio Sanchez",
        "Boris Becker",
        "Ivan Lendl",
        "Andrés Goméz",
        "Thomas Muster"
    };
    
    TLSE* lista_fake = TLSE_inicializa();
    
    for (int i = 0; i < 8; i++) {
        ChampionsByYearTeste* jogador = malloc(sizeof(ChampionsByYearTeste));
        
        strcpy(jogador->chave, jogadores_1990[i]);
        jogador->prox = -1;
        
        // Inicializar arrays
        for (int j = 0; j < 15; j++) {
            strcpy(jogador->torneio[j], "-");
            jogador->pontos[j] = 0;
        }
        
        // Pontuação decrescente: 5000, 4500, 4000, etc.
        jogador->pontos[0] = 5000 - (i * 500);
        
        // Adicionar informação fake de torneio principal
        strcpy(jogador->torneio[0], "Australian Open");
        jogador->pontos[1] = 2000; // Pontos do Australian Open
        
        lista_fake = TLSE_insere_inicio(lista_fake, jogador);
    }
    
    return lista_fake;
}