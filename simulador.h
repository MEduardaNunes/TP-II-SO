#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// Estrutura para armazenar as informações de acessos à memória
typedef struct {
    long numeroReferenciasMemoria;
    long numeroPageFaults;
    long numeroPaginasSujasEscritas;
} EstatisticasSimulador;


// Estrutura da especificação do simulador
typedef struct {
    int numeroPaginas;
    int numeroQuadros;
    int tamanhoPagina;
    int tamanhoMemoria;
    char* politicaSubstituicao;
    char* tabelaDePaginas;
    bool *quadrosLivres;
    EstatisticasSimulador estatisticasSimulador;
} EspecificacaoSimulador;


// Estrutura estatísticas da tabela de páginas
typedef struct {
    long acessosTabela;
    long memoriaConsumida;
} EstatisticasTabela;


// Estrutura de tempo
extern int tempo;


// Informações gerais da entrada da tabela de páginas
typedef struct {
    int numeroPagina;
    int numeroQuadro;
    bool bitPresenca;
    bool bitModificacao;
    int ultimoAcesso;
    int quantidadeAcessos;
} InformacoesEntrada;


// Estrutura da Tabela de Página densa
typedef struct {
    InformacoesEntrada informacoes;
} EntradaTabelaDensa;

typedef struct {
    EntradaTabelaDensa* entradas;
    int quantidadeEntradasPreenchidas;
    int capacidade;
} TabelaDensa;


// Estrutura da Tabela de Páginas Invertida
typedef struct EntradaTabelaInvertida {
    InformacoesEntrada informacoes;
    struct EntradaTabelaInvertida* proximo;
} EntradaTabelaInvertida;

typedef struct {
    EntradaTabelaInvertida* entradas;
    int capacidade;
} TabelaInvertida;


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