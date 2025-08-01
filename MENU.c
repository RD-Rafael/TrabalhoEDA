#include "MENU.h"
#include "TABM.h"
#include "ATLETA.h"
#include "./TLSE/TLSE.h"
#include "./Hash/HASH.h"
#include <string.h>
#include <stddef.h>

// const char* CAMINHO_HASHES = "Tabelas_Hash";

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
    printf("Selecione uma das acoes acima:\n");
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
            printf("(2): Ver jogadores que nasceram no mesmo ano em que um de seus compatriotas ganhou um Grand Slam\n");
            printf("(3): Ver a ranking geral de tenistas desde 1990\n");
            printf("(4): Ver a ranking ano a ano(25 melhores) de tenistas desde 1990\n");
            printf("(5): Ver lista de jogadores que venceram todos os Grand Slams no mesmo ano\n");
            printf("(6): Ver lista de quem furou o ranking, isto é, ganhou algum torneio, mas não estava no ranking (até 25)\n");
            printf("(7): Retirar todos os jogadores de determinada nacionalidade\n");
            printf("(8): Listar 8 melhores jogadores de cada ano\n");
            printf("(9): Ver todos os vencedores, em ordem decrescente, por torneio\n");
            printf("(10): Voltar ao menu principal\n");

            scanf("%d", &acao);
    
            int ch = getchar();

            switch (acao)
            {
            case 1:
                // retira_pais();
                break;
            case 2:
                nasceu_com_compatriota_campeao_new();
                break;
            case 3:
                ranking_geral();
                break;
            case 4:
                pontuacao_obtida_por_ano();
                break;
            case 5:
                grand_slams_mesmo_ano();
                break;
            case 6:
                furou_ranking();
                break;
            case 7:
                retira_pais();
                break;
            case 8:
                ATP_final_por_ano();
                break;
            case 9:
                maiores_campeoes_torneio();
                break;
            case 10:
                printf("Saindo da tarefa...\n");
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

    char* pais;
    int paisI;
    printf("Deseja remover os atletas de qual nacionalidade da base de dados?\n");
    listaNacionalidades();
    scanf("%d", &paisI);

    pais = getNacionalidade(paisI);

    TAtleta atleta_temp;
    strcpy(atleta_temp.nacionalidade, pais);
    
    TLSE* lse = HASH_busca_generica("Tabelas_Hash/hash_por_nacionalidade.hash", &atleta_temp, 40, 36, hash_nacionalidade);
    TLSE* old = lse;


    while(lse){


        TAtleta* atleta = TABM_busca("BMFiles/index.bin", lse->info);

        if(atleta){
            TABM_retira("BMFiles/index.bin", atleta->chave);
            HASH_remove_global((void*)atleta);
            strcpy(lse->info, atleta->nome);
            liberaAtleta(atleta);
        }

        lse = lse->prox;

        
    }

    printf("Os seguintes atletas foram deletados da base de dados:\n");
    TLSE_print(old);


    TLSE_free(old);

}

void nasceu_com_compatriota_campeão(){

    const char* nacionalidades_hash[] = {
    "Argentina",
    "Australia", 
    "Austria",
    "Belarus",
    "Belgium",
    "Brazil",
    "Bulgaria", 
    "Canada",
    "Chile",
    "Croatia",
    "Cyprus",
    "Czech Republic",
    "Denmark",
    "Ecuador",
    "Finland",
    "France",
    "Georgia",
    "Germany",
    "Great Britain",
    "Greece",
    "Haiti",
    "Hungary",
    "India",
    "Israel",
    "Italy",
    "Japan",
    "Kazakhstan",
    "Latvia",
    "Luxembourg",
    "Morocco",
    "Netherlands",
    "Norway",
    "Peru",
    "Poland",
    "Romania",
    "Russia",
    "Serbia",
    "Slovakia",
    "South Africa",
    "Spain",
    "Sweden",
    "Switzerland",
    "Taipei",
    "Thailand",
    "Ukraine",
    "United States",
    "Uruguay",
    "Yugoslavia",
    "Zimbabwe",
    "15",
    "25"
    };

    TLSE* lse = TLSE_inicializa();

    // FILE* arq_hash_nacionalidade = fopen("./Hash/hash_por_nacionalidade.hash", "rb");
    int register_size = 40;
    int prox_offset = 36;

    TAtleta atleta_temp;
    
    
    for (int i = 0; i < sizeof(nacionalidades_hash)/sizeof(nacionalidades_hash)[0]; i++) //Percorre todas as linhas da hash table de nacionalidade
    {   
        strcpy(atleta_temp.nacionalidade, nacionalidades_hash[i]);


        lse = HASH_busca_generica("Tabelas_Hash/hash_por_nacionalidade.hash", &atleta_temp, register_size, prox_offset, hash_nacionalidade);

        TLSE* old = lse;

        while (lse)
        {
            TAtleta* atleta = TABM_busca("BMFiles/index.bin", lse->info);

            if(atleta && atleta->rank != -1){


            }
        }
        

        printf("\n\n\nPais %s\n", nacionalidades_hash[i]);
        TLSE_print(lse);
        TLSE_free(lse);

    }

}

int compara(void* a, void* b){

    Champion* x = (Champion*)a;
    Champion* y = (Champion*)b;

    int qtd_x = 0;
    int qtd_y = 0;
    for (int i = 0; i < 34; i++)
    {
        if(x->ano[i] != 0) qtd_x++;
        if(y->ano[i] != 0) qtd_y++;

        if((x->ano[i] == 0) && (y->ano[i] == 0)) break;
    }
    
    if(qtd_y == qtd_x){
        return strcmp(x->chave, y->chave);
    }

    return qtd_y - qtd_x;
}

void maiores_campeoes_torneio(){

    TLSE* lse = TLSE_inicializa();

    TLSE* output = TLSE_inicializa();

    const char* nomes_torneios[15] = {
    "Australian Open",
    "French Open", 
    "Wimbledon",
    "US Open",
    "ATP Finals",
    "Olympic games",
    "Indian Wells",
    "Miami",
    "Monte Carlo",
    "Madrid",
    "Rome",
    "Canada",
    "Cincinnati",
    "Shanghai",
    "Paris"
};

    for (int i = 0; i < 15; i++)
    {
        printf("\n\n\n\nTorneio %s\n", nomes_torneios[i]);
        lse = HASH_busca_com_hash("./Tabelas_Hash/hash_por_torneio.hash", sizeof(Champion), offsetof(Champion, prox) , i);


        TLSE_ordena(lse, compara);

        TLSE_print_maiores_campeoes(lse);

        TLSE_free(lse);
    }
    
}

void furou_ranking(){

    char tipo_torneio[35];
    
    printf("\n\nSe algum jogador ganhou algum torneio, mas não estava no ranking (até 25), ele 'furou' o ranking.\nGostaria de ver os atletas que se encaixam nessa definição considerando quais títulos?:\n\n(0) Grand Slams\n(1) ATP Finals\n(2) Olimpíadas\n(3) ATP 1000\n");
    int opcao;
    scanf("%d", &opcao);

    int inf, sup;
    printf("****Casos de furo de ranking em ");

    switch (opcao) {
        case 0:
            printf("Grand Slams");
            inf = 0;
            sup = 3;
            break;
        case 1:
            printf("ATP Finals");
            inf = 4;
            sup = 4;
            break;
        case 2:
            printf("Olimpíadas");
            inf = 5;
            sup = 5;
            break;
        case 3:
            printf("ATP 1000");
            inf = 6;
            sup = 14;
            break;
      
        default:
            printf("Opção inválida!\n");
            return;
    }

    printf("****:\n\n");


    TLSE* lse = TLSE_inicializa();


    const char* nomes_torneios[15] = {
    "Australian Open",
    "French Open", 
    "Wimbledon",
    "US Open",
    "ATP Finals",
    "Olympic games",
    "Indian Wells",
    "Miami",
    "Monte Carlo",
    "Madrid",
    "Rome",
    "Canada",
    "Cincinnati",
    "Shanghai",
    "Paris"
};  

    int teve_caso = 0;

    for (int i = inf; i <=sup; i++)
    {   
        

        lse = HASH_busca_com_hash("./Tabelas_Hash/hash_por_torneio.hash", sizeof(Champion), offsetof(Champion, prox) , i);

        TLSE* old = lse;
        while(lse){

            Champion* campeao = (Champion*)(lse->info);

            TAtleta* atleta = TABM_busca("BMFiles/index.bin", campeao->chave);

            if(atleta){

                for (int j = 0; j < 34; j++)
                {   
                    if(campeao->ano[j] == 0) break; 
                    if(atleta->rank == -1) {
                        printf("\n\nAtleta %s furou o ranking em %d, quando foi campeão do Torneio %s\n", atleta->nome, campeao->ano[j], nomes_torneios[i]);
                        teve_caso = 1;
                    }
                }
                liberaAtleta(atleta);
            }
            
            lse = lse->prox;
        }

        TLSE_free(old);

}     

    if(!teve_caso){
        printf("\nNão existiram casos de furo de ranking para a opção escolhida.\n");
    }

}

int compara_pontuacao(void* a, void* b){

    ChampionsByYear* x = (ChampionsByYear*)a;
    ChampionsByYear* y = (ChampionsByYear*)b;

    return y->pontos - x->pontos;
}

void ranking_geral(){

    TLSE* lse = TLSE_inicializa();
    TLSE* output = TLSE_inicializa();
    TLSE* iter;

    for (int i = 1990; i <= 2024; i++)
    {   
        lse = HASH_busca_generica("./Tabelas_Hash/hash_campeoes_por_ano.hash", &i, sizeof(ChampionsByYear), offsetof(ChampionsByYear, prox), hash_ano);

        iter = lse;
        while(iter){
            output = TLSE_insere_nao_duplicado(output, (iter->info));
            
            TLSE* aux = iter->prox;
            free(iter);
            iter = aux;
        }

    }

    TLSE_ordena(output, compara_pontuacao);

    int pos = 1;
    iter = output;
    while (iter) {

        TAtleta* atleta = TABM_busca("BMFiles/index.bin", ((ChampionsByYear*)iter->info)->chave);

        if(atleta){
            printf("%dº - %s - ", pos++, atleta->nome);
            printf("%d", ((ChampionsByYear*)iter->info)->pontos);
       
            printf("\n\n");

        }
        else{

            //Devido a erro de pessoas com mais de um sobrenome. Ponto a melhorar
            printf("%dº - %s - ", pos++, ((ChampionsByYear*)iter->info)->chave);
            printf("%d", ((ChampionsByYear*)iter->info)->pontos);
       
            printf("\n\n");

        }
        
        
        TLSE* aux = iter->prox;
        iter = aux;

        liberaAtleta(atleta);
    }
    TLSE_free(output);
    
}

void pontuacao_obtida_por_ano(){

    TLSE* lse = TLSE_inicializa();
    TLSE* output = TLSE_inicializa();
    TLSE* iter;
    TLSE* iter2;



    for (int i = 1990; i <= 2024; i++)
    {   
        printf("\nAno de %d:\n", i);
        lse = HASH_busca_generica("./Tabelas_Hash/hash_campeoes_por_ano.hash", &i, sizeof(ChampionsByYear), offsetof(ChampionsByYear, prox), hash_ano);

        iter = lse;
        while(iter){
            output = TLSE_insere_nao_duplicado(output, (iter->info));
            
            TLSE* aux = iter->prox;
            free(iter);
            iter = aux;
        }

        iter2 = output;
        int pos = 1;

        TLSE_ordena(output, compara_pontuacao);

        while (iter2) {

            if(pos > 25) break;

            TAtleta* atleta = TABM_busca("BMFiles/index.bin", ((ChampionsByYear*)iter2->info)->chave);

            if(atleta){
                printf("%dº - %s - ", pos++, atleta->nome);
                printf("%d", ((ChampionsByYear*)iter2->info)->pontos);
        
                printf("\n\n");

            }
            else{

                //Devido a erro de pessoas com mais de um sobrenome. Ponto a melhorar
                printf("%dº - %s - ", pos++, ((ChampionsByYear*)iter2->info)->chave);
                printf("%d", ((ChampionsByYear*)iter2->info)->pontos);
        
                printf("\n\n");

            }
            
            
            iter2 = iter2->prox;

            liberaAtleta(atleta);
        }

    }

    TLSE_ordena(output, compara_pontuacao);


    TLSE_free(output);
    
}

void grand_slams_mesmo_ano(){

    TLSE* lse = TLSE_inicializa();
    TLSE* output = TLSE_inicializa();
    TLSE* iter;

    const char* grand_slams[4] = {
    "Australian Open",
    "French Open", 
    "Wimbledon",
    "US Open",
};

    int ganhou_tudo = 1;
    int alguem_ganhou = 0;


    for (int i = 0; i <= 34; i++)
    {   
        lse = HASH_busca_com_hash("./Tabelas_Hash/hash_campeoes_por_ano_teste.hash", sizeof(ChampionsByYearTeste), offsetof(ChampionsByYearTeste, prox), i);

        iter = lse;

        

        while(iter){

            ChampionsByYearTeste* champion = ((ChampionsByYearTeste*)(iter->info));

            ganhou_tudo = 1;
            
            for (int i = 0; i < 4; i++)
            {
                if(strcpy(champion->torneio[i], grand_slams[i]) != 0){
                    ganhou_tudo = 0;
                    break;
                }
            }

            if(ganhou_tudo){
                alguem_ganhou = 1;
                printf("%s ganhou tudo esse ano: ", champion->chave);

                for (int i = 0; (champion->pontos[i] != 0 && i<15); i++)
                {
                    printf("- %s ", ((ChampionsByYearTeste*)lse->info)->torneio[i]);
                    // printf("%d - ", ((ChampionsByYearTeste*)lse->info)->pontos[i]);
                }
                
                
                printf("\n\n");
            }
            
            iter = iter->prox;
        }

        TLSE_free(lse);

        // TLSE_print_teste(lse);
        // printf("\n\n\n");


    }

    if(!alguem_ganhou) printf("\nNinguém ganhou todos os grand slams no mesmo ano(True Golden Slam) no período entre 1990 e 2024\n");

}

int conta_grand_slams(ChampionsByYearTeste* champion) {
    const char* grand_slams[4] = {
        "Australian Open",
        "French Open", 
        "Wimbledon",
        "US Open"
    };
    
    int qtd_grand_slams = 0;
    
    for (int i = 0; i < 15 && champion->pontos[i] > 0; i++) {
        for (int j = 0; j < 4; j++) {
            if (strcmp(champion->torneio[i], grand_slams[j]) == 0) {
                qtd_grand_slams++;
                break; 
            }
        }
    }
    
    return qtd_grand_slams;
}

int get_pontuacao(ChampionsByYearTeste* a, TLSE* lse){

    while (lse)
    {   
        ChampionsByYearTeste* champion = (ChampionsByYearTeste*)(lse->info);

        if(strcmp(a->chave, champion->chave)==0) {
            return champion->pontos[0];
        };
        lse = lse->prox;
    }
    
}

int compara_pontuacao_ATP_Finals(void* a, void* b, TLSE* ant){

    ChampionsByYearTeste* x = (ChampionsByYearTeste*)a;
    ChampionsByYearTeste* y = (ChampionsByYearTeste*)b;
    

   

    if(y->pontos[0] == x->pontos[0]){

        int x_slams = conta_grand_slams(x);
        int y_slams = conta_grand_slams(y);

        if(x_slams == y_slams){
            return (get_pontuacao(y, ant) - get_pontuacao(x, ant));
        }
        return y_slams - x_slams;
    }
    

    return y->pontos[0] - x->pontos[0];
}

void pre_processamento(void* a){

    ChampionsByYearTeste* x = (ChampionsByYearTeste*)a;
    
    
    for (int i = 1; i < 15; i++)
    {   
        x->pontos[0] += x->pontos[i]; 
    }

    int n = conta_grand_slams(x);
    x->pontos[14] = n ? n : -1;

}

void ATP_final_por_ano(){

    TLSE* lse = TLSE_inicializa();
    TLSE* ant = TLSE_inicializa();
    TLSE* iter;
    TLSE* iter2;

    // ant = HASH_busca_com_hash("./Hash/hash_campeoes_por_ano_teste.hash", sizeof(ChampionsByYearTeste), offsetof(ChampionsByYearTeste, prox), 0);

    ant = TLSE_cria_lista_fake_1990_simples();

    for (int i = 1; i <= 34; i++)
    {   

        printf("\nAno de %d:\n", (1990+i));

        while(lse){
            TLSE* aux = lse->prox;
            free(lse);
            lse = aux;
        }

        lse = NULL;
        lse = HASH_busca_com_hash("./Tabelas_Hash/hash_campeoes_por_ano_teste.hash", sizeof(ChampionsByYearTeste), offsetof(ChampionsByYearTeste, prox), i);


        iter = lse;
        int pos = 1;

        // TLSE_print_teste(lse);

        while (iter)
        {   
            pre_processamento(iter->info);
            iter = iter->prox;
        }

        lse = TLSE_ordena_ATP_Finals(lse, ant, compara_pontuacao_ATP_Finals);

        TLSE_free(ant);
        ant = TLSE_copy(lse);

        iter2 = lse;

        while (iter2) {

            if(pos > 8) break;

            TAtleta* atleta = TABM_busca("BMFiles/index.bin", ((ChampionsByYearTeste*)iter2->info)->chave);

            if(atleta){
                printf("%dº - %s - ", pos++, atleta->nome);
        
                liberaAtleta(atleta);

            }
            else{

                //Devido a erro de pessoas com mais de um sobrenome. Ponto a melhorar
                printf("%dº - %s - ", pos++, ((ChampionsByYearTeste*)iter2->info)->chave);
        

            }
            
            printf("%d", ((ChampionsByYearTeste*)iter2->info)->pontos[0]);
            printf("\n\n");

            
            iter2 = iter2->prox;

        }
        
    }
    while(ant && lse){
        void* info = ant->info;
        TLSE* l = lse;
        if(l->info == info){
            free(info);
            TLSE* auxp = ant->prox;
            free(ant);
            ant = auxp;
            auxp = lse->prox;
            free(lse);
            lse = auxp;
            continue;
        }
        TLSE* ant = l;
        l = l->prox;
        while(l && l->info != info){
            ant = l;
            l = l->prox;
        }
        if(l){
            TLSE* auxp = l->prox;
            free(l);
            ant->prox = auxp;
        }
        free(info);
        TLSE* auxp = ant->prox;
        free(ant);
        ant = auxp;
    }
    
    
    
}

void nasceu_com_compatriota_campeao_new(){

    const char* nacionalidades_hash[] = {
    "Argentina",
    "Australia", 
    "Austria",
    "Belarus",
    "Belgium",
    "Brazil",
    "Bulgaria", 
    "Canada",
    "Chile",
    "Croatia",
    "Cyprus",
    "Czech Republic",
    "Denmark",
    "Ecuador",
    "Finland",
    "France",
    "Georgia",
    "Germany",
    "Great Britain",
    "Greece",
    "Haiti",
    "Hungary",
    "India",
    "Israel",
    "Italy",
    "Japan",
    "Kazakhstan",
    "Latvia",
    "Luxembourg",
    "Morocco",
    "Netherlands",
    "Norway",
    "Peru",
    "Poland",
    "Romania",
    "Russia",
    "Serbia",
    "Slovakia",
    "South Africa",
    "Spain",
    "Sweden",
    "Switzerland",
    "Taipei",
    "Thailand",
    "Ukraine",
    "United States",
    "Uruguay",
    "Yugoslavia",
    "Zimbabwe",
    "15",
    "25"
    };

    TAtleta temp;


    for (int i = 0; i < sizeof(nacionalidades_hash)/sizeof(nacionalidades_hash)[0]; i++){

        strcpy(temp.nacionalidade, nacionalidades_hash[i]);
        TLSE* compatriotas = HASH_busca_generica("Tabelas_Hash/hash_por_nacionalidade.hash", &temp, 40, 36, hash_nacionalidade);
        
        // TLSE_print(compatriotas);
        TLSE* aux = compatriotas;
        
        while(compatriotas){
            
            TAtleta*atleta  = TABM_busca("BMFiles/index.bin", compatriotas->info);

            // printf("Ponteiro %p", atleta);


            if(atleta && atleta->anoNascimento >= 1990){
                TLSE* vencedores_ano = HASH_busca_com_hash("Tabelas_Hash/hash_campeoes_por_ano_teste.hash", sizeof(ChampionsByYearTeste), offsetof(ChampionsByYearTeste, prox), (atleta->anoNascimento-1990));
                

                TLSE* iter = compatriotas;

                while(iter){

                    if(esta_na_lista(vencedores_ano, iter->info)){
                        printf(" -> %s ganhou um Grand Slam no ano de %d, quando seu compatriota %s nasceu\n\n", (char*)iter->info, atleta->anoNascimento, (char*)compatriotas->info);
                    }
                    iter = iter->prox;
                }
                
                // TLSE_print_teste(vencedores_ano);
                TLSE_free(vencedores_ano);
                

            }
            if(atleta) liberaAtleta(atleta);
            compatriotas = compatriotas->prox;
        }
            
        TLSE_free(aux);


    }

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