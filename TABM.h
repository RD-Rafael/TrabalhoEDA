#include "ATLETA.h"

#ifndef TABM_H
#define TABM_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ATLETA.h"

#define T 2
typedef struct no_arvbm{
    int nchaves;
    char chaves[(T*2)-1][25];
    int folha;
    int filhos[T*2];
    int prox;
} TABM;

void TABM_ins_aux(FILE* fp, int posAtual, int posAnt, TAtleta* atleta);

void TABM_print_no(TABM* no){
    printf("número de chaves: %d\n", no->nchaves);
    for(int i = 0; i < no->nchaves; i++){
        printf("chave %d: %s\n", i, no->chaves[i]);
    }
    if(no->folha == 1) printf("Eh folha\n");
    else printf("Nao eh folha\n");

}


//     fclose(fp);
// }

// void printaux(FILE* fp){
//     fseek(fp, sizeof(int), SEEK_SET);
//     TABM no;
//     no.folha = -3;
//     while(!feof(fp)){
//         fread(&no, sizeof(TABM), 1, fp);
//         printf("%d\n", no.folha);
//     }
// }

int TABM_ppl(FILE* fp){
    // fseek(fp, 0L, SEEK_END);
    // return ftell(fp);
    TABM* no = (TABM*) malloc(sizeof(TABM));
    no->folha = 0;
    int pos = ftell(fp);
    fread(no, sizeof(TABM), 1, fp);
    while(no->folha != -1 && !feof(fp)){
        pos = ftell(fp);
        fread(no, sizeof(TABM), 1, fp);
    }
    if(no->folha != -1){
        fseek(fp, 0L, SEEK_END);
        pos = ftell(fp);
    }
    free(no);
    return pos;
}



int pos_novo_no_int(char chaves[T][25], int nchaves, char* nome_no){
    if(nchaves == 0) return 0;
    if(nchaves == 1){
        if(strcmp(chaves[0], nome_no) >= 0) return 0;
        else return 1;
    }
    if(strcmp(chaves[0], nome_no) > 0) return 0;
    for(int i = 0; i < nchaves - 1; i++){
        if(strcmp(chaves[i], nome_no) < 0 && strcmp(chaves[i+1], nome_no) >= 0) return i + 1;
    }
    return nchaves;
}
int pos_novo_no_folha(char* nomeFolha, char* nomeAtleta){
    FILE* fp = fopen(nomeFolha, "rb");
    if(!fp){
        printf("Erro ao abrir arquivo folha\n");
        exit(1);
    }
    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);
    if(nchaves == 0) return 0;
    TAtleta atleta;
    fread(&atleta, sizeof(TAtleta), 1, fp);
    if(nchaves == 1){
        fclose(fp);
        if(strcmp(atleta.nome, nomeAtleta) >= 0) return 0;
        else return 1;
    }
    if(strcmp(atleta.nome, nomeAtleta) > 0) return 0;
    TAtleta atleta2;
    for(int i = 0; i < nchaves -1; i++){
        fread(&atleta2, sizeof(TAtleta), 1, fp);
        if(strcmp(atleta.nome, nomeAtleta) < 0 && strcmp(atleta2.nome, nomeAtleta) >= 0){
            fclose(fp);    
            return i + 1;
        }
        atleta = atleta2;
    }
    fclose(fp);
    return nchaves;
}

int nchaves_em_folha(char* nomeFolha){
    FILE* fp = fopen(nomeFolha, "rb");
    if(!fp){
        printf("Erro ao abrir arquivo folha\n");
        exit(1);
    }
    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);
    fclose(fp);
    return nchaves;
}

void inicializar_folha(char* nomeFolha){
    FILE* fp = fopen(nomeFolha, "wb");
    if(!fp){
        printf("Erro ao criar arquivo folha");
        exit(1);
    }
    int nchaves = 0;
    fwrite(&nchaves, sizeof(int), 1, fp);
    fclose(fp);
}

TAtleta* buscaAtletaFolha(char* nomeFolha, char* nomeAtleta){
    TAtleta atleta;
    int nchaves;
    FILE* fp = fopen(nomeFolha, "rb");
    if(!fp){
        printf("Erro ao abrir arquivo folha\n");
        exit(1);
    }
    fread(&nchaves, sizeof(int), 1, fp);
    if(nchaves == 0) return NULL;
    TAtleta* ans = AtletaInit();
    for(int i = 0; i < nchaves; i++){
        fread(&atleta, sizeof(TAtleta), 1, fp);
        if(strcmp(atleta.nome, nomeAtleta) == 0){
            ans = novoAtleta(
                atleta.nome,
                atleta.anoNascimento,
                atleta.anoMorte,
                atleta.nacionalidade,
                atleta.rank,
                atleta.anoMelhorRank
            );
        }
    }
    return ans;
}

TAtleta* buscaAtletaNFolha(char* nomeFolha, int n){ // retorna o atleta com indice n na folha
    int nchaves;
    FILE* fp = fopen(nomeFolha, "rb");
    if(!fp){
        printf("Erro ao abrir arquivo folha\n");
        exit(1);
    }
    fread(&nchaves, sizeof(int), 1, fp);
    if(n >= nchaves){
        fclose(fp);
        return NULL;
    }
    fseek(fp, n*sizeof(TAtleta) + sizeof(int), SEEK_SET);
    TAtleta* ans = (TAtleta*) malloc(sizeof(TAtleta));
    fread(ans, sizeof(TAtleta), 1, fp);
    fclose(fp);
    return ans;
}

int insere_atleta_folha(char* nomeFolha, TAtleta* atleta){
    int posChave = pos_novo_no_folha(nomeFolha, atleta->nome);


    FILE* fp = fopen(nomeFolha, "rb+");
    if(!fp){
        printf("Erro ao abrir arquivo folha\n");
        exit(1);
    }
    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);
    
    //allocs
    TAtleta **atletasAFrente = (TAtleta**) malloc(sizeof(TAtleta*)*(nchaves - posChave)); //apenas acessar esse vetor limitado por nchaves-pos, se não causa problemas obviamente
    
    //salvar os atletas a serem movidos para frente do arquivo
    fseek(fp, (posChave*sizeof(TAtleta)) + sizeof(int), SEEK_SET);
    for(int i = 0; i < nchaves-posChave; i++){
        atletasAFrente[i] = (TAtleta*) malloc(sizeof(TAtleta));
        fread(atletasAFrente[i], sizeof(TAtleta),1 , fp);
    }


    fseek(fp, (posChave*sizeof(TAtleta)) + sizeof(int), SEEK_SET);
    //escrever o novo atleta em sua posição
    fwrite(atleta, sizeof(TAtleta), 1, fp);
    //escrever todos os atletas a frente
    for(int i = 0; i < nchaves-posChave; i++) fwrite(atletasAFrente[i], sizeof(TAtleta), 1, fp);
    
    
    
    //liberar allocs
    for(int i = 0; i < nchaves-posChave; i++) liberaAtleta(atletasAFrente[i]);
    if(atletasAFrente) free(atletasAFrente);

    
    //atualizar nchaves
    nchaves++;
    fseek(fp, 0L, SEEK_SET);
    fwrite(&nchaves, sizeof(int), 1, fp);


    fclose(fp);
    return nchaves;
}





int folha_cheia(char* nomeFolha){
    if(nchaves_em_folha(nomeFolha) == 2*T-1) return 1;
    else return 0;
}

void removeAtletaFolha(char* nomeFolha, char* nomeAtleta){
    //função assume que atleta existe na folha
    FILE* fp = fopen(nomeFolha, "rb+");
    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);
    if(nchaves == 0) {
        printf("Tentando remover atleta em folha vazia");
        exit(1);
    }

    TAtleta* atleta = (TAtleta*) malloc(sizeof(TAtleta));
    int i;
    for(i = 0; i < nchaves; i++){
        fread(atleta, sizeof(TAtleta), 1, fp);
        if(strcmp(atleta->nome, nomeAtleta) == 0) break;
    }
    int k = nchaves - i - 1;
    TAtleta** atletas = (TAtleta**) malloc(sizeof(TAtleta*)*k);
    for(int i = 0; i < k; i++){
        fread(atleta, sizeof(TAtleta), 1, fp);
        atletas[i] = novoAtleta(
            atleta->nome,
            atleta->anoNascimento,
            atleta->anoMorte,
            atleta->nacionalidade,
            atleta->rank,
            atleta->anoMelhorRank
        );
    }
    fseek(fp, sizeof(int) + i*sizeof(TAtleta), SEEK_SET);
    for(int l = 0; l < k; l++){
        fwrite(atletas[l], sizeof(TAtleta), 1, fp);
    }


    fseek(fp, 0L, SEEK_SET);
    nchaves--;
    fwrite(&nchaves, sizeof(int), 1, fp);

    for(int l = 0; l < k; l++) liberaAtleta(atletas[l]);
    if(atletas) free(atletas);
    liberaAtleta(atleta);
    fclose(fp);
}


void divideFolha(char * nomeFolha){
    TAtleta* atletaMediano = buscaAtletaNFolha(nomeFolha, T-1);
    inicializar_folha(atletaMediano->nome);
    for(int i = 0; i < T; i++){
        TAtleta* atletaAMover = buscaAtletaNFolha(nomeFolha, T-1);
        insere_atleta_folha(atletaMediano->nome, atletaAMover);
        removeAtletaFolha(nomeFolha, atletaAMover->nome);
        liberaAtleta(atletaAMover);
    }
    liberaAtleta(atletaMediano);
}


void TABM_insere(char* nome_indice, TAtleta* atleta){
    FILE* fp = fopen(nome_indice, "rb+");
    if(!fp){
        printf("Erro ao abrir o arquivo indice\n");
        exit(1);
    }
    TABM raiz;
    if(fread(&raiz, sizeof(TABM), 1, fp) == 0){
        //primeiro atleta a ser inserido na arvore
        inicializar_folha(atleta->nome);
        strcpy(raiz.chaves[0], atleta->nome);
        raiz.nchaves = 1;
        raiz.folha = 1;
        raiz.prox = 0;
        for(int i = 0; i < 2*T; i++) raiz.filhos[i] = -1;
        fwrite(&raiz, sizeof(TABM), 1, fp);
        insere_atleta_folha(atleta->nome, atleta);
        fclose(fp);
        return;
    }

    if(raiz.folha == 1){
        //raiz é folha, ver se está completa
        if(raiz.nchaves == 2*T-1){
            //é necessário dividir a folha em dois, raiz vira nó interno
            TABM esq, dir;
            esq.folha = 1;
            esq.nchaves = T-1;
            dir.folha = 1;
            dir.nchaves = T;
            raiz.folha = 0;
            raiz.nchaves = 1;

            strcpy(esq.chaves[0], raiz.chaves[0]);
            TAtleta* atletaMediano = buscaAtletaNFolha(raiz.chaves[0], T-1);
            strcpy(dir.chaves[0], atletaMediano->nome);
            divideFolha(raiz.chaves[0]);

            strcpy(raiz.chaves[0], atletaMediano->nome);

            int ppl = TABM_ppl(fp);
            fseek(fp, ppl, SEEK_SET);
            raiz.filhos[0] = ftell(fp);
            fwrite(&esq, sizeof(TABM), 1, fp);

            ppl = TABM_ppl(fp);
            fseek(fp, ppl, SEEK_SET);
            raiz.filhos[1] = ftell(fp);
            fwrite(&dir, sizeof(TABM), 1, fp);

            fseek(fp, 0L, SEEK_SET);
            fwrite(&raiz, sizeof(TABM), 1, fp);
            
            
            liberaAtleta(atletaMediano);
        } else{
            //insere na folha;
            raiz.nchaves++;
            //verifica se nome vai ser atualizado
            int posChave = pos_novo_no_folha(raiz.chaves[0], atleta->nome);

            if(posChave == 0 && strcmp(atleta->nome, raiz.chaves[0])!= 0){
                rename(raiz.chaves[0], atleta->nome);
                strcpy(raiz.chaves[0], atleta->nome);
            }
            insere_atleta_folha(raiz.chaves[0], atleta);
            fseek(fp, 0L, SEEK_SET);
            fwrite(&raiz, sizeof(TABM), 1, fp);
            fclose(fp);
            return;
        }
    } else if(raiz.nchaves == 2*T-1){ //raiz é nó interno, verificar se é completo
        TABM esq, dir;
        esq.folha = 0;
        dir.folha = 0;
        esq.nchaves = T-1;
        dir.nchaves = T-1;
        raiz.nchaves = 1;

        //atualizar chaves
        for(int i = 0; i < T-1; i++){
            strcpy(esq.chaves[i], raiz.chaves[i]);
            strcpy(dir.chaves[i], raiz.chaves[i + T]);
        }
        strcpy(raiz.chaves[0], raiz.chaves[T-1]);

        //atualizar filhos
        for(int i = 0; i < T; i++){
            esq.filhos[i] = raiz.filhos[i];
            dir.filhos[i] = raiz.filhos[i+T];
        }
        //registrar indiices
        int ppl = TABM_ppl(fp);
        fseek(fp, ppl, SEEK_SET);
        raiz.filhos[0] = ftell(fp);
        fwrite(&esq, sizeof(TABM), 1, fp);

        ppl = TABM_ppl(fp);
        fseek(fp, ppl, SEEK_SET);
        raiz.filhos[1] = ftell(fp);
        fwrite(&dir, sizeof(TABM), 1, fp);

        fseek(fp, 0L, SEEK_SET);
        fwrite(&raiz, sizeof(TABM), 1, fp);
    }
    //raiz é nó interno e não completo, fazer recursão
    //...
    fclose(fp);
    return;
}




void TABM_inicializa(char *nome_indice){
    FILE* fidx = fopen(nome_indice, "wb");
    if(!fidx){
        printf("Houve um erro ao inicializar a arvore B+");
        exit(1);
    }
    fclose(fidx);
}



#endif