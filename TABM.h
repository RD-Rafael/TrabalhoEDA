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
    fseek(fp, sizeof(int), SEEK_SET);
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

    if(posChave == 0){
        int nchaves = nchaves_em_folha(nomeFolha);
        inicializar_folha(atleta->nome);
        FILE* faux = fopen(atleta->nome, "rb+");
        fwrite(&nchaves, sizeof(int), 1, faux);
        for(int i = 0; i < nchaves; i++){
            TAtleta* aux = buscaAtletaNFolha(nomeFolha, i);
            fwrite(aux, sizeof(TAtleta), 1, faux);
            liberaAtleta(aux);
        }
        if(strcmp(nomeFolha, atleta->nome) != 0 )remove(nomeFolha);
        strcpy(nomeFolha, atleta->nome);
        fclose(faux);
    }


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
    
    TAtleta atleta;
    int nchaves;
    int k = nchaves;
    FILE* fp = fopen(nomeFolha, "rb");
    if(!fp){
        printf("Erro ao abrir arquivo folha\n");
        exit(1);
    }
    fread(&nchaves, sizeof(int), 1, fp);
    if(nchaves == 0) return;
    int posAtleta = ftell(fp);

    for(int i = 0; i < nchaves; i++){
        posAtleta = ftell(fp);
        k--;
        fread(&atleta, sizeof(TAtleta), 1, fp);
        if(strcmp(atleta.nome, nomeAtleta) == 0){
            break;
        }
    }
    //encontrou o atleta e k é igual a quantidade de atletas que devem ser movidos para trás
    TAtleta** atletasAMover = (TAtleta**) malloc(sizeof(TAtleta*)*k);
    for(int i = 0; i < k; i++){
        fread(&atleta, sizeof(TAtleta), 1, fp);
        atletasAMover[i] = novoAtleta(
            atleta.nome,
            atleta.anoNascimento,
            atleta.anoMorte,
            atleta.nacionalidade,
            atleta.rank,
            atleta.anoMelhorRank
        );
    }
    fseek(fp, posAtleta,SEEK_SET);
    for(int i = 0; i < k; i++){
        fwrite(atletasAMover[i], sizeof(TAtleta), 1, fp);
    }


    for(int i = 0; i < k; i++) liberaAtleta(atletasAMover[i]);
    if(atletasAMover) free(atletasAMover);

    fseek(fp, 0L, SEEK_SET);
    nchaves--;
    fwrite(&nchaves, sizeof(int), 1, fp);

    fclose(fp);
}

void TABM_inicializa(char *nome_indice){
    FILE* fidx = fopen(nome_indice, "wb");
    if(!fidx){
        printf("Houve um erro ao inicializar a arvore B+");
        exit(1);
    }

    char nomeFolha[25] = "ZZZZZZZZZZZZZZZZZZZZZ";
    sprintf(nomeFolha, "f%d.bin", 0);
    inicializar_folha(nomeFolha);

    TABM raiz;
    raiz.nchaves = 0;
    for(int i = 0; i < (T*2)-1; i++) strcpy(raiz.chaves[i], "");
    strcpy(raiz.chaves[0], nomeFolha);
    raiz.folha = 1;
    for(int i = 0; i < (T*2); i++) raiz.filhos[i] = -1;
    raiz.prox = -1;

    int proxIdFolha = 1;
    fwrite(&proxIdFolha, sizeof(int), 1, fidx);
    fwrite(&raiz, sizeof(TABM), 1, fidx);

    fclose(fidx);
}

void TABM_ins_aux_alt(FILE* fp, int posAtual, int posAnt, TAtleta* atleta);

void TABM_insere(char *nome_indice, TAtleta* atleta){
    char nome_folha[25];
    strcpy(nome_folha, atleta->nome);
    FILE* fp = fopen(nome_indice, "rb+");
    if(!fp){
        printf("Houve um erro ao inserir na arvore B+");
        exit(1);
    }
    int idNovaFolha, posAnt = -1, posAtual;
    fread(&idNovaFolha, sizeof(int), 1, fp);
    posAtual = ftell(fp);
    TABM no;
    fread(&no, sizeof(TABM), 1, fp);
    if(no.nchaves == 2*T-1 && no.folha == 0){
        //dividir raíz (raiz continua como no interno)
        TABM esq, dir;
        esq.folha = 0;
        dir.folha = 0;
        esq.nchaves = T-1;
        dir.nchaves = T-1;
        for(int i = 0; i < T-1; i++){
            strcpy(esq.chaves[i], no.chaves[i]);
        }
        for(int i = 0; i < T-1; i++){
            strcpy(dir.chaves[i], no.chaves[i+T]);
        }
        strcpy(no.chaves[0], no.chaves[T-1]);
        no.nchaves = 1;

        for(int i = 0; i < T; i++){
            esq.filhos[i] = no.filhos[i];
            dir.filhos[i] = no.filhos[i+T];
        }

        int pos_ppl = TABM_ppl(fp);
        fseek(fp, pos_ppl, SEEK_SET);
        no.filhos[0] = ftell(fp);
        fwrite(&esq, sizeof(TABM), 1, fp);

        pos_ppl = TABM_ppl(fp);
        fseek(fp, pos_ppl, SEEK_SET);
        no.filhos[1] = ftell(fp);
        fwrite(&dir, sizeof(TABM), 1, fp);


        fseek(fp, posAtual, SEEK_SET);
        fwrite(&no, sizeof(TABM), 1, fp);
    } else if(no.nchaves == 2*T-1 && no.folha == 1){
        //dividir raíz e inserir (raiz vira um no interno)
        
        
        //DIVIDIR FOLHA
        TABM esq, dir;
        esq.folha = 1;
        dir.folha = 1;
        no.folha = 0;
        esq.nchaves = T-1;
        dir.nchaves = T;
        
        
        

        TAtleta* aux = buscaAtletaNFolha(no.chaves[0], T-1);
        char nomeFolha[25] = "";
        strcat(nomeFolha, aux->nome);
        liberaAtleta(aux);
        idNovaFolha++;

        inicializar_folha(nomeFolha);
        strcpy(dir.chaves[0], nomeFolha);
        for(int i = T-1; i < 2*T-1; i++){
            TAtleta* aux = buscaAtletaNFolha(no.chaves[0], i);
            insere_atleta_folha(nomeFolha, aux);
            liberaAtleta(aux);
        }
        
        strcpy(esq.chaves[0], no.chaves[0]);
        for(int i = T-1; i < 2*T-1; i++){
            TAtleta* aux = buscaAtletaNFolha(no.chaves[0], i);
            removeAtletaFolha(no.chaves[0], aux->nome);
            liberaAtleta(aux);
        }

        


        no.nchaves = 1;
        strcpy(no.chaves[0], dir.chaves[0]);

        int pos_ppl = TABM_ppl(fp);
        fseek(fp, pos_ppl, SEEK_SET);
        no.filhos[0] = ftell(fp);
        fwrite(&esq, sizeof(TABM), 1, fp);

        pos_ppl = TABM_ppl(fp);
        fseek(fp, pos_ppl, SEEK_SET);
        no.filhos[1] = ftell(fp);
        fwrite(&dir, sizeof(TABM), 1, fp);


        fseek(fp, posAtual, SEEK_SET);
        fwrite(&no, sizeof(TABM), 1, fp);

        //atualizar idNovaFolha
        fseek(fp, 0L, SEEK_SET);
        fwrite(&idNovaFolha, sizeof(int), 1, fp);
    } else if(no.folha == 1){
        //inserir na raiz
        insere_atleta_folha(no.chaves[0], atleta);
        no.nchaves++;
        if(strcmp(no.chaves[0], atleta->nome) > 0) strcpy(no.chaves[0], atleta->nome);
        fseek(fp, posAtual, SEEK_SET);
        fwrite(&no, sizeof(TABM), 1, fp);
        
        //atualizar idNovaFolha
        fseek(fp, 0L, SEEK_SET);
        fwrite(&idNovaFolha, sizeof(int), 1, fp);
        fclose(fp);
        return;
    }
    //raiz é no interno
    for(int i = 0; i < no.nchaves; i++){
        if(strcmp(no.chaves[i], nome_folha)>0){
            //primeiro maior, vá para a esquerda
            TABM_ins_aux(fp, no.filhos[i], posAtual, atleta);
            fclose(fp);
            return;
        } else if(strcmp(no.chaves[i], nome_folha) == 0){
            //encontrou o valor, vá para a direita
            TABM_ins_aux(fp, no.filhos[i+1], posAtual, atleta);
            fclose(fp);
            return;
        }
    }
    //chave buscada é maior que todas as chaves do nó atual, ir para o filho a direita da última chave
    TABM_ins_aux(fp, no.filhos[no.nchaves], posAtual, atleta);
    fclose(fp);
    return;
}

void TABM_ins_aux_alt(FILE* fp, int posAtual, int posAnt, TAtleta* atleta){
    TABM no;
    fseek(fp, posAtual, SEEK_SET);
    fread(&no, sizeof(TABM), 1, fp);
    if(no.folha == 0){
        if(no.nchaves == 2*T-1){
            //dividir no interno
            printf("if 1\n");
            TABM pai;
            fseek(fp, posAnt, SEEK_SET);
            fread(&pai, sizeof(TABM), 1, fp);


            TABM esq, dir;
            esq.folha = 0;
            dir.folha = 0;
            esq.nchaves = T-1;
            dir.nchaves = T-1;
            for(int i = 0; i < T-1; i++){
                strcpy(esq.chaves[i], no.chaves[i]);
            }
            for(int i = 0; i < T-1; i++){
                strcpy(dir.chaves[i], no.chaves[i+T]);
            }
            strcpy(no.chaves[0], no.chaves[T-1]);
            no.nchaves = 1;

            for(int i = 0; i < T; i++){
                esq.filhos[i] = no.filhos[i];
                dir.filhos[i] = no.filhos[i+T];
            }


            int pos_ppl = TABM_ppl(fp);
            fseek(fp, pos_ppl, SEEK_SET);
            no.filhos[0] = ftell(fp);
            fwrite(&esq, sizeof(TABM), 1, fp);

            pos_ppl = TABM_ppl(fp);
            fseek(fp, pos_ppl, SEEK_SET);
            no.filhos[1] = ftell(fp);
            fwrite(&dir, sizeof(TABM), 1, fp);



            //encontrar no pai onde colocar a chave em no
            int i = pos_novo_no_int(pai.chaves, pai.nchaves, no.chaves[0]);
            for(int j = pai.nchaves; j > i; j--){
                strcpy(pai.chaves[j], pai.chaves[j-1]);
                pai.filhos[j+1] = pai.filhos[j];
                pai.filhos[j]   = pai.filhos[j-1];
            }


            pai.filhos[i] = no.filhos[0];
            pai.filhos[i+1] = no.filhos[1];
            strcpy(pai.chaves[i], no.chaves[0]);
            pai.nchaves++;

            fseek(fp, posAnt, SEEK_SET);
            fwrite(&pai, sizeof(TABM), 1, fp);


            //disponibilizar endereço do no atual antigo
            fseek(fp, posAtual, SEEK_SET);
            no.folha = -1;
            fwrite(&no, sizeof(TABM), 1, fp);




            //inserir no
            if(strcmp(pai.chaves[i], nome_no) <= 0){
                //recursão para a direita
                TABM_ins_aux(fp, pai.filhos[i+1], posAnt, atleta);
            } else if (strcmp(pai.chaves[i], nome_no) > 0){
                //recursão para a esquerda
                TABM_ins_aux(fp, pai.filhos[i], posAnt, atleta);
            }
            return;
        }
    }
}


void TABM_ins_aux(FILE* fp, int posAtual, int posAnt, TAtleta* atleta){
    char nome_no[25];
    strcpy(nome_no, atleta->nome);
    fseek(fp, posAtual, SEEK_SET);
    TABM no;
    fread(&no, sizeof(TABM), 1, fp);

    if(no.folha==1){
        //buscar se nome_no já existe
        TAtleta* aux = buscaAtletaFolha(no.chaves[0], atleta->nome);
        if(aux){
            liberaAtleta(aux);
            //atualizar no;
            return;
        }
        if(aux) liberaAtleta(aux);
        //nome_no nao existe, verificar se o nó está completo
        if(no.nchaves == 2*T-1){
            printf("if 1\n");
            TABM pai;
            fseek(fp, posAnt, SEEK_SET);
            fread(&pai, sizeof(TABM), 1, fp);

            //dividir no atual em dois
            TABM esq, dir;
            esq.folha = 1;
            dir.folha = 1;
            no.folha = 0;
            esq.nchaves = T-1;
            dir.nchaves = T;
            for(int i = 0; i < T-1; i++){
                strcpy(esq.chaves[i], no.chaves[i]);
            }
            for(int i = 0; i < T; i++){
                strcpy(dir.chaves[i], no.chaves[i+T-1]);
            }
            strcpy(no.chaves[0], no.chaves[T-1]);
            no.nchaves = 1;


            int pos_ppl = TABM_ppl(fp);
            fseek(fp, pos_ppl, SEEK_SET);
            no.filhos[0] = ftell(fp);
            fwrite(&esq, sizeof(TABM), 1, fp);

            pos_ppl = TABM_ppl(fp);
            fseek(fp, pos_ppl, SEEK_SET);
            no.filhos[1] = ftell(fp);
            fwrite(&dir, sizeof(TABM), 1, fp);



            int i = pos_novo_no_int(pai.chaves, pai.nchaves, no.chaves[0]);
            for(int j = pai.nchaves; j > i; j--){
                strcpy(pai.chaves[j], pai.chaves[j-1]);
                pai.filhos[j+1] = pai.filhos[j];
                pai.filhos[j]   = pai.filhos[j-1];
            }

            //atualizar pai
            pai.filhos[i] = no.filhos[0];
            pai.filhos[i+1] = no.filhos[1];
            strcpy(pai.chaves[i], no.chaves[0]);
            pai.nchaves++;

            fseek(fp, posAnt, SEEK_SET);
            fwrite(&pai, sizeof(TABM), 1, fp);


            //disponibilizar endereço do nó atual antigo
            no.folha = -1;
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);



            //inserir no
            if(strcmp(pai.chaves[i], nome_no) <= 0){
                //recursão para a direita
                TABM_ins_aux(fp, pai.filhos[i+1], posAnt, atleta);
            } else if (strcmp(pai.chaves[i], nome_no) > 0){
                //recursão para a esquerda
                TABM_ins_aux(fp, pai.filhos[i], posAnt, atleta);
            }
            return;
        } else{
            printf("if 2\n");
            //inserir no arquivo folha
            insere_atleta_folha(no.chaves[0], atleta);
            if(strcmp(no.chaves[0], atleta->nome) > 0) strcpy(no.chaves[0], atleta->nome);
            //atualizar no...
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);

            return;
        }
    } else{
        if(no.nchaves == 2*T -1){
            printf("if 3\n");
            //dividir o nó
            TABM pai;
            fseek(fp, posAnt, SEEK_SET);
            fread(&pai, sizeof(TABM), 1, fp);


            TABM esq, dir;
            esq.folha = 0;
            dir.folha = 0;
            esq.nchaves = T-1;
            dir.nchaves = T-1;
            for(int i = 0; i < T-1; i++){
                strcpy(esq.chaves[i], no.chaves[i]);
            }
            for(int i = 0; i < T-1; i++){
                strcpy(dir.chaves[i], no.chaves[i+T]);
            }
            strcpy(no.chaves[0], no.chaves[T-1]);
            no.nchaves = 1;

            for(int i = 0; i < T; i++){
                esq.filhos[i] = no.filhos[i];
                dir.filhos[i] = no.filhos[i+T];
            }


            int pos_ppl = TABM_ppl(fp);
            fseek(fp, pos_ppl, SEEK_SET);
            no.filhos[0] = ftell(fp);
            fwrite(&esq, sizeof(TABM), 1, fp);

            pos_ppl = TABM_ppl(fp);
            fseek(fp, pos_ppl, SEEK_SET);
            no.filhos[1] = ftell(fp);
            fwrite(&dir, sizeof(TABM), 1, fp);



            //encontrar no pai onde colocar a chave em no
            int i = pos_novo_no_int(pai.chaves, pai.nchaves, no.chaves[0]);
            for(int j = pai.nchaves; j > i; j--){
                strcpy(pai.chaves[j], pai.chaves[j-1]);
                pai.filhos[j+1] = pai.filhos[j];
                pai.filhos[j]   = pai.filhos[j-1];
            }


            pai.filhos[i] = no.filhos[0];
            pai.filhos[i+1] = no.filhos[1];
            strcpy(pai.chaves[i], no.chaves[0]);
            pai.nchaves++;

            fseek(fp, posAnt, SEEK_SET);
            fwrite(&pai, sizeof(TABM), 1, fp);


            //disponibilizar endereço do no atual antigo
            fseek(fp, posAtual, SEEK_SET);
            no.folha = -1;
            fwrite(&no, sizeof(TABM), 1, fp);




            //inserir no
            if(strcmp(pai.chaves[i], nome_no) <= 0){
                //recursão para a direita
                TABM_ins_aux(fp, pai.filhos[i+1], posAnt, atleta);
            } else if (strcmp(pai.chaves[i], nome_no) > 0){
                //recursão para a esquerda
                TABM_ins_aux(fp, pai.filhos[i], posAnt, atleta);
            }
            return;
        } else{
            printf("if 4\n");
            //apenas buscar onde fazer a recursão
            int i;
            for(i = 0; i < no.nchaves; i++){
                if(strcmp(no.chaves[i], nome_no) > 0){
                    //encontrou primeiro maior
                    break;
                }
            }
            TABM_ins_aux(fp, no.filhos[i], posAtual, atleta);

        }
    }
}

void printTABM(FILE* fin, int nivel, int posAtual){
    fseek(fin, posAtual, SEEK_SET);
    TABM no;
    fread(&no, sizeof(TABM), 1, fin);
    if(no.folha == 1){
        printf("Eh folha, Arquivos: \n");
        for(int i = 0; i < no.nchaves; i++) printf("%s, ", no.chaves[i]);
        printf("\n");
    } else{
        printf("Eh no interno, Chaves: \n");
        for(int i = 0; i < no.nchaves; i++) printf("%s, ", no.chaves[i]);
        printf("\n");
        for(int i = 0; i <= no.nchaves; i++) printTABM(fin, nivel+1, no.filhos[i]);
    }
    

}


#endif