#include "TABM.h"
#include "MENU.h"
#include <sys/stat.h>
#include <sys/types.h>


int main(){
    mkdir("BMFiles", 0777);
    TABM_inicializa("BMFiles/index.bin");
    TABM_leitura_arq("tennis_players.txt", "BMFiles/index.bin");
    
    imprimeTABM("BMFiles/index.bin");
    
    TABM_destruir_arq("tennis_players.txt", "BMFiles/index.bin");
    printf("%d\n", TABM_no_count("BMFiles/index.bin"));


    char ch[35];
    while(1){
        int i = 0;
        ch[i] = getchar();
        if(ch[0] == '\n') break;
        while(ch[i] != '\n' && i < 35){
            i++;
            ch[i] = getchar();
        }
        ch[i] = '\0';
        TAtleta* atleta = novoAtleta(ch, 0, 0, "Brazil", 0, 0);
        TABM_insere("BMFiles/index.bin", atleta);
        liberaAtleta(atleta);

        imprimeTABM("BMFiles/index.bin");
    }


    while(1){
        int i = 0;
        ch[i] = getchar();
        if(ch[0] == '\n') break;
        while(ch[i] != '\n' && i < 35){
            i++;
            ch[i] = getchar();
        }
        ch[i] = '\0';
        TAtleta* atleta = TABM_busca("BMFiles/index.bin", ch);
        if(atleta != NULL){
            printf("%s\n", atleta->nome);
            liberaAtleta(atleta);
        }
    }

    while(1){
        int i = 0;
        ch[i] = getchar();
        if(ch[0] == '\n') break;
        while(ch[i] != '\n' && i < 35){
            i++;
            ch[i] = getchar();
        }
        ch[i] = '\0';
        TABM_retira("BMFiles/index.bin", ch);
        imprimeTABM("BMFiles/index.bin");
        printf("%d", TABM_no_count("BMFiles/index.bin"));
    }
    

    
    
    
}