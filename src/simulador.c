#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/simulador.h"
#include "../header/tabelaDensa.h"
#include "../header/tabelaInvertida.h"
#include "../header/tabelaHierarquica2.h"
#include "../header/tabelaHierarquica3.h"


void inicializarSimuladorTabelaDensa(EspecificacaoSimulador *simulador) {
    int capacidade = simulador->numeroPaginas;
    // Inicializando a tabela densa
    inicializarTabelaDensa(simulador, capacidade);
    
    // Inicializando o vetor de quadros livres
    simulador->simuladorTabelaDensa.quadrosLivres = (bool*) malloc(simulador->numeroQuadros * sizeof(bool));
    simulador->simuladorTabelaDensa.paginasPorQuadro = (int*) malloc(simulador->numeroQuadros * sizeof(int));
    for (int i = 0; i < simulador->numeroQuadros; i++) {
        simulador->simuladorTabelaDensa.quadrosLivres[i] = true;
        simulador->simuladorTabelaDensa.paginasPorQuadro[i] = -1;
    }
    simulador->simuladorTabelaDensa.numeroQuadroOcupados = 0;

    //Inicializando tempo
    simulador->simuladorTabelaDensa.tempo = 0;

    //Inicializando estatísticas
    inicializarEstatisticasTabela(&simulador->simuladorTabelaDensa.estatisticas);
    simulador->simuladorTabelaDensa.estatisticas.memoriaConsumida = capacidade * sizeof(EntradaTabelaDensa) + simulador->numeroQuadros * sizeof(bool) + simulador->numeroQuadros * sizeof(int);
}

void inicializarSimuladorTabelaHierarquica2(EspecificacaoSimulador *simulador) {
    inicializarTabelaHierarquica2(simulador);

    // Alocação dos quadros livres
    simulador->simuladorTabelaHierarquica2.quadrosLivres = (bool*) malloc(simulador->numeroQuadros * sizeof(bool));
    if (simulador->simuladorTabelaHierarquica2.quadrosLivres == NULL){
        fprintf(stderr, "Erro ao alocar quadrosLivres\n");
        exit(1);
    }

    // Alocação das páginas por quadro
    simulador->simuladorTabelaHierarquica2.paginasPorQuadro = (int*) malloc(simulador->numeroQuadros * sizeof(int));
    if (simulador->simuladorTabelaHierarquica2.paginasPorQuadro == NULL){
        fprintf(stderr, "Erro ao alocar paginasPorQuadro\n");
        exit(1);
    }

    for (int i = 0; i < simulador->numeroQuadros; i++) {
        simulador->simuladorTabelaHierarquica2.quadrosLivres[i] = true;
        simulador->simuladorTabelaHierarquica2.paginasPorQuadro[i] = -1;
    }
    simulador->simuladorTabelaHierarquica2.numeroQuadroOcupados = 0;
    simulador->simuladorTabelaHierarquica2.tempo = 0;

    inicializarEstatisticasTabela(&simulador->simuladorTabelaHierarquica2.estatisticas);
    simulador->simuladorTabelaHierarquica2.estatisticas.memoriaConsumida = simulador->simuladorTabelaHierarquica2.tabela.tamanhoTabelaExterna * sizeof(EntradaTabelaHierarquicaNivel1_2) + simulador->numeroQuadros * sizeof(int);
}

void inicializarSimuladorTabelaHierarquica3(EspecificacaoSimulador *simulador) {

}

void inicializarSimuladorTabelaInvertida(EspecificacaoSimulador *simulador) {
    int capacidade = simulador->numeroQuadros;
    // Inicializando a tabela densa
    inicializarTabelaInvertida(simulador, capacidade);
    
    // Inicializando o vetor de quadros livres
    simulador->simuladorTabelaInvertida.quadrosLivres = (bool*) malloc(simulador->numeroQuadros * sizeof(bool));
    for (int i = 0; i < simulador->numeroQuadros; i++) {
        simulador->simuladorTabelaInvertida.quadrosLivres[i] = true;
    }
    simulador->simuladorTabelaInvertida.numeroQuadroOcupados = 0;

    //Inicializando tempo
    simulador->simuladorTabelaInvertida.tempo = 0;

    //Inicializando estatísticas
    inicializarEstatisticasTabela(&simulador->simuladorTabelaInvertida.estatisticas);
    simulador->simuladorTabelaInvertida.estatisticas.memoriaConsumida = capacidade * sizeof(EntradaTabelaInvertida) + simulador->numeroQuadros * sizeof(bool);
}

void inicializarSimulador(EspecificacaoSimulador *especificacao, int numeroPaginas, int numeroQuadros, int tamanhoPagina, int tamanhoMemoria, char* politicaSubstituicao, char *arquivoLog, bool modoDebugAtivo) {
    especificacao->numeroPaginas = numeroPaginas;
    especificacao->numeroQuadros = numeroQuadros;
    especificacao->tamanhoPagina = tamanhoPagina;
    especificacao->tamanhoMemoria = tamanhoMemoria;
    strncpy(especificacao->politicaSubstituicao, politicaSubstituicao, MAX_POLITICA_SUBSTITUICAO - 1);
    especificacao->politicaSubstituicao[MAX_POLITICA_SUBSTITUICAO - 1] = '\0';
    strncpy(especificacao->arquivoLog, arquivoLog, MAX_ARQUIVO_LOG - 1);
    especificacao->arquivoLog[MAX_ARQUIVO_LOG - 1] = '\0';
    especificacao->modoDebugAtivo = modoDebugAtivo;

    inicializarSimuladorTabelaDensa(especificacao);
    inicializarSimuladorTabelaHierarquica2(especificacao);
    inicializarSimuladorTabelaHierarquica3(especificacao);
    inicializarSimuladorTabelaInvertida(especificacao);
}


void lerArgumentosTerminal(int argc, char *argv[], EspecificacaoSimulador *simulador) {
    // 4 argumentos + 1 de debug -> algortimo, arquivo com sequencia de endereços, 
    // tamanho de cada pagina/quadro kB, tamanho total da memoria fisica
    if (argc < 5 || argc > 6) {
        printf("Uso: %s <algoritmo> <arquivo_enderecos> <tamanho_pagina_kB> <tamanho_memoria_kB> [debug]\n", argv[0]);
        exit(1);
    }
    
    
    int tamanhoPagina = atoi(argv[3]);
    int tamanhoMemoria = atoi(argv[4]);

    if (tamanhoPagina <= 0 || tamanhoMemoria <= 0) {
        fprintf(stderr, "Erro: tamanho_pagina_kB e tamanho_memoria_kB devem ser maiores que zero.\n");
        exit(EXIT_FAILURE);
    }

    if ((tamanhoPagina & (tamanhoPagina - 1)) != 0) {
        fprintf(stderr, "Erro: tamanho_pagina_kB deve ser uma potência de dois.\n");
        exit(EXIT_FAILURE);
    }

    int numeroQuadros = tamanhoMemoria / tamanhoPagina;
    if (numeroQuadros <= 0) {
        fprintf(stderr, "Erro: número de quadros não pode ser zero.\n");
        exit(EXIT_FAILURE);
    }

    int deslocamento = calcularDeslocamento(tamanhoPagina);
    if (deslocamento >= 32) {
        fprintf(stderr, "Erro: deslocamento de página inválido.\n");
        exit(EXIT_FAILURE);
    }

    int numeroPaginas = 1 << (32 - deslocamento);
    
    char* politicaSubstituicao = argv[1];
    char* arquivoLog = argv[2];
    bool modoDebugAtivo = (argc == 6);

    inicializarSimulador(simulador, numeroPaginas, numeroQuadros, tamanhoPagina, tamanhoMemoria, politicaSubstituicao, arquivoLog, modoDebugAtivo);
}

void salvarResultadosDebug(EspecificacaoSimulador *simulador, unsigned long pageFaults, unsigned long paginasSujas) {
    char nomeArquivo[150];
    
    sprintf(nomeArquivo, "resultados/resultado_%s.csv", simulador->politicaSubstituicao);

    FILE *file = fopen(nomeArquivo, "a");
    if (file == NULL) {
        printf("Aviso: Nao foi possivel salvar o log em %s.\n", nomeArquivo);
        return;
    }

    fprintf(file, "%s;%s;%u;%u;%lu;%lu\n",
            simulador->politicaSubstituicao,
            simulador->arquivoLog,
            simulador->tamanhoMemoria,
            simulador->tamanhoPagina,
            pageFaults,
            paginasSujas);

    fclose(file);
}

void rodarSimulador(EspecificacaoSimulador *simulador) {
    unsigned int deslocamento = calcularDeslocamento(simulador->tamanhoPagina);

    // inicialização da estrutura
    FILE *file = fopen(simulador->arquivoLog, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo: %s\n", simulador->arquivoLog);
        return;
    }

    // TEMPORARIO
    FILE *fileDebugInv = NULL, *fileDebugDen = NULL;
    if (simulador->modoDebugAtivo) {
        char nome[150];
        sprintf(nome, "resultados/debug_invertida_%s.txt", simulador->politicaSubstituicao);
        fileDebugInv = fopen(nome, "w");
        sprintf(nome, "resultados/debug_densa_%s.txt", simulador->politicaSubstituicao);
        fileDebugDen = fopen(nome, "w");
    }

    unsigned int endereco;
    char rw;

    if (simulador->modoDebugAtivo) printf("Iniciando leitura...\n");

    while (fscanf(file, "%x %c", &endereco, &rw) == 2) {
        // Calcula número da página
        unsigned int numeroPagina = extrairNumeroPagina(endereco, deslocamento);

        // Acessar tabelas
        acessarPaginaTabelaDensa(simulador, numeroPagina, rw);
        simulador->simuladorTabelaDensa.estatisticas.numeroReferenciasMemoria++;

        acessarPaginaTabelaInvertida(simulador, numeroPagina, rw);
        simulador->simuladorTabelaInvertida.estatisticas.numeroReferenciasMemoria++;
        
        acessarPaginaTabelaHierarquica_2(simulador, numeroPagina, rw);
        simulador->simuladorTabelaHierarquica2.estatisticas.numeroReferenciasMemoria++;
        // Modo debug
        if (simulador->modoDebugAtivo) {
            printf("Lido: Endereco=%08x, Pagina=%u, Operacao=%c\n", endereco, numeroPagina, rw);
        }

        if (fileDebugDen) fprintf(fileDebugDen, "Lido: Endereco=%08x, Pagina=%u, Operacao=%c\n", endereco, numeroPagina, rw);
        if (fileDebugInv) fprintf(fileDebugInv, "Lido: Endereco=%08x, Pagina=%u, Operacao=%c\n", endereco, numeroPagina, rw);
    }

    fclose(file);
    if (fileDebugInv) fclose(fileDebugInv); // TEMP
    if (fileDebugDen) fclose(fileDebugDen); // TEMP
}

void imprimirEspecificacoesGerais(EspecificacaoSimulador *simulador) {
    printf("======================= CONFIGURAÇÕES GERAIS =======================\n");
    printf("Arquivo de entrada: %s\n", simulador->arquivoLog);
    printf("Tamanho da memoria: %u KB\n", simulador->tamanhoMemoria);
    printf("Tamanho das paginas: %u KB\n", simulador->tamanhoPagina);
    printf("Política de substituição: %s\n", simulador->politicaSubstituicao);
}

static void imprimirMemoriaConsumida(long memoriaConsumida) {
    if (memoriaConsumida < 1024) {
        printf("Memória consumida: %ld bytes\n", memoriaConsumida);

    } else if (memoriaConsumida % 1024 == 0) {
        printf("Memória consumida: %ld KB\n", memoriaConsumida / 1024);

    } else {
        printf("Memória consumida: %.2f KB\n", memoriaConsumida / 1024.0);
    }
}

void imprimirResultadosTabelaDensa(EspecificacaoSimulador *simulador) {
    printf("--------------------------- TABELA DENSA ---------------------------\n");
    printf("Page faults: %lu\n", simulador->simuladorTabelaDensa.estatisticas.numeroPageFaults);
    printf("Paginas escritas: %lu\n", simulador->simuladorTabelaDensa.estatisticas.numeroPaginasSujasEscritas);
    imprimirMemoriaConsumida(simulador->simuladorTabelaDensa.estatisticas.memoriaConsumida);
    printf("Acessos à tabela: %lu\n", simulador->simuladorTabelaDensa.estatisticas.acessosTabela);
    salvarResultadosDebug(simulador, simulador->simuladorTabelaDensa.estatisticas.numeroPageFaults, simulador->simuladorTabelaDensa.estatisticas.numeroPaginasSujasEscritas);
}

void imprimirResultadosTabelaHierarquica2(EspecificacaoSimulador *simulador) {
    printf("-------------------- TABELA HIERARQUICA 2 NIVEIS -------------------\n");
    printf("Page faults: %lu\n", simulador->simuladorTabelaHierarquica2.estatisticas.numeroPageFaults);
    printf("Paginas escritas: %lu\n", simulador->simuladorTabelaHierarquica2.estatisticas.numeroPaginasSujasEscritas);
    imprimirMemoriaConsumida(simulador->simuladorTabelaHierarquica2.estatisticas.memoriaConsumida);
    printf("Acessos à tabela: %lu\n", simulador->simuladorTabelaHierarquica2.estatisticas.acessosTabela);
    salvarResultadosDebug(simulador, simulador->simuladorTabelaHierarquica2.estatisticas.numeroPageFaults, simulador->simuladorTabelaHierarquica2.estatisticas.numeroPaginasSujasEscritas);
}

void imprimirResultadosTabelaHierarquica3(EspecificacaoSimulador *simulador) {

}

void imprimirResultadosTabelaInvertida(EspecificacaoSimulador *simulador) {
    printf("------------------------- TABELA INVERTIDA -------------------------\n");
    printf("Page faults: %lu\n", simulador->simuladorTabelaInvertida.estatisticas.numeroPageFaults);
    printf("Paginas escritas: %lu\n", simulador->simuladorTabelaInvertida.estatisticas.numeroPaginasSujasEscritas);
    imprimirMemoriaConsumida(simulador->simuladorTabelaInvertida.estatisticas.memoriaConsumida);
    printf("Acessos à tabela: %lu\n", simulador->simuladorTabelaInvertida.estatisticas.acessosTabela);
    salvarResultadosDebug(simulador, simulador->simuladorTabelaInvertida.estatisticas.numeroPageFaults, simulador->simuladorTabelaInvertida.estatisticas.numeroPaginasSujasEscritas);
}

void imprimirSimulador(EspecificacaoSimulador *simulador) {
    imprimirEspecificacoesGerais(simulador);
    imprimirResultadosTabelaDensa(simulador);
    imprimirResultadosTabelaHierarquica2(simulador);
    imprimirResultadosTabelaHierarquica3(simulador);
    imprimirResultadosTabelaInvertida(simulador);
    printf("\n");
}

void destruirSimulador(EspecificacaoSimulador *simulador) {
    destruirTabelaDensa(simulador);
    destruirTabelaInvertida(simulador);
}