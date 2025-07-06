#include "MENU.h"
#include "TABM.h"
#include "ATLETA.h"
#include "./TLSE/TLSE.h"
#include "./Hash/HASH.h"
#include <string.h>

char* getNacionalidade(int n){
    FILE* fp = fopen("nationalities.txt", "r");
    int i = 0;

    if (!fp) return NULL;
    static char nacionalidade[20];
    while (fgets(nacionalidade, sizeof(nacionalidade), fp)) {
        if (i == n) {
            // Remove newline se presente
            char *newline = strchr(nacionalidade, '\n');
            if (newline) *newline = '\0';
            fclose(fp);
            return nacionalidade;
        }
        i++;
    }
    fclose(fp);
    return NULL;
}

void listaNacionalidades(){
    FILE* fp = fopen("nationalities.txt", "r");

    int i = 0;
    static char nacionalidade[20];
    fgets(nacionalidade, sizeof(nacionalidade), fp);
    while(!feof(fp)){
        char *newline = strchr(nacionalidade, '\n');
        if (newline) *newline = '\0';

        printf("(%d) %s ", i, nacionalidade);
        i++;
        if(i%4 == 0) printf("\n");
        
        fgets(nacionalidade, sizeof(nacionalidade), fp);
    }
    printf("\n");
    fclose(fp);
}

void MENU_selecionaAcao(){
    printf("(0): Inserir atleta\n");
    printf("(1): Buscar atleta\n");
    printf("(2): Remover atleta\n");
    printf("(3): Imprimir árvore\n");
    printf("(4): Resolver questão\n");
    printf("(5): Sair\n");

    int acao;
    printf("Selecione uma das acoes acima:");
    scanf("%d", &acao);
    
    int ch = getchar();
    switch (acao) {
        case 0:
            MENU_inserir_atleta();
            break;
        case 1:
            MENU_buscar_atleta();
            break;
        case 2:
            MENU_remover_atleta();
            break;
        case 3:
            imprimeTABM(INDEX_FILE);
            break;
        case 4:
            printf("Qual tarefa deseja executar?\n");
            printf("(0): Remover todos os atletas de uma nacionalidade\n");
            printf("(1): Buscar atleta\n");
            printf("(2): Remover atleta\n");
            printf("(3): Imprimir árvore\n");
            printf("(4): Resolver questão\n");
            printf("(5): Sair\n");

            scanf("%d", &acao);
    
            int ch = getchar();

            switch (acao)
            {
            case 0:
                retira_pais();
                break;
            
            default:
                break;
            }
            break;
        case 5:
            printf("Saindo...\n");
            exit(0);
            break;
        default:
            printf("Ação inválida!\n");
            break;
    }
}

void MENU_inserir_atleta(){
    printf("Insira o nome do atleta:");
    char ch[35];

    int i = 0;
    ch[i] = getchar();
    if(ch[0] == '\n') return;
    while(ch[i] != '\n' && i < 35){
        i++;
        ch[i] = getchar();
    }
    ch[i] = '\0';
    int birth, death = -1, bestRank = -1, bestRankYear = -1, weeksTop1 = 0;


    printf("Insira o ano de nascimento:");
    scanf("%d", &birth);
    //inserir na tabela idade ..


    printf("Insira o ano de morte (-1 se está vivo):");
    scanf("%d", &death);
    if(death==-1){
        //inserir na tabela hash de vivos
    }
    

    listaNacionalidades();
    printf("Selecione uma das nacionalidades acima:");
    int id;
    scanf("%d", &id);
    char* nacionalidade = getNacionalidade(id);
    //insere na tabela nacionalidade..

    printf("Insira melhor rank dentro do top 25 do atleta (-1 se nunca chegou no top 25):");
    scanf("%d", &bestRank);
    if(bestRank != -1){
        printf("Insira o ano que o atleta atingiu esse rank:" );
        scanf("%d", &bestRankYear);
        if(bestRank == 1){
            printf("Insira quantas semanas o atleta ficou no top 1:");
            scanf("%d", &weeksTop1);
        }
    }

    
    TAtleta* atleta = novoAtleta(ch, birth, death, nacionalidade, bestRank, bestRankYear, weeksTop1);
    
    //Atualizar hashes se atleta já existir na B+
    //..

    TABM_insere(INDEX_FILE, atleta);

    liberaAtleta(atleta);
    atleta = NULL;

    imprimeTABM("BMFiles/index.bin");
}
void MENU_buscar_atleta(){
    printf("Insira o nome do atleta:");
    char ch[35];

    int i = 0;
    ch[i] = getchar();
    if(ch[0] == '\n') return;
    while(ch[i] != '\n' && i < 35){
        i++;
        ch[i] = getchar();
    }
    ch[i] = '\0';

    TAtleta* atleta = TABM_busca(INDEX_FILE, ch);
    if(atleta != NULL){
        printf("Nome: %s\n", atleta->nome);
        printf("Ano de nascimento: %d\n", atleta->anoNascimento);
        if(atleta->anoMorte != -1) printf("Ano de morte: %d\n", atleta->anoMorte);
        printf("Nacionalidade: %s\n", atleta->nacionalidade);
        if(atleta->rank != -1){
            printf("Melhor rank: %d\n", atleta->rank);
            printf("Ano de melhor rank: %d\n", atleta->anoMelhorRank);
            if(atleta->rank == 1){
                printf("Semanas no top 1: %d\n", atleta->semanasTop1);
            }
        }
        liberaAtleta(atleta);
        atleta = NULL;
    } else printf("Atleta nao encontrado\n");
}
void MENU_remover_atleta(){
    printf("Insira o nome do atleta:");
    char ch[35];

    int i = 0;
    ch[i] = getchar();
    if(ch[0] == '\n') return;
    while(ch[i] != '\n' && i < 35){
        i++;
        ch[i] = getchar();
    }
    ch[i] = '\0';

    TAtleta* atleta = TABM_busca(INDEX_FILE, ch);
    if(atleta != NULL){
        TABM_retira(INDEX_FILE, atleta->chave);
        printf("Atleta removido\n");
        liberaAtleta(atleta);
        atleta = NULL;
    } else printf("Atleta nao encontrado\n");
}


void retira_pais(){

    char pais[40];

    // HASH_print("Hash/hash_por_nacionalidade.hash", 50, 40, 36);

    printf("Deseja remover os atletas de qual nacionalidade da base de dados?\n");
    scanf("%s", pais);

    TAtleta atleta_temp;
    strcpy(atleta_temp.nacionalidade, pais);
    
    TLSE* lse = HASH_busca_generica("Hash/hash_por_nacionalidade.hash", &atleta_temp, 40, 36, hash_nacionalidade);
    TLSE* old = lse;


    while(lse){


        TAtleta* atleta = TABM_busca("BMFiles/index.bin", lse->info);

        if(!atleta) printf("Aviso: Atleta %s não encontrado na base de dados\n", lse->info);
        else{
            TABM_retira("BMFiles/index.bin", lse->info);
            HASH_remove_global((void*)atleta);
            strcpy(lse->info, atleta->nome);

        }

        lse = lse->prox;

        
    }

    printf("Os seguintes atletas forma deletados da base de dados:\n");
    TLSE_print(old);


    TLSE_free(old);

}


void table_scan(char* nome_arq_dados, int compare_func(TAtleta* atleta)){

    TLSE* lse = TLSE_inicializa();

    FILE* arq_dados = fopen(nome_arq_dados, "rb");

    char anoMorte_buffer[10];
    char rank_buffer[10];
    char anoRank_buffer[10];

    char linha[300];

    int i = 0;

    TAtleta atleta;

    fscanf(arq_dados, "%*[^\n]\n"); //Pula primeira linha de descrição do arquivo


    while(fgets(linha, sizeof(linha)/sizeof(linha[0]),arq_dados ) != NULL){

        int qtd_res = sscanf(linha, "%[^\\]\\%d\\%[^\\]\\%[^\\]\\%[^\\]\\%s (%d)",
               atleta.nome,
               &atleta.anoNascimento,
               anoMorte_buffer,
               atleta.nacionalidade,
               rank_buffer,
               anoRank_buffer,
               &atleta.semanasTop1);

        

       
        if(qtd_res == 6) atleta.semanasTop1 = -1;


        if(strcmp(anoMorte_buffer, "-") == 0) atleta.anoMorte = -1;
        else atleta.anoMorte = atoi(anoMorte_buffer);

        if(strcmp(rank_buffer, "-") == 0) atleta.rank = -1;
        else atleta.rank = atoi(rank_buffer);

        if(strcmp(anoRank_buffer, "-") == 0) atleta.anoMelhorRank = -1;
        else atleta.anoMelhorRank = atoi(anoRank_buffer);

        
        //Pega o atleta da pilha, associa a chave(sobrenome) e aloca dinamicamente
        TAtleta* new_atleta = novoAtleta(atleta.nome, atleta.anoNascimento,atleta.anoMorte, atleta.nacionalidade, atleta.rank, atleta.anoMelhorRank, atleta.semanasTop1);

        if(compare_func(new_atleta)){
            lse = TLSE_insere_inicio(lse, new_atleta);
        }
    }
    fclose(arq_dados);

    TLSE_print(lse);
    TLSE_free(lse);
}