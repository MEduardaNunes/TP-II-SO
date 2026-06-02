#ifndef TABELAINVERTIDA_H
#define TABELAINVERTIDA_H

#include "funcoesGerais.h"

// Estrutura da Tabela de Páginas Invertida
typedef struct EntradaTabelaInvertida {
    InformacoesEntrada informacoes;
    struct EntradaTabelaInvertida* proximo;
} EntradaTabelaInvertida;

typedef struct {
    EntradaTabelaInvertida* entradas;
    int capacidade;
} TabelaInvertida;

// Protótipos das funções para manipulação da tabela invertida
void inicializarEntradaTabelaInvertida(EntradaTabelaInvertida *entrada);
void inicializarTabelaInvertida(TabelaInvertida *tabela, int capacidade);
void destruirTabelaInvertida(TabelaInvertida *tabela);
int RANTabelaInvertida(TabelaInvertida *tabela);
int LRUTabelaInvertida(TabelaInvertida *tabela);
int MFUTabelaInvertida(TabelaInvertida *tabela);
int LFUTabelaInvertida(TabelaInvertida *tabela);
int selecionaPaginaParaSubstituirTabelaInvertida(TabelaInvertida *tabela, char *politicaSubstituicao);
void acessarPaginaTabelaInvertida(TabelaInvertida *tabela, int numeroPagina, char tipoAcesso);

#endif