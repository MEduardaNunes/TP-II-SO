#ifndef TABELA_HIERARQUICA_2_H
#define TABELA_HIERARQUICA_2_H

#include "funcoesGerais.h"

// Estrutura da Tabela de Páginas Hierárquica de dois níveis
typedef struct {
    InformacoesEntrada informacoes;
} EntradaTabelaHierarquicaNivel2_2;

typedef struct {
    EntradaTabelaHierarquicaNivel2_2* entradas;
    int quantidadeEntradasPreenchidas;
    int capacidadeEntradasNivel1;
} EntradaTabelaHierarquicaNivel1_2;

typedef struct {
    EntradaTabelaHierarquicaNivel1_2 tabelaBase;
    int quantidadeEntradasPreenchidas;
} TabelaHierarquica_2;


#endif // TABELA_HIERARQUICA_2_H