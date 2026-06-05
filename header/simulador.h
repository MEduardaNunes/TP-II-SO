#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "funcoesGerais.h"
#include "tabelaDensa.h"
#include "tabelaInvertida.h"
#include "tabelaHierarquica2.h"
#include "tabelaHierarquica3.h"

#include <stdlib.h>
#include <stdbool.h>

#define MAX_POLITICA_SUBSTITUICAO 16
#define MAX_ARQUIVO_LOG 512

// Estruturas para o simulador das diferentes tabelas de páginas
typedef struct{
    TabelaDensa tabela;
    bool *quadrosLivres;
    int *paginasPorQuadro;
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
    int *paginasPorQuadro;
    int numeroQuadroOcupados;
    int tempo;
    EstatisticasTabela estatisticas;
} SimuladorTabelaHierarquica2;

typedef struct{
    TabelaHierarquica_3 tabela;
    bool *quadrosLivres;
    int *paginasPorQuadro;
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
    char politicaSubstituicao[MAX_POLITICA_SUBSTITUICAO];
    char arquivoLog[MAX_ARQUIVO_LOG];
    bool modoDebugAtivo;

    // Estruturas de dados para as tabelas de páginas
    SimuladorTabelaDensa simuladorTabelaDensa;
    SimuladorTabelaInvertida simuladorTabelaInvertida;
    SimuladorTabelaHierarquica2 simuladorTabelaHierarquica2;
    SimuladorTabelaHierarquica3 simuladorTabelaHierarquica3;
} EspecificacaoSimulador;

void inicializarSimulador(EspecificacaoSimulador *especificacao, int numeroPaginas, int numeroQuadros, int tamanhoPagina, int tamanhoMemoria, char* politicaSubstituicao, char *arquivoLog, bool modoDebugAtivo);
void lerArgumentosTerminal(int argc, char *argv[], EspecificacaoSimulador *simulador);
void rodarSimulador(EspecificacaoSimulador *simulador);
void imprimirSimulador(EspecificacaoSimulador *simulador);
void destruirSimulador(EspecificacaoSimulador *simulador);

// Função debug
void salvarResultadosDebug(EspecificacaoSimulador *simulador, unsigned long pageFaults, unsigned long paginasSujas);

#endif
