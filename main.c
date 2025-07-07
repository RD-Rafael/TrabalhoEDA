#include "TABM.h"
#include "Hash/HASH.h"
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
    TABM_leitura_arq("arquivos/tennis_players.txt", "BMFiles/index.bin");

    cria_hash_campeoes_por_ano_teste("arquivos/champions.txt", "Tabelas_Hash/hash_campeoes_por_ano_teste.hash");
    cria_hash_por_ano("arquivos/champions.txt", "Tabelas_Hash/hash_campeoes_por_ano.hash");
    cria_hash_por_torneio("arquivos/champions.txt", "Tabelas_Hash/hash_por_torneio.hash");
    cria_hash_nacionalidade("arquivos/tennis_players.txt", "Tabelas_Hash/hash_por_nacionalidade.hash");


    // nasceu_com_compatriota_campeao_new();
    
    //TABM_destruir_arq("tennis_players.txt", "BMFiles/index.bin");
    //printf("%d\n", TABM_no_count("BMFiles/index.bin"));

    while(1){
        MENU_selecionaAcao();
    }
 

    return 0;

}