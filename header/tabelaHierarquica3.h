#ifndef TABELA_HIERARQUICA_3
#define TABELA_HIERARQUICA_3

#include "funcoesGerais.h"

// Estrutura da Tabela de Páginas Hierárquica de três níveis
typedef struct {
    InformacoesEntrada informacoes;
    bool valido;
} EntradaTabelaHierarquicaNivel3_3;

typedef struct {
    EntradaTabelaHierarquicaNivel3_3 *tabelaNivel3;
    bool alocada;
} EntradaTabelaHierarquicaNivel2_3;

typedef struct {
    EntradaTabelaHierarquicaNivel2_3 *tabelaNivel2;
    bool alocada;
} EntradaTabelaHierarquicaNivel1_3;

typedef struct {
    EntradaTabelaHierarquicaNivel1_3 *tabelaNivel1;
    int tamanhoNivel1;
    int tamanhoNivel2;
    int tamanhoNivel3;
} TabelaHierarquica_3;

#endif
