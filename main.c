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

    retira_pais();

    // TAtleta* x = TABM_busca("BMFiles/index.bin", "Steeb");

    // imprimeTABM("BMFiles/index.bin");

    // if(!x) printf("\nNot found\n");
    // else printf("\n%s\n", x->nome);
    
    
    //TABM_destruir_arq("tennis_players.txt", "BMFiles/index.bin");
    //printf("%d\n", TABM_no_count("BMFiles/index.bin"));

    // MENU_selecionaAcao();
 

    return 0;

}