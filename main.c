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
            .estatisticasSimulador = {0, 0, 0},
        },
        .simuladorTabelaInvertida = {
            .tempo = 0,
            .estatisticas = {0, 0},
            .estatisticasSimulador = {0, 0, 0},
        },
        .simuladorTabelaHierarquica2 = {
            .tempo = 0,
            .estatisticas = {0, 0},
            .estatisticasSimulador = {0, 0, 0},
        },
        .simuladorTabelaHierarquica3 = {
            .tempo = 0,
            .estatisticas = {0, 0},
            .estatisticasSimulador = {0, 0, 0},
        }
    };

    simulador.simuladorTabelaDensa.quadrosLivres = malloc(simulador.numeroQuadros * sizeof(bool));
    simulador.simuladorTabelaInvertida.quadrosLivres = malloc(simulador.numeroQuadros * sizeof(bool));
    if (simulador.simuladorTabelaDensa.quadrosLivres == NULL || simulador.simuladorTabelaInvertida.quadrosLivres == NULL) {
        fprintf(stderr, "Erro de alocação de quadros livres\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < simulador.numeroQuadros; i++)
    {
        simulador.simuladorTabelaDensa.quadrosLivres[i] = true;
        simulador.simuladorTabelaInvertida.quadrosLivres[i] = true;
    }

    inicializarTabelaDensa(&simulador, simulador.numeroQuadros);

    acessarPaginaTabelaDensa(&simulador, 1, 'R');
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&simulador, 2, 'R');
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&simulador, 3, 'R');
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&simulador, 4, 'R');
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&simulador, 5, 'R'); // Isso deve causar um page fault e substituir uma página
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&simulador, 6, 'W'); // Acessa a página 1 novamente, agora para escrita
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&simulador, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&simulador, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&simulador, 8, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&simulador, 3, 'W'); // Isso deve causar outro page fault e substituir uma página
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&simulador, 4, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria++; 
    printf("Número de Acessos à Memória: %ld\n", simulador.simuladorTabelaDensa.estatisticasSimulador.numeroReferenciasMemoria); 
    printf("Número de Page Faults: %ld\n", simulador.simuladorTabelaDensa.estatisticasSimulador.numeroPageFaults); 
    printf("Número de Páginas Sujas Escritas: %ld\n", simulador.simuladorTabelaDensa.estatisticasSimulador.numeroPaginasSujasEscritas); 
    printf("Número de Acessos à Tabela: %ld\n", simulador.simuladorTabelaDensa.estatisticas.acessosTabela); 
    printf("Memória Consumida pela Tabela: %ld bytes\n", simulador.simuladorTabelaDensa.estatisticas.memoriaConsumida); 

    destruirTabelaDensa(&simulador);
    free(simulador.simuladorTabelaDensa.quadrosLivres);
    free(simulador.simuladorTabelaInvertida.quadrosLivres);

    return 0;
}