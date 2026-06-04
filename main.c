#include "header/simulador.h"
#include "header/tabelaDensa.h"
#include "header/funcoesGerais.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

// FUNCAO TEMPORARIA
void salvarResultadosDebug(EspecificacaoSimulador *simulador, unsigned long pageFaults, unsigned long paginasSujas) {
    char nomeArquivo[150];
    
    sprintf(nomeArquivo, "resultados/resultado_%s_%s.csv", simulador->tabelaDePaginas, simulador->politicaSubstituicao);

    FILE *file = fopen(nomeArquivo, "a");
    if (file == NULL) {
        printf("Aviso: Nao foi possivel salvar o log em %s.\n", nomeArquivo);
        return;
    }

    fprintf(file, "%s;%s;%s;%u;%u;%lu;%lu\n",
            simulador->tabelaDePaginas,
            simulador->politicaSubstituicao,
            simulador->arquivoLog,
            simulador->tamanhoMemoria,
            simulador->tamanhoPagina,
            pageFaults,
            paginasSujas);

    fclose(file);
}

void lerArgumentosTerminal(int argc, char *argv[], EspecificacaoSimulador *simulador) {
    // 4 argumentos + 1 de debug -> algortimo, arquivo com sequencia de endereços, 
    // tamanho de cada pagina/quadro kB, tamanho total da memoria fisica
    if (argc < 5 || argc > 6) {
        printf("Uso: %s <algoritmo> <arquivo_enderecos> <tamanho_pagina_kB> <tamanho_memoria_kB> [debug]\n", argv[0]);
        exit(1);
    }

    simulador->politicaSubstituicao = argv[1];
    simulador->arquivoLog = argv[2];
    simulador->tamanhoPagina = atoi(argv[3]);
    simulador->tamanhoMemoria = atoi(argv[4]);

    // Calcula a quantidade de quadros
    simulador->numeroQuadros = simulador->tamanhoMemoria / simulador->tamanhoPagina;
    simulador->modoDebugAtivo = (argc == 6);
}

unsigned int calcularDeslocamento(unsigned int tamanhoPagina) {
    unsigned int deslocamento = 0;
    unsigned int temp = tamanhoPagina * 1024;
    while (temp > 1) {
        deslocamento++;
        temp >>= 1; 
    }
    return deslocamento;
}

unsigned int extrairNumeroPagina(unsigned int endereco, unsigned int deslocamento) {
    return endereco >> deslocamento;
}

void executarTesteCompleto(EspecificacaoSimulador *simulador, bool ehTabelaDensa) {
    printf("\n--- INICIANDO TESTE DE ESTRESSE: %s ---\n", ehTabelaDensa ? "TABELA DENSA" : "TABELA INVERTIDA");

    // Sequência de acesso (Página, Tipo de Acesso)
    // 1. Enchendo a memória (Capacidade: 4 quadros)
    // 2. Acesso a página que já está na memória (Hit)
    // 3. Escrita que suja uma página
    // 4. Substituição forçada
    
    int paginas[] = {1, 2, 3, 4, 2, 5, 1, 6, 2, 3};
    char tipos[]  = {'R', 'W', 'R', 'R', 'R', 'R', 'W', 'R', 'W', 'R'};
    int totalAcessos = 10;

    for (int i = 0; i < totalAcessos; i++) {
        if (ehTabelaDensa) {
            acessarPaginaTabelaDensa(simulador, paginas[i], tipos[i]);
        } else {
            acessarPaginaTabelaInvertida(simulador, paginas[i], tipos[i]);
        }
        
        // Simula a contagem global de referências
        if (ehTabelaDensa) {
            incrementarReferenciasMemoria(&simulador->simuladorTabelaDensa.estatisticas);
        } else {
            incrementarReferenciasMemoria(&simulador->simuladorTabelaInvertida.estatisticas);
        }
    }

    // Exibição dos resultados
    EstatisticasTabela *estat = ehTabelaDensa ? &simulador->simuladorTabelaDensa.estatisticas : &simulador->simuladorTabelaInvertida.estatisticas;
    printf("Acessos à Memória: %ld\n", estat->numeroReferenciasMemoria);
    printf("Acessos à tabela: %ld\n", estat->acessosTabela);
    printf("Page Faults: %ld\n", estat->numeroPageFaults);
    printf("Páginas Sujas Escritas: %ld\n", estat->numeroPaginasSujasEscritas);
    printf("Memória Consumida: %ld bytes\n", estat->memoriaConsumida);
}


int main (int argc, char *argv[]) {
    /*
    EspecificacaoSimulador simulador = {
        .numeroPaginas = 8,
        .numeroQuadros = 4,
        .politicaSubstituicao = "MFU",
        .simuladorTabelaDensa = {
            .tempo = 0,
            .numeroQuadroOcupados = 0,
            .estatisticas = {0, 0},
        },
        .simuladorTabelaInvertida = {
            .tempo = 0,
            .numeroQuadroOcupados = 0,
            .estatisticas = {0, 0},
        },
        .simuladorTabelaHierarquica2 = {
            .tempo = 0,
            .numeroQuadroOcupados = 0,
            .estatisticas = {0, 0},
        },
        .simuladorTabelaHierarquica3 = {
            .tempo = 0,
            .numeroQuadroOcupados = 0,
            .estatisticas = {0, 0},
        }
    };

    inicializarTabelaDensa(&simulador, simulador.numeroPaginas);
    inicializarTabelaInvertida(&simulador, simulador.numeroQuadros);

    executarTesteCompleto(&simulador, 1);
    executarTesteCompleto(&simulador, 0);

    destruirTabelaDensa(&simulador);
    destruirTabelaInvertida(&simulador);
    */

    // processamento inicial
    EspecificacaoSimulador simulador = {0};
    lerArgumentosTerminal(argc, argv, &simulador);
    unsigned int deslocamento = calcularDeslocamento(simulador.tamanhoPagina);

    // inicialização da estrutura
    simulador.tabelaDePaginas = "invertida";
    inicializarTabelaInvertida(&simulador, simulador.numeroQuadros);
    
    FILE *file = fopen(simulador.arquivoLog, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", simulador.arquivoLog);
        return 1;
    }

    unsigned int endereco;
    char rw;

    if (simulador.modoDebugAtivo) printf("Iniciando leitura...\n");

    while (fscanf(file, "%x %c", &endereco, &rw) == 2) {
        unsigned int numeroPagina = extrairNumeroPagina(endereco, deslocamento);

        acessarPaginaTabelaInvertida(&simulador, numeroPagina, rw);
        simulador.simuladorTabelaInvertida.estatisticas.numeroReferenciasMemoria++;
    
        if (simulador.modoDebugAtivo) {
            printf("Lido: Endereco=%08x, Pagina=%u, Operacao=%c\n", endereco, numeroPagina, rw);
        }
    }

    fclose(file);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", simulador.arquivoLog);
    printf("Tamanho da memoria: %u KB\n", simulador.tamanhoMemoria);
    printf("Tamanho das paginas: %u KB\n", simulador.tamanhoPagina);
    printf("Tecnica de reposicao: %s\n", simulador.politicaSubstituicao);
    printf("Paginas lidas: %lu\n", simulador.simuladorTabelaInvertida.estatisticas.numeroPageFaults);
    printf("Paginas escritas: %lu\n", simulador.simuladorTabelaInvertida.estatisticas.numeroPaginasSujasEscritas);

    salvarResultadosDebug(&simulador, simulador.simuladorTabelaInvertida.estatisticas.numeroPageFaults, simulador.simuladorTabelaInvertida.estatisticas.numeroPaginasSujasEscritas);
    
    destruirTabelaInvertida(&simulador);

    simulador.tabelaDePaginas = "densa";
    unsigned int totalPaginasPossiveis = 1 << (32 - deslocamento); // capacidade da dens = numero total de paginas logicas possiveis
    inicializarTabelaDensa(&simulador, totalPaginasPossiveis);

    file = fopen(simulador.arquivoLog, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", simulador.arquivoLog);
        return 1;
    }

    if (simulador.modoDebugAtivo) printf("Iniciando leitura...\n");

    while (fscanf(file, "%x %c", &endereco, &rw) == 2) {
        unsigned int numeroPagina = extrairNumeroPagina(endereco, deslocamento);

        acessarPaginaTabelaDensa(&simulador, numeroPagina, rw);
        simulador.simuladorTabelaDensa.estatisticas.numeroReferenciasMemoria++;
    
        if (simulador.modoDebugAtivo) {
            printf("Lido: Endereco=%08x, Pagina=%u, Operacao=%c\n", endereco, numeroPagina, rw);
        }
    }

    fclose(file);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", simulador.arquivoLog);
    printf("Tamanho da memoria: %u KB\n", simulador.tamanhoMemoria);
    printf("Tamanho das paginas: %u KB\n", simulador.tamanhoPagina);
    printf("Tecnica de reposicao: %s\n", simulador.politicaSubstituicao);
    printf("Paginas lidas: %lu\n", simulador.simuladorTabelaDensa.estatisticas.numeroPageFaults);
    printf("Paginas escritas: %lu\n", simulador.simuladorTabelaDensa.estatisticas.numeroPaginasSujasEscritas);

    salvarResultadosDebug(&simulador, simulador.simuladorTabelaDensa.estatisticas.numeroPageFaults, simulador.simuladorTabelaDensa.estatisticas.numeroPaginasSujasEscritas);
    
    destruirTabelaDensa(&simulador);

    return 0;
}