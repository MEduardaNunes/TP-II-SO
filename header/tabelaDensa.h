#ifndef TABELADENSA_H
#define TABELADENSA_H

#include "funcoesGerais.h"
// Estrutura da Tabela de Página densa
typedef struct {
    InformacoesEntrada informacoes;
} EntradaTabelaDensa;

typedef struct {
    EntradaTabelaDensa* entradas;
    int quantidadeEntradasPreenchidas;
    int capacidade;
} TabelaDensa;

void inicializarTabelaDensa(TabelaDensa *tabela, int capacidade);
void destruirTabelaDensa(TabelaDensa *tabela);
void acessarPaginaTabelaDensa(TabelaDensa *tabela, int numeroPagina, char tipoAcesso);
int RANTabelaDensa(TabelaDensa *tabela);
int LRUTabelaDensa(TabelaDensa *tabela);
int MFUTabelaDensa(TabelaDensa *tabela);
int LFUTabelaDensa(TabelaDensa *tabela);
int selecionaPaginaParaSubstituirTabelaDensa(TabelaDensa *tabela, char *politicaSubstituicao);
void acessarPaginaTabelaDensa(TabelaDensa *tabela, int numeroPagina, char tipoAcesso);

#endif