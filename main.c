#include "header/simulador.h"
#include "header/tabelaDensa.h"
#include "header/funcoesGerais.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>


int main () {
    EspecificacaoSimulador simulador = {
        .numeroQuadros = 4,
        .politicaSubstituicao = "MFU",
        .simuladorTabelaDensa = {
            .tempo = 0,
            .estatisticas = {0, 0},
        },
        .simuladorTabelaInvertida = {
            .tempo = 0,
            .estatisticas = {0, 0},
        },
        .simuladorTabelaHierarquica2 = {
            .tempo = 0,
            .estatisticas = {0, 0},
        },
        .simuladorTabelaHierarquica3 = {
            .tempo = 0,
            .estatisticas = {0, 0},
        }
    };


    inicializarTabelaDensa(&simulador, simulador.numeroQuadros);
    acessarPaginaTabelaDensa(&simulador, 1, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 2, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 3, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 4, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 5, 'R'); // Isso deve causar um page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 6, 'W'); // Acessa a página 1 novamente, agora para escrita
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 8, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 3, 'W'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    acessarPaginaTabelaDensa(&simulador, 4, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaDensa.estatisticas);
    printf("Número de Acessos à Memória: %ld\n", simulador.simuladorTabelaDensa.estatisticas.numeroReferenciasMemoria);
    printf("Número de Page Faults: %ld\n", simulador.simuladorTabelaDensa.estatisticas.numeroPageFaults);
    printf("Número de Páginas Sujas Escritas: %ld\n", simulador.simuladorTabelaDensa.estatisticas.numeroPaginasSujasEscritas);
    printf("Número de Acessos à Tabela: %ld\n", simulador.simuladorTabelaDensa.estatisticas.acessosTabela);
    printf("Memória Consumida pela Tabela: %ld bytes\n", simulador.simuladorTabelaDensa.estatisticas.memoriaConsumida); 
    printf("Tempo total gasto: %d\n", simulador.simuladorTabelaDensa.tempo);

    destruirTabelaDensa(&simulador);

    inicializarTabelaInvertida(&simulador, simulador.numeroQuadros);
    acessarPaginaTabelaInvertida(&simulador, 1, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 2, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 3, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 4, 'R');
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 5, 'R'); // Isso deve causar um page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 6, 'W'); // Acessa a página 1 novamente, agora para escrita
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 8, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 3, 'W'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    acessarPaginaTabelaInvertida(&simulador, 4, 'R'); // Isso deve causar outro page fault e substituir uma página
    incrementarReferenciasMemoria(&simulador.simuladorTabelaInvertida.estatisticas);
    printf("Número de Acessos à Memória: %ld\n", simulador.simuladorTabelaInvertida.estatisticas.numeroReferenciasMemoria);
    printf("Número de Page Faults: %ld\n", simulador.simuladorTabelaInvertida.estatisticas.numeroPageFaults);
    printf("Número de Páginas Sujas Escritas: %ld\n", simulador.simuladorTabelaInvertida.estatisticas.numeroPaginasSujasEscritas);
    printf("Número de Acessos à Tabela: %ld\n", simulador.simuladorTabelaInvertida.estatisticas.acessosTabela);
    printf("Memória Consumida pela Tabela: %ld bytes\n", simulador.simuladorTabelaInvertida.estatisticas.memoriaConsumida); 
    printf("Tempo total gasto: %d\n", simulador.simuladorTabelaInvertida.tempo);

    destruirTabelaInvertida(&simulador);
    return 0;
}