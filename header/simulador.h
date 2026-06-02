#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "funcoesGerais.h"
#include "tabelaDensa.h"
#include "tabelaInvertida.h"
#include "tabelaHierarquica2.h"
#include "tabelaHierarquica3.h"

#include <stdlib.h>
#include <stdbool.h>


// Estrutura para armazenar as informações de acessos à memória
typedef struct {
    long numeroReferenciasMemoria;
    long numeroPageFaults;
    long numeroPaginasSujasEscritas;
} EstatisticasSimulador;

// Estrutura estatísticas da tabela de páginas
typedef struct {
    long acessosTabela;
    long memoriaConsumida;
} EstatisticasTabela;

void inicializarEstatisticasTabela(EstatisticasTabela *estatisticas);
void incrementarAcessosTabela(EstatisticasTabela *estatisticas);


// Estruturas para o simulador das diferentes tabelas de páginas
typedef struct{
    TabelaDensa tabela;
    bool *quadrosLivres;
    int tempo;
    EstatisticasTabela estatisticas;
    EstatisticasSimulador estatisticasSimulador;
} SimuladorTabelaDensa;

typedef struct{
    TabelaInvertida tabela;
    bool *quadrosLivres;
    int tempo;
    EstatisticasTabela estatisticas;
    EstatisticasSimulador estatisticasSimulador;
} SimuladorTabelaInvertida;

typedef struct{
    TabelaHierarquica_2 tabela;
    bool *quadrosLivres;
    int tempo;
    EstatisticasTabela estatisticas;
    EstatisticasSimulador estatisticasSimulador;
} SimuladorTabelaHierarquica2;

typedef struct{
    TabelaHierarquica_3 tabela;
    bool *quadrosLivres;
    int tempo;
    EstatisticasTabela estatisticas;
    EstatisticasSimulador estatisticasSimulador;
} SimuladorTabelaHierarquica3;


// Estrutura da especificação do simulador geral
typedef struct EspecificacaoSimulador {
    // Parâmetros de configuração do simulador
    int numeroPaginas;
    int numeroQuadros;
    int tamanhoPagina;
    int tamanhoMemoria;
    char* politicaSubstituicao;
    char* tabelaDePaginas;
    int tempo;

    // Estruturas de dados para as tabelas de páginas
    SimuladorTabelaDensa simuladorTabelaDensa;
    SimuladorTabelaInvertida simuladorTabelaInvertida;
    SimuladorTabelaHierarquica2 simuladorTabelaHierarquica2;
    SimuladorTabelaHierarquica3 simuladorTabelaHierarquica3;
} EspecificacaoSimulador;


#endif