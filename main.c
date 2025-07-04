#include "TABM.h"
#include "MENU.h"


int main(){
    TABM_inicializa("index.bin");
    // strcpy(atleta->nome, "Chardy");
    // TABM_insere("index.bin", atleta);
    // strcpy(atleta->nome, "Cuevas");
    // TABM_insere("index.bin", atleta);
    // strcpy(atleta->nome, "Olgopolov");
    // TABM_insere("index.bin", atleta);
    // strcpy(atleta->nome, "Djokovic");
    // TABM_insere("index.bin", atleta);
    // strcpy(atleta->nome, "Koubek");
    // TABM_insere("index.bin", atleta);
    // strcpy(atleta->nome, "Mirnyi");
    // TABM_insere("index.bin", atleta);
    //     strcpy(atleta->nome, "Escude");
    // TABM_insere("index.bin", atleta);
    //     strcpy(atleta->nome, "Kiefer");
    // TABM_insere("index.bin", atleta);
    //     strcpy(atleta->nome, "Clement");
    // TABM_insere("index.bin", atleta);
    // imprimeTABM("index.bin");
    // TABM_retira("index.bin", "Djokovic");
    // imprimeTABM("index.bin");
    // TABM_retira("index.bin", "Cuevas");
    // imprimeTABM("index.bin");
    TABM_leitura_arq("tennis_players.txt", "index.bin");
    imprimeTABM("index.bin");
    
    char ch[25];
    while(1){
        int i = 0;
        ch[i] = getchar();
        if(ch[0] == '\n') break;
        while(ch[i] != '\n' && i < 25){
            i++;
            ch[i] = getchar();
        }
        ch[i] = '\0';
        TAtleta* atleta = novoAtleta(ch, 0, 0, "Brazil", 0, 0);
        TABM_insere("index.bin", atleta);
        liberaAtleta(atleta);

        imprimeTABM("index.bin");
    }


    while(1){
        int i = 0;
        ch[i] = getchar();
        if(ch[0] == '\n') break;
        while(ch[i] != '\n' && i < 25){
            i++;
            ch[i] = getchar();
        }
        ch[i] = '\0';
        TAtleta* atleta = TABM_busca("index.bin", ch);
        if(atleta != NULL){
            printf("%s\n", atleta->nome);
            liberaAtleta(atleta);
        }
    }

    while(1){
        int i = 0;
        ch[i] = getchar();
        if(ch[0] == '\n') break;
        while(ch[i] != '\n' && i < 25){
            i++;
            ch[i] = getchar();
        }
        ch[i] = '\0';
        TABM_retira("index.bin", ch);
        imprimeTABM("index.bin");
    }
    

    
    
    
}