# Sistema de Análise de Dados do Tênis ATP

## Descrição do Projeto

Sistema completo de análise de dados históricos do tênis ATP (Association of Tennis Professionals) desenvolvido em C como trabalho prático da disciplina de **Estruturas de Dados**. O sistema implementa múltiplas estruturas de dados para armazenamento, busca e análise eficiente de informações sobre jogadores, torneios e rankings do circuito profissional de tênis.

---

## Estruturas de Dados Implementadas

### **Tabelas Hash**
- **Hash por Nacionalidade**: Organiza jogadores por país de origem
- **Hash por Torneio**: Indexa campeões por tipo de torneio
- **Hash por Ano**: Estrutura temporal dos campeões por ano
- **Tratamento de Colisões**: Implementação com listas encadeadas

### **Listas Simplesmente Encadeadas (TLSE)**
- Estrutura genérica para armazenamento dinâmico
- Operações de inserção, busca e ordenação
- Tratamento de duplicatas com consolidação automática

### **Tabela de Acesso por Métodos (TABM)**
- Índice binário para acesso rápido aos dados dos atletas
- Busca eficiente por chave primária
- Estrutura otimizada para grandes volumes de dados

---

## Funcionalidades Principais

### **Análises Estatísticas**
- **Ranking Geral**: Classificação de jogadores por pontuação total acumulada
- **ATP Finals por Ano**: Top 8 jogadores qualificados para o torneio de fim de ano
- **Grand Slam Analysis**: Detecção de "True Golden Slam" (4 Grand Slams no mesmo ano)
- **Análise de Furos no Ranking**: Jogadores fora do top 25 que conquistaram títulos importantes

### **Análises Geográficas**
- **Compatriotas Campeões**: Jogadores que nasceram no mesmo ano que compatriotas ganharam títulos
- **Análise por Nacionalidade**: Distribuição de títulos por país
- **Maiores Campeões por Torneio**: Rankings específicos por competição

### **Sistema de Pontuação ATP**
Implementação do sistema oficial de pontuação:
- **Grand Slams**: 2000 pontos
- **ATP Finals**: Pontuação variável
- **Masters 1000**: 1000 pontos
- **Jogos Olímpicos**: Tratamento especial

---

## Estrutura do Projeto

```
📦 TrabalhoEDA/
├── 📄 main.c                    # Programa principal e inicialização
├── 📄 MENU.c/h                  # Interface de usuário e navegação
├── 📄 ATLETA.c/h               # Estrutura e operações de atletas
├── 📄 TABM.c/h                 # Tabela de Acesso por Métodos
├── 📂 Hash/                     # Módulo de Tabelas Hash
│   ├── 📄 HASH.c/h
│   ├── 📄 cria_hash_*.c
│   └── 📄 teste_hash.c
├── 📂 TLSE/                     # Listas Simplesmente Encadeadas
│   └── 📄 TLSE.c/h
├── 📂 arquivos/                 # Dados de entrada
│   ├── 📄 champions.txt         # Histórico de campeões (1990-2024)
│   ├── 📄 tennis_players.txt    # Base de dados dos jogadores
│   └── 📄 nationalities.txt     # Mapeamento de nacionalidades
├── 📂 BMFiles/                  # Arquivos binários de índice
├── 📂 Tabelas_Hash/            # Arquivos hash gerados
└── 📄 README.md                # Este arquivo
```

---

## Como Compilar e Executar

### **Pré-requisitos**
- Compilador GCC
- Sistema operacional Linux/Unix


### **Compilação**
```bash
# Compilação em uma linha
gcc -o programa main.c ATLETA.c TABM.c TLSE/TLSE.c Hash/HASH.c MENU.c

# Executar
./programa
```

---

## Dados de Entrada

### **champions.txt**
Arquivo com histórico de campeões de 1990 a 2024:
```
Ano\Australian Open (2000)\French Open (2000)\Wimbledon (2000)\US Open (2000)...
1990\Lendl (1/1)\Gómez (1/1)\Edberg (2/7)\Sampras (1/30)...
```

### **tennis_players.txt**
Base de dados dos jogadores:
```
Name\Birth\Death\Nationality\Rank\Year of the Best Rank
Ivan Lendl\1960\-\United States\1\1985 (270)
Andrés Gómez\1960\-\Ecuador\4\1990...
```



## Integrantes

- Rafael Rocha Damasceno Ferreira
- Eduardo Rottschaefer Oliveira
- Vinicius Barreto
---


## Licença

Este projeto foi desenvolvido para fins acadêmicos como parte do curso de Estruturas de Dados.

---
