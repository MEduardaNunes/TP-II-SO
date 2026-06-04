#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "funcoesGerais.h"
#include "tabelaDensa.h"
#include "tabelaInvertida.h"
#include "tabelaHierarquica2.h"
#include "tabelaHierarquica3.h"

#include <stdlib.h>
#include <stdbool.h>


// Estruturas para o simulador das diferentes tabelas de páginas
typedef struct{
    TabelaDensa tabela;
    bool *quadrosLivres;
    int numeroQuadroOcupados;
    int tempo;
    EstatisticasTabela estatisticas;
} SimuladorTabelaDensa;

typedef struct{
    TabelaInvertida tabela;
    bool *quadrosLivres;
    int numeroQuadroOcupados;
    int tempo;
    EstatisticasTabela estatisticas;
} SimuladorTabelaInvertida;

typedef struct{
    TabelaHierarquica_2 tabela;
    bool *quadrosLivres;
    int numeroQuadroOcupados;
    int tempo;
    EstatisticasTabela estatisticas;
} SimuladorTabelaHierarquica2;

typedef struct{
    TabelaHierarquica_3 tabela;
    bool *quadrosLivres;
    int numeroQuadroOcupados;
    int tempo;
    EstatisticasTabela estatisticas;
} SimuladorTabelaHierarquica3;


// Estrutura da especificação do simulador geral
typedef struct EspecificacaoSimulador {
    // Parâmetros de configuração do simulador
    int numeroPaginas;
    int numeroQuadros;
    int tamanhoPagina;
    int tamanhoMemoria;
    char* politicaSubstituicao;
    char *arquivoLog;
    char* tabelaDePaginas;
    int tempo;
    bool modoDebugAtivo;

    // Estruturas de dados para as tabelas de páginas
    SimuladorTabelaDensa simuladorTabelaDensa;
    SimuladorTabelaInvertida simuladorTabelaInvertida;
    SimuladorTabelaHierarquica2 simuladorTabelaHierarquica2;
    SimuladorTabelaHierarquica3 simuladorTabelaHierarquica3;
} EspecificacaoSimulador;


#endif