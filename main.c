#include "header/simulador.h"
#include "header/tabelaDensa.h"
#include "header/funcoesGerais.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

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


int main () {
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
    return 0;
}