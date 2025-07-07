#ifndef MENU_H
#define MENU_H
#define INDEX_FILE "BMFiles/index.bin"
#include "ATLETA.h"
#include "TABM.h"
#include <stdio.h>

void MENU_inserir_atleta();
void MENU_buscar_atleta();
void MENU_remover_atleta();
char* getNacionalidade(int n);
void listaNacionalidades();
void MENU_selecionaAcao();

//Interface pra teste na main
void table_scan(char* nome_arq_dados, int compare_func(TAtleta* atleta));
void retira_pais();
void nasceu_com_compatriota_campeão();
void maiores_campeoes_torneio();
void furou_ranking();
void pontuacao_obtida_por_ano();
void ranking_geral();
void grand_slams_mesmo_ano();


#endif