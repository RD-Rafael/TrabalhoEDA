#include "TABM.h"
#include <stdio.h>
#include <string.h>

void caminho_folha(char* buffer, const char* nomeFolha) {
    sprintf(buffer, "BMFiles/%s", nomeFolha);
}

void renameFolha(char* nomeFolha, char* novoNomeFolha){
    char caminho[256], caminhoNovo[256];
    caminho_folha(caminho, nomeFolha);
    caminho_folha(caminhoNovo, novoNomeFolha);
    rename(caminho, caminhoNovo);
}

void removeFolha(char* nomeFolha){
    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    remove(caminho);
}

void TABM_no_count_aux(FILE* fp, int* count, int posAtual){
    fseek(fp, posAtual, SEEK_SET);
    TABM no;
    fread(&no, sizeof(TABM), 1, fp);
    if(no.folha){
        (*count) += no.nchaves;
        return;
    }
    for(int i = 0; i <= no.nchaves; i++){
        TABM_no_count_aux(fp, count, no.filhos[i]);
    }
}

int TABM_no_count(char* nomeIdx){
    int count = 0;
    FILE* fp = fopen(nomeIdx, "rb");
    if(!fp){
        printf("Nao foi possivel abrir arquivo indice");
        fclose(fp);
        return -1;
    }
    TABM no;
    fread(&no, sizeof(TABM), 1, fp);
    if(feof(fp)) return 0;
    if(no.folha == 1) return no.nchaves;
    TABM_no_count_aux(fp, &count, 0);
    fclose(fp);
    return count;
}

void TABM_leitura_arq(char* nomeArq, char* nomeIdx){
    TABM_inicializa(nomeIdx);
    FILE* fp = fopen(nomeArq, "r");

    char ch;
    ch = fgetc(fp);
    while(!feof(fp) && ch != '\n') ch = fgetc(fp);
    if(feof(fp)){
        fclose(fp);
        return;
    }

    while(!feof(fp)){
        char nome[35], nationality[20];
        int birth, death = -1, bestRank = -1, bestRankYear = -1, weeksTop1 = -1;
        int i = 0;

        ch = fgetc(fp);
        if(feof(fp)){
            fclose(fp);
            return;
        }

        while(!feof(fp) && ch != '\\'){
            nome[i] = ch;
            ch = fgetc(fp);
            i++;
        }
        if(feof(fp)){
            printf("eof no momento errado\n");
            exit(1);
        }
        nome[i] = '\0';

        fscanf(fp, "%d\\", &birth);
        int auxPos = ftell(fp);
        ch = fgetc(fp);
        if(ch != '-'){
            fseek(fp, auxPos, SEEK_SET);
            fscanf(fp, "%d\\", &death);
        } else ch = fgetc(fp);

        i = 0;
        ch = fgetc(fp);
        while(!feof(fp) && ch != '\\'){
            nationality[i] = ch;
            ch = fgetc(fp);
            i++;
        }
        if(feof(fp)){
            printf("eof no momento errado\n");
            exit(1);
        }
        nationality[i] = '\0';


        auxPos = ftell(fp);
        ch = fgetc(fp);
        if(ch != '-'){
            fseek(fp, auxPos, SEEK_SET);
            fscanf(fp, "%d\\%d", &bestRank, &bestRankYear);
            ch = fgetc(fp);
            while(!feof(fp) && ch != '\n'){
                if(ch == '('){
                    fscanf(fp, "%d", &weeksTop1);
                    break;
                }
                ch = fgetc(fp);
            }
        }
        TAtleta* atleta = novoAtleta(nome, birth, death, nationality, bestRank, bestRankYear, weeksTop1);
        TABM_insere(nomeIdx, atleta);
        //printf("\nInserido: %s count: %d\n", atleta->nome, TABM_no_count(nomeIdx));
        
        liberaAtleta(atleta);
        atleta = NULL;

        while(ch != '\n') ch = fgetc(fp);
    }
    fclose(fp);
}

void TABM_destruir_arq(char* nomeArq, char* nomeIdx){
    FILE* fp = fopen(nomeArq, "r");

    char ch;
    ch = fgetc(fp);
    while(!feof(fp) && ch != '\n') ch = fgetc(fp);
    if(feof(fp)){
        fclose(fp);
        return;
    }

    while(!feof(fp)){
        char nome[35], nationality[20];
        int birth, death = -1, bestRank = -1, bestRankYear = -1, weeksTop1 = -1;
        int i = 0;
        ch = fgetc(fp);
        if(feof(fp)){
            fclose(fp);
            return;
        }

        while(!feof(fp) && ch != '\\'){
            nome[i] = ch;
            ch = fgetc(fp);
            i++;
        }
        if(feof(fp)){
            printf("eof no momento errado\n");
            exit(1);
        }
        nome[i] = '\0';

        fscanf(fp, "%d\\", &birth);
        int auxPos = ftell(fp);
        ch = fgetc(fp);
        if(ch != '-'){
            fseek(fp, auxPos, SEEK_SET);
            fscanf(fp, "%d\\", &death);
        } else ch = fgetc(fp);

        i = 0;
        ch = fgetc(fp);
        while(!feof(fp) && ch != '\\'){
            nationality[i] = ch;
            ch = fgetc(fp);
            i++;
        }
        if(feof(fp)){
            printf("eof no momento errado\n");
            exit(1);
        }
        nationality[i] = '\0';


        auxPos = ftell(fp);
        ch = fgetc(fp);
        if(ch != '-'){
            fseek(fp, auxPos, SEEK_SET);
            fscanf(fp, "%d\\%d", &bestRank, &bestRankYear);
            ch = fgetc(fp);
            while(!feof(fp) && ch != '\n'){
                if(ch == '('){
                    fscanf(fp, "%d", &weeksTop1);
                    break;
                }
                ch = fgetc(fp);
            }
        }
        TAtleta* atleta = novoAtleta(nome, birth, death, nationality, bestRank, bestRankYear, weeksTop1);
        TABM_retira(nomeIdx, atleta->chave);
        
        liberaAtleta(atleta);
        atleta = NULL;


        while(ch != '\n') ch = fgetc(fp);
    }
    fclose(fp);
}


void TABM_retira_chave_inicio(TABM* no){
    for(int i = 0; i < no->nchaves-1; i++){
        strcpy(no->chaves[i], no->chaves[i+1]);
    }
    for(int i = 0; i < no->nchaves; i++){
        no->filhos[i] = no->filhos[i+1];
    }
    no->nchaves--;
}


void TABM_print_no(TABM* no){
    printf("número de chaves: %d\n", no->nchaves);
    for(int i = 0; i < no->nchaves; i++){
        printf("chave %d: %s\n", i, no->chaves[i]);
    }
    if(no->folha == 1) printf("Eh folha\n");
    else printf("Nao eh folha\n");

}

int TABM_ppl(FILE* fp){
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
    no = NULL;
    return pos;
}



int pos_novo_no_int(char chaves[T][35], int nchaves, char* nome_no){
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

int pos_novo_no_folha(char* nomeFolha, char* chaveAtleta){
    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb");
    
    if(!fp){
        printf("Erro ao abrir arquivo folha1 %s\n", nomeFolha);
        exit(1);
    }
    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);
    if(nchaves == 0){
        fclose(fp);
        return 0;
    }
    TAtleta atleta;
    fread(&atleta, sizeof(TAtleta), 1, fp);
    if(nchaves == 1){
        fclose(fp);
        if(strcmp(atleta.chave, chaveAtleta) >= 0) return 0;
        else return 1;
    }
    if(strcmp(atleta.chave, chaveAtleta) >= 0){
        fclose(fp);
        return 0;
    }
    TAtleta atleta2;
    for(int i = 0; i < nchaves -1; i++){
        fread(&atleta2, sizeof(TAtleta), 1, fp);
        if(strcmp(atleta.chave, chaveAtleta) <= 0 && strcmp(atleta2.chave, chaveAtleta) > 0){
            fclose(fp);    
            return i + 1;
        }
        atleta = atleta2;
    }
    fclose(fp);
    return nchaves;
}

int nchaves_em_folha(char* nomeFolha){
    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb");

    if(!fp){
        printf("Erro ao abrir arquivo folha2 %s\n", nomeFolha);
        exit(1);
    }
    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);
    fclose(fp);
    return nchaves;
}

void inicializar_folha(char* nomeFolha){
    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "wb");
    
    if(!fp){
        printf("Erro ao criar arquivo folha %s", nomeFolha);
        exit(1);
    }
    int nchaves = 0;
    fwrite(&nchaves, sizeof(int), 1, fp);
    fclose(fp);
}

TAtleta* buscaAtletaFolha(char* nomeFolha, char* chaveAtleta){
    TAtleta atleta;
    int nchaves;

    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb");
    
    if(!fp){
        printf("Erro ao abrir arquivo folha3 %s\n", nomeFolha);
        exit(1);
    }
    fread(&nchaves, sizeof(int), 1, fp);
    if(nchaves == 0){
        fclose(fp);
        return NULL;
    }
    TAtleta* ans = AtletaInit();
    for(int i = 0; i < nchaves; i++){
        fread(&atleta, sizeof(TAtleta), 1, fp);
        if(strcmp(atleta.chave, chaveAtleta) == 0){
            ans = novoAtleta(
                atleta.nome,
                atleta.anoNascimento,
                atleta.anoMorte,
                atleta.nacionalidade,
                atleta.rank,
                atleta.anoMelhorRank,
                atleta.semanasTop1
            );
        }
    }
    fclose(fp);
    return ans;
}

TAtleta* buscaAtletaNFolha(char* nomeFolha, int n){ // retorna o atleta com indice n na folha
    int nchaves;

    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb");

    if(!fp){
        printf("Erro ao abrir arquivo folha4 %s\n", nomeFolha);
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
    TAtleta* atletaExistente = buscaAtletaFolha(nomeFolha, atleta->chave);
    int posChave = pos_novo_no_folha(nomeFolha, atleta->chave);
    
    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb+");
    
    if(!fp){
        printf("Erro ao abrir arquivo folha5 %s\n", nomeFolha);
        exit(1);
    }

    int nchaves;
    fread(&nchaves, sizeof(int), 1, fp);


    if(atletaExistente != NULL){
        posChave = ftell(fp);
        TAtleta aux;
        fread(&aux, sizeof(TAtleta),1, fp);
        while(strcmp(aux.chave, atleta->chave) != 0){
            posChave = ftell(fp);
            fread(&aux, sizeof(TAtleta),1, fp);
        }

        fseek(fp, posChave, SEEK_SET);    
        fwrite(atleta, sizeof(TAtleta), 1, fp);
        
        fseek(fp, 0L, SEEK_SET);
        fwrite(&nchaves, sizeof(int), 1, fp);
        
        liberaAtleta(atletaExistente);
        atletaExistente = NULL;
        fclose(fp);
        return nchaves;
    }
    
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
    atletasAFrente = NULL;

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

void removeAtletaFolha(char* nomeFolha, char* chaveAtleta){
    //função assume que atleta existe na folha
    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb+");
    
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
        if(strcmp(atleta->chave, chaveAtleta) == 0) break;
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
            atleta->anoMelhorRank,
            atleta->semanasTop1
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
    atletas = NULL;
    liberaAtleta(atleta);
    atleta = NULL;
    fclose(fp);
}


void divideFolha(char * nomeFolha){
    TAtleta* atletaMediano = buscaAtletaNFolha(nomeFolha, T-1);
    inicializar_folha(atletaMediano->chave);
    for(int i = 0; i < T; i++){
        TAtleta* atletaAMover = buscaAtletaNFolha(nomeFolha, T-1);
        insere_atleta_folha(atletaMediano->chave, atletaAMover);
        removeAtletaFolha(nomeFolha, atletaAMover->chave);
        liberaAtleta(atletaAMover);
        atletaAMover = NULL;
    }
    liberaAtleta(atletaMediano);
    atletaMediano = NULL;
}

void mergeFolha(char* nomeFolhaEsq, char* nomeFolhaDir){
    //transfere atletas da folha direita para a folha esquerda e deleta a folha direita
    char caminhoE[256];
    caminho_folha(caminhoE, nomeFolhaEsq);
    
    char caminhoD[256];
    caminho_folha(caminhoD, nomeFolhaDir);

    FILE *fpE = fopen(caminhoE, "rb+"), *fpD = fopen(caminhoD, "rb+");
    if(!fpE || !fpD){
        if(!fpE) printf("Erro ao abrir arquivo folha6 %s", nomeFolhaEsq);
        if(!fpD) printf("Erro ao abrir arquivo folha7 %s", nomeFolhaDir);
        exit(1);
    }

    int nchavesE, nchavesD;
    fread(&nchavesE, sizeof(int), 1, fpE);
    fseek(fpE, sizeof(int) + sizeof(TAtleta)*nchavesE, SEEK_SET);

    fread(&nchavesD, sizeof(int), 1, fpD);
    if(nchavesD > 0){
        TAtleta* atletas = (TAtleta*) malloc(sizeof(TAtleta)*nchavesD);
        fread(atletas, sizeof(TAtleta), nchavesD, fpD);
        fwrite(atletas, sizeof(TAtleta), nchavesD, fpE);
        free(atletas);
        atletas = NULL;
    }
    nchavesE = nchavesE + nchavesD;
    
    fseek(fpE, 0L, SEEK_SET);
    fwrite(&nchavesE, sizeof(int), 1, fpE);

    fclose(fpE);
    fclose(fpD);

    removeFolha(nomeFolhaDir);
}

char* nomeFolhaAtleta(char* nome_indice, char* chaveAtleta){
    //igual o código da função busca, mas retorna o nome da folha ao invés do atleta
    FILE* fp = fopen(nome_indice, "rb");
    if(!fp){
        printf("problema ao abrir arquivo indice\n");
        exit(1);
    }
    TABM no;
    if(fread(&no, sizeof(TABM), 1, fp) != 1 || no.folha == -1 || no.nchaves == 0){
        fclose(fp);
        return NULL;
    }
    //raiz não vazia
    while(no.folha != 1){
        int i = 0;
        //Da pra mudar esse for por uma busca binária depois
        for(i = 0; i < no.nchaves; i++){
            if(strcmp(chaveAtleta, no.chaves[i]) < 0) break;
        }
        if(no.filhos[i] == -1){
            fclose(fp);
            return NULL;
        }
        fseek(fp, no.filhos[i], SEEK_SET);
        fread(&no, sizeof(TABM), 1, fp);
    }
    fclose(fp);
    if(no.folha == 1){
        char* ans = (char*) malloc(sizeof(char)*35);
        strcpy(ans, no.chaves[0]);
        return ans;
    }

    return NULL;
}

void TABM_insere(char* nome_indice, TAtleta* atleta){
    
    //se atleta já existe, apenas atualizar
    TAtleta* atletaVerif = TABM_busca(nome_indice, atleta->chave);
    if(atletaVerif != NULL){
        //futuramente aqui vai atualizar a hash tambem
        char* nomeFolha = nomeFolhaAtleta(nome_indice, atleta->chave);
        insere_atleta_folha(nomeFolha, atleta);
        free(nomeFolha);
        nomeFolha = NULL;
        return;
    }


    FILE* fp = fopen(nome_indice, "rb+");
    if(!fp){
        printf("Erro ao abrir o arquivo indice\n");
        exit(1);
    }
    TABM raiz;
    if(fread(&raiz, sizeof(TABM), 1, fp) == 0){
        //primeiro atleta a ser inserido na arvore
        inicializar_folha(atleta->chave);
        strcpy(raiz.chaves[0], atleta->chave);
        raiz.nchaves = 1;
        raiz.folha = 1;
        raiz.prox = 0;
        for(int i = 0; i < 2*T; i++) raiz.filhos[i] = -1;
        fwrite(&raiz, sizeof(TABM), 1, fp);
        insere_atleta_folha(atleta->chave, atleta);
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
            strcpy(dir.chaves[0], atletaMediano->chave);
            divideFolha(raiz.chaves[0]);

            strcpy(raiz.chaves[0], atletaMediano->chave);

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
            atletaMediano = NULL;
        } else{
            //insere na folha, aqui vai adicionar o atleta em todas as hashes
            raiz.nchaves++;
            //verifica se nome vai ser atualizado
            int posChave = pos_novo_no_folha(raiz.chaves[0], atleta->chave);

            if(posChave == 0 && strcmp(atleta->chave, raiz.chaves[0])!= 0){
                renameFolha(raiz.chaves[0], atleta->chave);
                strcpy(raiz.chaves[0], atleta->chave);
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
    fclose(fp);
    //raiz é nó interno e não completo, fazer recursão
    
    int i;
    for(i = 0; i <= raiz.nchaves; i++){
        if(i == raiz.nchaves || strcmp(atleta->chave, raiz.chaves[i]) < 0) {
            TABM_ins_aux(nome_indice, raiz.filhos[i], 0, atleta);
            break;
        }
    }

    return;
}

void TABM_ins_aux(char* nome_indice, int posAtual, int posAntiga, TAtleta* atleta){
    FILE* fp = fopen(nome_indice, "rb+");
    if(!fp){
        printf("Erro ao abrir o arquivo indice\n");
        exit(1);
    }
    TABM noAtual;
    fseek(fp, posAtual, SEEK_SET);
    fread(&noAtual, sizeof(TABM), 1, fp);
    if(noAtual.folha == 1){
        //verificar se atleta já existe
        TAtleta *existente = buscaAtletaFolha(noAtual.chaves[0], atleta->chave);
        if(existente != NULL){
            //no existe, apenas atualizar
            insere_atleta_folha(noAtual.chaves[0], atleta);
            fclose(fp);
            liberaAtleta(existente);
            existente = NULL;
            return;
        }
        //verficiar se é completo
        if(noAtual.nchaves == 2*T-1){
            //dividir folha em dois, no atual vai ser o nó esquerdo
            TABM dir;
            dir.folha = 1;
            dir.nchaves = T;
            noAtual.nchaves = T-1;
            

            TAtleta* atletaMediano = buscaAtletaNFolha(noAtual.chaves[0], T-1);
            strcpy(dir.chaves[0], atletaMediano->chave);
            divideFolha(noAtual.chaves[0]);

            TABM pai;
            fseek(fp, posAntiga, SEEK_SET);
            fread(&pai, sizeof(TABM), 1, fp);

            int posPai = pos_novo_no_int(pai.chaves, pai.nchaves, atleta->chave);
            

            for (int j = pai.nchaves; j > posPai; j--) {
                strcpy(pai.chaves[j], pai.chaves[j - 1]);
                pai.filhos[j + 1] = pai.filhos[j];
            }
            strcpy(pai.chaves[posPai], atletaMediano->chave);
            pai.filhos[posPai] = posAtual;

            if(strcmp(atleta->chave, pai.chaves[posPai]) >= 0){
                if(strcmp(dir.chaves[0], atleta->chave) > 0){
                    renameFolha(dir.chaves[0], atleta->chave);
                    strcpy(dir.chaves[0], atleta->chave);
                }
                insere_atleta_folha(dir.chaves[0], atleta);
                dir.nchaves++;
            } else{
                if(strcmp(noAtual.chaves[0], atleta->chave) > 0){
                    renameFolha(noAtual.chaves[0], atleta->chave);
                    strcpy(noAtual.chaves[0], atleta->chave);
                }
                insere_atleta_folha(noAtual.chaves[0], atleta);
                noAtual.nchaves++;
            }
            

            
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&noAtual, sizeof(TABM), 1, fp);

            int ppl = TABM_ppl(fp);
            fseek(fp, ppl, SEEK_SET);
            pai.filhos[posPai+1] = ftell(fp);
            fwrite(&dir, sizeof(TABM), 1, fp);

            pai.nchaves++;
            fseek(fp, posAntiga, SEEK_SET);
            fwrite(&pai, sizeof(TABM), 1, fp);


            fclose(fp);
            liberaAtleta(atletaMediano);
            atletaMediano = NULL;
            return;
        } else{
            if(strcmp(noAtual.chaves[0], atleta->chave) > 0){
                renameFolha(noAtual.chaves[0], atleta->chave);
                strcpy(noAtual.chaves[0], atleta->chave);
            }
            insere_atleta_folha(noAtual.chaves[0], atleta);
            noAtual.nchaves++;
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&noAtual, sizeof(TABM), 1, fp);
            fclose(fp);
            return;
        }
    } else{
        //é interno, verificar se é completo
        if(noAtual.nchaves == 2*T-1){
            //dividir no interno em dois
            TABM dir;
            dir.folha = 0;
            dir.nchaves = T - 1;

            // Copia as chaves e filhos para o novo nó direito
            for (int i = 0; i < T - 1; i++) {
                strcpy(dir.chaves[i], noAtual.chaves[i + T]);
            }
            for (int i = 0; i < T; i++) {
                dir.filhos[i] = noAtual.filhos[i + T];
            }

            // Atualiza o nó atual (esquerdo)
            noAtual.nchaves = T - 1;

            // Atualiza o pai
            TABM pai;
            fseek(fp, posAntiga, SEEK_SET);
            fread(&pai, sizeof(TABM), 1, fp);

            int posPai = pos_novo_no_int(pai.chaves, pai.nchaves, noAtual.chaves[T - 1]);
            for (int i = pai.nchaves; i > posPai; i--) {
                strcpy(pai.chaves[i], pai.chaves[i - 1]);
                pai.filhos[i + 1] = pai.filhos[i];
            }
            strcpy(pai.chaves[posPai], noAtual.chaves[T - 1]);

            // Salva o nó direito no arquivo
            int ppl = TABM_ppl(fp);
            fseek(fp, ppl, SEEK_SET);
            pai.filhos[posPai + 1] = ftell(fp);
            fwrite(&dir, sizeof(TABM), 1, fp);

            // Salva o nó atual (esquerdo) no arquivo
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&noAtual, sizeof(TABM), 1, fp);

            // Salva o pai atualizado
            pai.nchaves++;
            fseek(fp, posAntiga, SEEK_SET);
            fwrite(&pai, sizeof(TABM), 1, fp);

            fclose(fp);
            // Decide para qual filho descer
            if (strcmp(atleta->nome, pai.chaves[posPai]) < 0) {
                TABM_ins_aux(nome_indice, posAtual, posAntiga, atleta);
            } else {
                TABM_ins_aux(nome_indice, pai.filhos[posPai + 1], posAntiga, atleta);
            }
            return;
        } else{
            //no interno incompleto, apenas fazer recursão
            int i;
            for (i = 0; i < noAtual.nchaves; i++) {
                if (strcmp(atleta->chave, noAtual.chaves[i]) < 0) {
                    break;
                }
            }
            fclose(fp);
            TABM_ins_aux(nome_indice, noAtual.filhos[i], posAtual, atleta);
            return;
        }
    }
}



void imprimeFolha(char* nomeFolha) {
    int nchaves;

    char caminho[256];
    caminho_folha(caminho, nomeFolha);
    FILE* fp = fopen(caminho, "rb");
    
    if (!fp) {
        printf("Erro ao abrir arquivo folha8\n");
        exit(1);
    }
    fread(&nchaves, sizeof(int), 1, fp);
    for (int i = 0; i < nchaves; i++) {
        TAtleta atleta;
        fread(&atleta, sizeof(TAtleta), 1, fp);
        printf("%s ", atleta.nome);
    }
    fclose(fp);
}
void imprimeTABM_aux(FILE* fp, int posAtual, int nivel) {
    if (fp == NULL){
        printf("erro ao imprimir\n");
        return;
    }
    fseek(fp, posAtual, SEEK_SET);
    TABM no;
    if (fread(&no, sizeof(TABM), 1, fp) != 1) return;

    // Print indentation for the current level
    for (int i = 0; i < nivel; i++) printf("    ");

    if (no.folha == 1) {
        printf("Folha: ");
        imprimeFolha(no.chaves[0]);
        printf("\n");
    } else {
        printf("No interno: ");
        for (int i = 0; i < no.nchaves; i++) {
            printf("[%s] ", no.chaves[i]);
        }
        printf("\n");
        for (int i = 0; i <= no.nchaves; i++) {
            imprimeTABM_aux(fp, no.filhos[i], nivel + 1);
        }
    }
}

void imprimeTABM(char* nome_indice) {
    FILE* fp = fopen(nome_indice, "rb");
    if (!fp) {
        printf("Erro ao abrir arquivo indice\n");
        exit(1);
    }
    imprimeTABM_aux(fp, 0, 0);
    fclose(fp);
    printf("\n");
}


void TABM_inicializa(char *nome_indice){
    FILE* fidx = fopen(nome_indice, "wb");
    if(!fidx){
        printf("Houve um erro ao inicializar a arvore B+");
        exit(1);
    }
    fclose(fidx);
}

void TABM_retira_aux(FILE* fp, long posAtual, char* chaveAtleta) {
    int posRecursao = -1;

    fseek(fp, posAtual, SEEK_SET);
    TABM no;
    fread(&no, sizeof(TABM),1, fp);
    
    if(no.folha == 1){
        TAtleta* atleta = buscaAtletaFolha(no.chaves[0], chaveAtleta);
        if(atleta != NULL){ //Caso 1
            removeAtletaFolha(no.chaves[0], chaveAtleta);
            liberaAtleta(atleta);
            atleta = NULL;
            no.nchaves--;
            if(no.nchaves >0){
                atleta = buscaAtletaNFolha(no.chaves[0], 0);
                renameFolha(no.chaves[0], atleta->chave);
                strcpy(no.chaves[0], atleta->chave);
                liberaAtleta(atleta);
            }

            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);
        }
        return;
    }

    int i = 0;
    for(i = 0; i < no.nchaves; i++){
        if(strcmp(chaveAtleta, no.chaves[i]) < 0) break;
    }
    int kChave = i;
    if(i-1 < no.nchaves && i-1 >= 0 && strcmp(chaveAtleta, no.chaves[i-1]) >= 0) kChave = i-1;

    TABM filho;
    fseek(fp, no.filhos[i], SEEK_SET);
    fread(&filho, sizeof(TABM), 1, fp);

    if(filho.nchaves > T-1){
        TABM_retira_aux(fp, no.filhos[i], chaveAtleta);
        return;
    }
    //filho.nchaves <= T-1
    int temEsq = (i>0);
    int temDir = (i<no.nchaves);

    TABM no_esq, no_dir;
    no_esq.nchaves = -1;
    no_dir.nchaves = -1;

    if(temEsq){
        fseek(fp, no.filhos[i-1], SEEK_SET);
        fread(&no_esq, sizeof(TABM), 1, fp);
    }
    if(temDir){
        fseek(fp, no.filhos[i+1], SEEK_SET);
        fread(&no_dir, sizeof(TABM), 1, fp);
    }

    if(no_esq.nchaves > T-1){
        if(no_esq.folha == 1){
            //printf("CASO 3A esquerdo noh folha\n");
            //filho e no_esq são folhas
            TAtleta* atletaEmprestado = buscaAtletaNFolha(no_esq.chaves[0], no_esq.nchaves-1);
            removeAtletaFolha(no_esq.chaves[0], atletaEmprestado->chave);
            no_esq.nchaves--;
            insere_atleta_folha(filho.chaves[0], atletaEmprestado);
            filho.nchaves++;

            renameFolha(filho.chaves[0], atletaEmprestado->chave);
            strcpy(filho.chaves[0], atletaEmprestado->chave);

            strcpy(no.chaves[kChave], atletaEmprestado->chave);

            fseek(fp, no.filhos[i], SEEK_SET);
            fwrite(&filho, sizeof(TABM), 1, fp);
            fseek(fp, no.filhos[i-1], SEEK_SET);
            fwrite(&no_esq, sizeof(TABM), 1, fp);
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);

            liberaAtleta(atletaEmprestado);
            atletaEmprestado = NULL;
            posRecursao = no.filhos[i];
        } else{
            //printf("CASO 3A esquerdo noh int\n");
            // filho, no e no_esq são nó internos
            for(int j = filho.nchaves; j >0; j--){
                strcpy(filho.chaves[j], filho.chaves[j-1]);
            }
            for(int j = filho.nchaves+1; j > 0; j--){
                filho.filhos[j] = filho.filhos[j-1];
            }
            strcpy(filho.chaves[0], no.chaves[kChave]);
            strcpy(no.chaves[kChave], no_esq.chaves[no_esq.nchaves-1]);

            filho.filhos[0] = no_esq.filhos[no_esq.nchaves];

            no_esq.nchaves--;
            filho.nchaves++;

            fseek(fp, no.filhos[i], SEEK_SET);
            fwrite(&filho, sizeof(TABM),1, fp);
            fseek(fp, no.filhos[i-1], SEEK_SET);
            fwrite(&no_esq, sizeof(TABM), 1, fp);
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);

            posRecursao = no.filhos[i];
        }
    } else if(no_dir.nchaves > T-1){
        if(no_dir.folha == 1){
            //printf("CASO 3A direito noh folha\n");
            //no_dir e filho são folhas
            int kChaveDir = kChave == i? kChave : kChave + 1;
            TAtleta* atletaEmprestado = buscaAtletaNFolha(no_dir.chaves[0], 0);
            removeAtletaFolha(no_dir.chaves[0], atletaEmprestado->chave);
            no_dir.nchaves--;
            insere_atleta_folha(filho.chaves[0], atletaEmprestado);
            filho.nchaves++;
            liberaAtleta(atletaEmprestado);
            atletaEmprestado = NULL;

            TAtleta* novoPrimAtletadir = buscaAtletaNFolha(no_dir.chaves[0], 0);

            renameFolha(no_dir.chaves[0], novoPrimAtletadir->chave);
            strcpy(no_dir.chaves[0], novoPrimAtletadir->chave);

            strcpy(no.chaves[kChaveDir], novoPrimAtletadir->chave);

            liberaAtleta(novoPrimAtletadir);
            novoPrimAtletadir = NULL;
        
            fseek(fp, no.filhos[i], SEEK_SET);
            fwrite(&filho, sizeof(TABM), 1, fp);
            fseek(fp, no.filhos[i+1], SEEK_SET);
            fwrite(&no_dir, sizeof(TABM), 1, fp);
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);

            posRecursao = no.filhos[i];
        } else{
            //printf("CASO 3A direito noh int\n");
            // no_dir, no e filho são nos internos
            int kChaveDir = kChave == i? kChave : kChave + 1;
            strcpy(filho.chaves[filho.nchaves], no.chaves[kChaveDir]);
            strcpy(no.chaves[kChaveDir], no_dir.chaves[0]);
            
            filho.nchaves++;
            filho.filhos[filho.nchaves] = no_dir.filhos[0];

            TABM_retira_chave_inicio(&no_dir);
            
            fseek(fp, no.filhos[i], SEEK_SET);
            fwrite(&filho, sizeof(TABM), 1, fp);
            fseek(fp, no.filhos[i+1], SEEK_SET);
            fwrite(&no_dir, sizeof(TABM), 1, fp);
            fseek(fp, posAtual, SEEK_SET);
            fwrite(&no, sizeof(TABM), 1, fp);
            
            posRecursao = no.filhos[i];
        }
    } else{ //caso 3B
        if(filho.folha ==1){ // no filho é folha
            //se no tiver apenas uma folha, colapsar para o endereço dele
            if(no.nchaves == 1){
                TABM noE, noD;
                filho.folha = -1;
                if(temEsq){
                    no_esq.folha = -1;
                    noE = no_esq;
                    noD = filho;
                } else{
                    no_dir.folha = -1;
                    noE = filho;
                    noD = no_dir;
                }
                //liberar endereços para o filho esquerdo e direito no arquivo
                fseek(fp, no.filhos[0], SEEK_SET);
                fwrite(&noE, sizeof(TABM), 1, fp);
                fseek(fp, no.filhos[1], SEEK_SET);
                fwrite(&noD, sizeof(TABM), 1, fp);

                //juntar folhas esquerda e direita
                mergeFolha(noE.chaves[0], noD.chaves[0]);
                //copiar chaves dos filhos para o pai
                strcpy(no.chaves[0], noE.chaves[0]);
                
                no.folha = 1;
                no.nchaves = noE.nchaves + noD.nchaves;

                fseek(fp, posAtual, SEEK_SET);
                fwrite(&no, sizeof(TABM), 1, fp);

                posRecursao = posAtual;
            } else if(temEsq){
                //printf("CASO 3B folha esq\n");
                posRecursao = no.filhos[i-1];
                
                mergeFolha(no_esq.chaves[0], filho.chaves[0]);
                no_esq.nchaves = no_esq.nchaves + filho.nchaves;
                //liberar posição do filho no arquivo
                filho.folha = -1;
                
                //atualizar filhos
                fseek(fp, no.filhos[i], SEEK_SET);
                fwrite(&filho, sizeof(TABM), 1, fp);
                fseek(fp, no.filhos[i-1], SEEK_SET);
                fwrite(&no_esq, sizeof(TABM), 1, fp);

                for(int j = i; j <= no.nchaves; j++){
                    no.filhos[j] = no.filhos[j+1];
                }
                no.nchaves--;
                for(int j = kChave; j < no.nchaves; j++){
                    strcpy(no.chaves[j], no.chaves[j+1]);
                }
                
                //atualizar pai
                fseek(fp, posAtual, SEEK_SET);
                fwrite(&no, sizeof(TABM), 1, fp);

            } else if(temDir){
                //printf("CASO 3B folha dir\n");
                mergeFolha(filho.chaves[0], no_dir.chaves[0]);
                filho.nchaves = filho.nchaves + no_dir.nchaves;
                //liberar posição do filho no arquivo
                no_dir.folha = -1;
                
                //atualizar filhos
                fseek(fp, no.filhos[i], SEEK_SET);
                fwrite(&filho, sizeof(TABM), 1, fp);
                fseek(fp, no.filhos[i+1], SEEK_SET);
                fwrite(&no_dir, sizeof(TABM), 1, fp);
                
                for(int j = i+1; j < no.nchaves; j++){
                    no.filhos[j] = no.filhos[j+1];
                }
                no.nchaves--;
                for(int j = kChave; j < no.nchaves; j++){
                    strcpy(no.chaves[j], no.chaves[j+1]);
                }
                
                //atualizar pai
                fseek(fp, posAtual, SEEK_SET);
                fwrite(&no, sizeof(TABM), 1, fp);
                posRecursao = no.filhos[i];
            }
        } else{ //filho é no interno
            //se no tem apenas uma chave, colapsar para o endereço dele
            if(no.nchaves == 1){
                TABM noE, noD;
                filho.folha = -1;
                if(temEsq){
                    no_esq.folha = -1;
                    noE = no_esq;
                    noD = filho;
                } else{
                    no_dir.folha = -1;
                    noE = filho;
                    noD = no_dir;
                }
                //liberar endereços para o filho esquerdo e direito no arquivo
                fseek(fp, no.filhos[0], SEEK_SET);
                fwrite(&noE, sizeof(TABM), 1, fp);
                fseek(fp, no.filhos[1], SEEK_SET);
                fwrite(&noD, sizeof(TABM), 1, fp);

                //copiar primeira chave para o meio do no
                strcpy(no.chaves[noE.nchaves], no.chaves[0]);

                //copiar chaves dos filhos para o pai
                for(int j = 0; j < noE.nchaves; j++){
                    strcpy(no.chaves[j], noE.chaves[j]);
                }
                for(int j = 0; j < noD.nchaves; j++){
                    strcpy(no.chaves[j + noE.nchaves + 1], noD.chaves[j]);
                }

                //copiar filhos dos filhos para o pai
                for(int j = 0; j <= noE.nchaves; j++){
                    no.filhos[j] = noE.filhos[j];
                }
                for(int j = 0; j <= noD.nchaves; j++){
                    no.filhos[j+noE.nchaves+1] = noD.filhos[j];
                }

                no.nchaves = noE.nchaves + noD.nchaves + 1;
                fseek(fp, posAtual, SEEK_SET);
                fwrite(&no, sizeof(TABM), 1, fp);

                posRecursao = posAtual;
            } else if(temEsq){ //no tem mais de uma chave, depois do colapso ele ainda vai existir
                //printf("CASO 3B interno esq\n");
                strcpy(no_esq.chaves[no_esq.nchaves], no.chaves[kChave]);
                //tem q aumentar ncahves de no_esq
                no_esq.nchaves++;
                for(int j = 0; j < filho.nchaves; j++){
                    strcpy(no_esq.chaves[no_esq.nchaves + j], filho.chaves[j]);
                }
                for(int j = 0; j <= filho.nchaves; j++){
                    no_esq.filhos[no_esq.nchaves + j] = filho.filhos[j];
                }
                filho.folha = -1;
                no_esq.nchaves += filho.nchaves;

                //recursão agora é no nó_esq
                posRecursao = no.filhos[i-1];


                fseek(fp, no.filhos[i], SEEK_SET);
                fwrite(&filho, sizeof(TABM), 1, fp);
                fseek(fp, no.filhos[i-1], SEEK_SET);
                fwrite(&no_esq, sizeof(TABM), 1, fp);

                for(int j = kChave; j < no.nchaves-1; j++){
                    strcpy(no.chaves[j], no.chaves[j+1]);
                }
                for(int j = i; j < no.nchaves; j++){
                    no.filhos[j] = no.filhos[j+1];
                }
                no.nchaves--;

                fseek(fp, posAtual, SEEK_SET);
                fwrite(&no, sizeof(TABM), 1, fp);
                
            } else if(temDir){
                //printf("CASO 3B interno dir\n");
                strcpy(filho.chaves[filho.nchaves], no.chaves[kChave]);
                //tem q aumentar ncahves de filho
                filho.nchaves++;
                for(int j = 0; j < no_dir.nchaves; j++){
                    strcpy(filho.chaves[filho.nchaves + j], no_dir.chaves[j]);
                }
                for(int j = 0; j <= no_dir.nchaves; j++){
                    filho.filhos[filho.nchaves + j] = no_dir.filhos[j];
                }
                no_dir.folha = -1;
                filho.nchaves += no_dir.nchaves;

                
                //recursão ainda é no nó filho
                posRecursao = no.filhos[i];

                
                fseek(fp, no.filhos[i], SEEK_SET);
                fwrite(&filho, sizeof(TABM), 1, fp);
                fseek(fp, no.filhos[i+1], SEEK_SET);
                fwrite(&no_dir, sizeof(TABM), 1, fp);

                for(int j = kChave; j < no.nchaves - 1; j++){
                    strcpy(no.chaves[j], no.chaves[j+1]);
                }
                for(int j = i+1; j < no.nchaves; j++){
                    no.filhos[j] = no.filhos[j+1];
                }
                no.nchaves--;

                fseek(fp, posAtual, SEEK_SET);
                fwrite(&no, sizeof(TABM), 1, fp);
                
            }
        }
    }
    if(posRecursao != -1) TABM_retira_aux(fp, posRecursao, chaveAtleta);

}



void TABM_retira(char* nome_indice, char* chaveAtleta){
    FILE* fp = fopen(nome_indice, "rb+");
    if(!fp){
        printf("Erro ao abrir o arquivo indice\n");
        exit(1);
    }

    TABM no;
    fread(&no, sizeof(TABM),1, fp);
    if(no.nchaves == 0){
        fclose(fp);
        return;
    }
    if(no.folha == 1){
        TAtleta* atleta = buscaAtletaFolha(no.chaves[0], chaveAtleta);
        if(!atleta) return;
        //atleta está na raiz folha: remover
        removeAtletaFolha(no.chaves[0], chaveAtleta);
        no.nchaves--;

        if(no.nchaves > 0 && strcmp(no.chaves[0], chaveAtleta) == 0){
            liberaAtleta(atleta);
            atleta = buscaAtletaNFolha(no.chaves[0], 0);
            renameFolha(no.chaves[0], atleta->chave);
            strcpy(no.chaves[0], atleta->chave);
        }

        fseek(fp, 0L, SEEK_SET);
        fwrite(&no, sizeof(TABM), 1, fp);

        liberaAtleta(atleta);
        atleta = NULL;
        fclose(fp);
        return;
    } else{
        TABM_retira_aux(fp, 0, chaveAtleta);
    }
    fclose(fp);
    return;
}

TAtleta* TABM_busca(char* nome_indice, char* chaveAtleta){
    FILE* fp = fopen(nome_indice, "rb");
    if(!fp){
        printf("problema ao abrir arquivo indice\n");
        exit(1);
    }
    TABM no;
    if(fread(&no, sizeof(TABM), 1, fp) != 1 || no.folha == -1 || no.nchaves == 0){
        fclose(fp);
        return NULL;
    }
    //raiz não vazia
    while(no.folha != 1){
        int i = 0;
        //Da pra mudar esse for por uma busca binária depois
        for(i = 0; i < no.nchaves; i++){
            if(strcmp(chaveAtleta, no.chaves[i]) < 0) break;
        }
        if(no.filhos[i] == -1){
            fclose(fp);
            return NULL;
        }
        fseek(fp, no.filhos[i], SEEK_SET);
        fread(&no, sizeof(TABM), 1, fp);
    }
    fclose(fp);
    if(no.folha == 1){
        TAtleta* atleta = buscaAtletaFolha(no.chaves[0], chaveAtleta);
        if(atleta != NULL) return atleta;
    }

    return NULL;
}