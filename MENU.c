#include "MENU.h"

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
            // Chame a função para resolver a questão aqui
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
        if(atleta->anoMorte != -1) printf("Ano de moret: %d\n", atleta->anoMorte);
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