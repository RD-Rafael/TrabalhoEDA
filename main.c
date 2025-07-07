#include "TABM.h"
#include "MENU.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int compare_func(TAtleta* atleta){
    if((2025 - atleta->anoNascimento) >= 39) return 1;
    return 0;
}

int main(){
    mkdir("BMFiles", 0777);
    TABM_inicializa("BMFiles/index.bin");
    TABM_leitura_arq("tennis_players.txt", "BMFiles/index.bin");

    // grand_slams_mesmo_ano();

    ATP_final_por_ano();
    
    //TABM_destruir_arq("tennis_players.txt", "BMFiles/index.bin");
    //printf("%d\n", TABM_no_count("BMFiles/index.bin"));

    // while(1){
    //     MENU_selecionaAcao();
    // }
 

    return 0;

}