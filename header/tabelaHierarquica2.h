#ifndef TABELA_HIERARQUICA_2_H
#define TABELA_HIERARQUICA_2_H

#include "funcoesGerais.h"

typedef struct EspecificacaoSimulador EspecificacaoSimulador;

// Estrutura da Tabela de Páginas Hierárquica de dois níveis
typedef struct {
    InformacoesEntrada informacoes;
    bool valido;
} EntradaTabelaHierarquicaNivel2_2;

typedef struct {
    EntradaTabelaHierarquicaNivel2_2 *tabelaInterna;
    bool alocada;
} EntradaTabelaHierarquicaNivel1_2;

typedef struct {
    EntradaTabelaHierarquicaNivel1_2 *tabelaExterna;
    int tamanhoTabelaExterna; 
    int tamanhoTabelaInterna;

    unsigned int bitsNivel1;
    unsigned int bitsNivel2;
} TabelaHierarquica_2;

void inicializarTabelaHierarquica2(EspecificacaoSimulador *simulador);
void destruirTabelaHierarquica2(EspecificacaoSimulador *simulador);
void acessarPaginaTabelaHierarquica_2(EspecificacaoSimulador *simulador, unsigned int numeroPagina, char tipoAcesso);

#endif // TABELA_HIERARQUICA_2_H