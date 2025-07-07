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
    if (!novo) return lse;
    novo->info = info;
    novo->prox = lse;
    return novo;
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

void TLSE_print_champion(TLSE* lse) {
    while (lse) {
        printf("%s - ", ((Champion*)lse->info)->chave);
        printf("Campeão em");
        for (int i = 0; i < 34; i++)
        {   
            if(((Champion*)lse->info)->ano[i] == 0) break;
            printf(" %d,", ((Champion*)lse->info)->ano[i]);
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