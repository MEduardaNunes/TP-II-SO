#include "../header/tabelaInvertida.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Funções para manipulação Tabela Invertida
void inicializarEntradaTabelaInvertida(EntradaTabelaInvertida *entrada) {
    inicializarInformacoesEntrada(&entrada->informacoes);
    entrada->valido = false;
}

void inicializarTabelaInvertida(EspecificacaoSimulador *simulador, int capacidade) {
    // Inicializando a tabela invertida
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    tabela->entradas = (EntradaTabelaInvertida*) malloc(capacidade * sizeof(EntradaTabelaInvertida));
    if (tabela->entradas == NULL) {
        fprintf(stderr, "Erro de alocação\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < capacidade; i++) {
        inicializarEntradaTabelaInvertida(&tabela->entradas[i]);
    }

    tabela->capacidade = capacidade;
}

void destruirTabelaInvertida(EspecificacaoSimulador *simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;

    free(tabela->entradas);
    tabela->entradas = NULL;
    
    free(simulador->simuladorTabelaInvertida.quadrosLivres);
    simulador->simuladorTabelaInvertida.quadrosLivres = NULL;
}

int RANTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;

}

int LRUTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    int index = -1;
    int minTempo = INT_MAX;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        incrementarAcessosTabela(estatisticas);

        if (!entrada->valido) continue;
            
        if (entrada->informacoes.ultimoAcesso < minTempo) {
            minTempo = entrada->informacoes.ultimoAcesso;
            index = i;
        }
    }
    return index;
}

int MFUTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    int index = -1;
    int maxAcessos = -1;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        incrementarAcessosTabela(estatisticas);

        if(!entrada->valido) continue;

        if (entrada->informacoes.quantidadeAcessos > maxAcessos) {
            maxAcessos = entrada->informacoes.quantidadeAcessos;
            index = i;
        }  
    }
    
    return index;
}

int LFUTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    int index = -1;
    int minAcessos = INT_MAX;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        incrementarAcessosTabela(estatisticas);
       
        if(!entrada->valido) continue;

        if (entrada->informacoes.quantidadeAcessos < minAcessos) {
            minAcessos = entrada->informacoes.quantidadeAcessos;
            index = i;
        }
    }

    return index;
}

int selecionaPaginaParaSubstituirTabelaInvertida(EspecificacaoSimulador *simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    char* politicaSubstituicao = simulador->politicaSubstituicao;

    if (strcmp(politicaSubstituicao, "LRU") == 0) {
        return LRUTabelaInvertida(simulador);

    } else if (strcmp(politicaSubstituicao, "RAND") == 0) {
        return RANTabelaInvertida(simulador);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaInvertida(simulador);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaInvertida(simulador);
    }

    return -1;
}

void adicionarEntradaTabelaInvertida(EspecificacaoSimulador *simulador, int numeroPagina, int numeroQuadro, char tipoAcesso) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    int *tempo = &simulador->simuladorTabelaInvertida.tempo;

    EntradaTabelaInvertida *entrada = &tabela->entradas[numeroQuadro];
    preencherInformacoesEntrada(&entrada->informacoes, numeroPagina, numeroQuadro, *tempo);
    entrada->valido = true;
    simulador->simuladorTabelaInvertida.numeroQuadroOcupados++;

    if (tipoAcesso == 'W') setarBitModificacao(&entrada->informacoes);
}

void substituirEntradaTabelaInvertida(EspecificacaoSimulador *simulador, int index, int numeroPagina, char tipoAcesso) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    
    if (index < 0 || index >= tabela->capacidade) {
        fprintf(stderr, "Erro: índice de substituição inválido para tabela invertida.\n");
        return;
    }

    EntradaTabelaInvertida *atual = &tabela->entradas[index];
    if (atual->informacoes.bitModificacao) incrementarPaginasSujasEscritas(estatisticas);

    int quadroOriginal = atual->informacoes.numeroQuadro;
    inicializarInformacoesEntrada(&atual->informacoes);
    preencherInformacoesEntrada(&atual->informacoes, numeroPagina, quadroOriginal, simulador->simuladorTabelaInvertida.tempo);
    atual->valido = true;
    
    if (tipoAcesso == 'W') setarBitModificacao(&atual->informacoes);
}

void acessarPaginaTabelaInvertida(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    int *tempo = &simulador->simuladorTabelaInvertida.tempo;
    (*tempo)++;

    // Verificar se a página já existe na tabela
    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        if (!entrada->valido) continue;
        if (entrada->informacoes.numeroPagina == numeroPagina) {
            atualizarInformacoesEntrada(&entrada->informacoes, *tempo, tipoAcesso);
            incrementarAcessosTabela(estatisticas);
            return; // Encontrado!
        }
    }

    // Página não encontrada: é um page fault
    incrementarPageFaults(estatisticas);
    
    // Verificar se há quadros livres na memória física
    int quadrosOcupados = simulador->simuladorTabelaInvertida.numeroQuadroOcupados;

    if (quadrosOcupados < simulador->numeroQuadros) {
        // Há RAM disponível, inserir sem substituir
        int numeroQuadro = achaPrimeiroQuadroLivre(simulador->simuladorTabelaInvertida.quadrosLivres, simulador->numeroQuadros);
        simulador->simuladorTabelaInvertida.quadrosLivres[numeroQuadro] = false;
        adicionarEntradaTabelaInvertida(simulador, numeroPagina, numeroQuadro, tipoAcesso);
        incrementarAcessosTabela(estatisticas);

    } else {
        // Memória cheia: Substituir usando a política escolhida
        int index = selecionaPaginaParaSubstituirTabelaInvertida(simulador);
        substituirEntradaTabelaInvertida(simulador, index, numeroPagina, tipoAcesso);
        incrementarAcessosTabela(estatisticas);
    }
}
