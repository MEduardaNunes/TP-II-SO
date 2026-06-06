#ifndef TABELAINVERTIDA_H
#define TABELAINVERTIDA_H

#include "funcoesGerais.h"

typedef struct EspecificacaoSimulador EspecificacaoSimulador;

// Estrutura da Tabela de Páginas Invertida
typedef struct EntradaTabelaInvertida {
    InformacoesEntrada informacoes;
    bool valido;
} EntradaTabelaInvertida;

typedef struct {
    EntradaTabelaInvertida* entradas;
    int capacidade;
} TabelaInvertida;

// Protótipos das funções para manipulação da tabela invertida
void inicializarTabelaInvertida(EspecificacaoSimulador *simulador, int capacidade);
void destruirTabelaInvertida(EspecificacaoSimulador *simulador);
void acessarPaginaTabelaInvertida(EspecificacaoSimulador *simulador, unsigned int numeroPagina, char tipoAcesso);

#endif