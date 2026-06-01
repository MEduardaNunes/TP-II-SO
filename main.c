#include "header/simulador.h"
#include "header/tabelaDensa.h"
#include "header/funcoesGerais.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>


// Estruturas globais para o simulador
int tempo = 0;

EspecificacaoSimulador simulador = {
    .numeroPaginas = 0,
    .numeroQuadros = 0,
    .tamanhoPagina = 0,
    .tamanhoMemoria = 0,
    .politicaSubstituicao = "",
    .tabelaDePaginas = "",
    .estatisticasSimulador = {0, 0, 0}
};
EstatisticasTabela estatisticasTabelaDensa = {0, 0};
EstatisticasTabela estatisticasTabelaInvertida = {0, 0};
EstatisticasTabela estatisticasTabelaHierarquica2 = {0, 0};
EstatisticasTabela estatisticasTabelaHierarquica3 = {0, 0};


int main () {
    TabelaDensa tabelaDensa; inicializarTabelaDensa(&tabelaDensa, 4); 
    // Exemplo com capacidade para 8 páginas
    simulador.numeroQuadros = 8; simulador.quadrosLivres = (bool*) malloc(simulador.numeroQuadros * sizeof(bool));
    simulador.politicaSubstituicao = "MFU"; // Exemplo de política de substituição
    for (int i = 0; i < simulador.numeroQuadros; i++) simulador.quadrosLivres[i] = true;

    acessarPaginaTabelaDensa(&tabelaDensa, 1, 'R');
    simulador.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&tabelaDensa, 2, 'R');
    simulador.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&tabelaDensa, 3, 'R');
    simulador.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&tabelaDensa, 4, 'R');
    simulador.estatisticasSimulador.numeroReferenciasMemoria++;
    acessarPaginaTabelaDensa(&tabelaDensa, 3, 'R'); // Isso deve causar um page fault e substituir uma página
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&tabelaDensa, 1, 'W'); // Acessa a página 1 novamente, agora para escrita
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&tabelaDensa, 6, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&tabelaDensa, 7, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&tabelaDensa, 8, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&tabelaDensa, 3, 'W'); // Isso deve causar outro page fault e substituir uma página
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    acessarPaginaTabelaDensa(&tabelaDensa, 4, 'R'); // Isso deve causar outro page fault e substituir uma página
    simulador.estatisticasSimulador.numeroReferenciasMemoria++; 
    printf("Número de Acessos à Memória: %ld\n", simulador.estatisticasSimulador.numeroReferenciasMemoria); 
    printf("Número de Page Faults: %ld\n", simulador.estatisticasSimulador.numeroPageFaults); 
    printf("Número de Páginas Sujas Escritas: %ld\n", simulador.estatisticasSimulador.numeroPaginasSujasEscritas); 
    printf("Número de Acessos à Tabela: %ld\n", estatisticasTabelaDensa.acessosTabela); 
    printf("Memória Consumida pela Tabela: %ld bytes\n", estatisticasTabelaDensa.memoriaConsumida);
}