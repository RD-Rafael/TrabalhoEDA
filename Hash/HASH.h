#include "../ATLETA.h"

void HASH_inicializa(char* nome_arq_dados, char* nome_arq_hash, int hash_size, int hash_func(void* chave), int ord_func(void* a, void* b));

void HASH_remove(char* nome_arq_hash, TAtleta atleta, int hash_func(void* chave));

char* HASH_busca(char* nome_arq_hash, TAtleta atleta, int hash_func(void* chave), char* name);

void HASH_print(char* nome_arq, int hash_size);

