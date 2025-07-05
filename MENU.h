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
void table_scan(char* nome_arq_dados, int compare_func(TAtleta* atleta));
void retira_pais();


#endif