#ifndef TABELA_HIERARQUICA_2_H
#define TABELA_HIERARQUICA_2_H

#include "funcoesGerais.h"

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


#endif // TABELA_HIERARQUICA_2_H