#ifndef TABELADENSA_H
#define TABELADENSA_H

#include "funcoesGerais.h"

typedef struct EspecificacaoSimulador EspecificacaoSimulador;

// Estrutura da Tabela de Página densa
typedef struct {
    InformacoesEntrada informacoes;
    bool valido;
} EntradaTabelaDensa;

typedef struct {
    EntradaTabelaDensa* entradas;
    int capacidade;
} TabelaDensa;

// Protótipos das funções para manipulação da tabela densa
void inicializarTabelaDensa(EspecificacaoSimulador *simulador, int capacidade);
void destruirTabelaDensa(EspecificacaoSimulador *simulador);
void acessarPaginaTabelaDensa(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso);

#endif