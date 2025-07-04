#include "TABM.h"
#include "MENU.h"
#include <sys/stat.h>
#include <sys/types.h>



int main(){
    mkdir("BMFiles", 0777);
    TABM_inicializa("BMFiles/index.bin");
    //TABM_leitura_arq("tennis_players.txt", "BMFiles/index.bin");
    
    
    //TABM_destruir_arq("tennis_players.txt", "BMFiles/index.bin");
    //printf("%d\n", TABM_no_count("BMFiles/index.bin"));

    while(1){
        MENU_selecionaAcao();
    }

}