#ifndef TABELA_HIERARQUICA_3
#define TABELA_HIERARQUICA_3

#include "funcoesGerais.h"

// Estrutura da Tabela de Páginas Hierárquica de três níveis
typedef struct {
    InformacoesEntrada informacoes;
} EntradaTabelaHierarquicaNivel3_3;

typedef struct {
    EntradaTabelaHierarquicaNivel3_3* entradas;
    int quantidadeEntradasPreenchidas;
    int capacidadeEntradasNivel2;
} EntradaTabelaHierarquicaNivel2_3;

typedef struct {
    EntradaTabelaHierarquicaNivel2_3* entradas;
    int quantidadeEntradasPreenchidas;
    int capacidadeEntradasNivel1;
} EntradaTabelaHierarquicaNivel1_3;

typedef struct {
    EntradaTabelaHierarquicaNivel1_3 tabelaBase;
    int quantidadeEntradasPreenchidas;
} TabelaHierarquica_3;

#endif
