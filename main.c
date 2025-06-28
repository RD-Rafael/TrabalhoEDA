#include "TABM.h"


int main(){
    TABM_inicializa("index.bin");
    TAtleta* atleta = novoAtleta("", 0, 0, "", 0, 0);
    strcpy(atleta->nome, "Chardy");
    TABM_insere("index.bin", atleta);
    strcpy(atleta->nome, "Cuevas");
    TABM_insere("index.bin", atleta);
    strcpy(atleta->nome, "Olgopolov");
    TABM_insere("index.bin", atleta);
    strcpy(atleta->nome, "Djokovic");
    TABM_insere("index.bin", atleta);
    strcpy(atleta->nome, "Koubek");
    TABM_insere("index.bin", atleta);
    strcpy(atleta->nome, "Mirnyi");
    TABM_insere("index.bin", atleta);
        strcpy(atleta->nome, "Escude");
    TABM_insere("index.bin", atleta);
        strcpy(atleta->nome, "Kiefer");
    TABM_insere("index.bin", atleta);
        strcpy(atleta->nome, "Clement");
    TABM_insere("index.bin", atleta);
    imprimeTABM("index.bin");
    TABM_retira("index.bin", "Djokovic");
    imprimeTABM("index.bin");
    TABM_retira("index.bin", "Cuevas");
    imprimeTABM("index.bin");
    // TABM_insere("index.bin", "olgopolov");
    // TABM_insere("index.bin", "Chardy");
    // TABM_insere("index.bin", "Djokovic");
    // FILE* fin = fopen("index.bin", "rb");
    // if(!fin){
    //     printf("algo deu errado\n");
    //     exit(1);
    // }
    // printTABM(fin, 0, sizeof(int));
    // fclose(fin);

    // TABM_insere("index.bin", "Cuevas");
    // TABM_insere("index.bin", "Clément");
    // TABM_insere("index.bin", "Mirnyi");
    // TABM_insere("index.bin", "Koubek");
    // TABM_insere("index.bin", "Kiefer");
    // TABM_insere("index.bin", "Escudé");
    
    if(strcmp("20", "2") > 0) printf("20 maior q 2\n");
    else printf("20 < 2\n");
    

    
    
    liberaAtleta(atleta);
}