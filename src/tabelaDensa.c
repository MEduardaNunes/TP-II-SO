#include "../header/tabelaDensa.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


// Funções para manipulação Tabela Densa
void inicializarTabelaDensa(EspecificacaoSimulador *simulador, int capacidade) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    tabela->entradas = (EntradaTabelaDensa*) malloc(capacidade * sizeof(EntradaTabelaDensa));

    if (tabela->entradas == NULL) {
        fprintf(stderr, "Erro de alocação\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < capacidade; i++) {
        inicializarInformacoesEntrada(&tabela->entradas[i].informacoes);
    }

    tabela->quantidadeEntradasPreenchidas = 0;
    tabela->capacidade = capacidade;
    simulador->simuladorTabelaDensa.estatisticas.memoriaConsumida = capacidade * sizeof(EntradaTabelaDensa);
}

void destruirTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    free(tabela->entradas);
    tabela->entradas = NULL;
}


int RANTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    
}

int LRUTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;

}

int MFUTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaDensa.estatisticas;
    int paginaMaisFrequente = -1;
    int maxAcessos = -1;

    for (int i = 0; i < tabela->quantidadeEntradasPreenchidas; i++) {
        InformacoesEntrada *entrada_i = &tabela->entradas[i].informacoes;
        incrementarAcessosTabela(estatisticas);

        if (entrada_i->quantidadeAcessos > maxAcessos) {
            maxAcessos = entrada_i->quantidadeAcessos;
            paginaMaisFrequente = i;
        }
    }

    return paginaMaisFrequente;
}

int LFUTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaDensa.estatisticas;
    int paginaMenosFrequente = -1;
    int minAcessos = __INT_MAX__;

    for (int i = 0; i < tabela->quantidadeEntradasPreenchidas; i++) {
        InformacoesEntrada *entrada_i = &tabela->entradas[i].informacoes;
        incrementarAcessosTabela(estatisticas);

        if (entrada_i->quantidadeAcessos < minAcessos) {
            minAcessos = entrada_i->quantidadeAcessos;
            paginaMenosFrequente = i;
        }
    }

    return paginaMenosFrequente;
}

int selecionaPaginaParaSubstituirTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabelaDensa = &simulador->simuladorTabelaDensa.tabela;
    char *politicaSubstituicao = simulador->politicaSubstituicao;

    if (strcmp(politicaSubstituicao, "RAN") == 0) {
        return RANTabelaDensa(simulador);

    } else if (strcmp(politicaSubstituicao, "LRU") == 0) {
        return LRUTabelaDensa(simulador);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaDensa(simulador);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaDensa(simulador);
    }

    return -1;
}

void adicionarEntradaTabelaDensa(EspecificacaoSimulador *simulador, int numeroPagina, int numeroQuadro, char tipoAcesso) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    int pos = tabela->quantidadeEntradasPreenchidas;
    InformacoesEntrada *entrada = &tabela->entradas[pos].informacoes;
    int *tempo = &simulador->simuladorTabelaDensa.tempo;

    preencherInformacoesEntrada(entrada, numeroPagina, numeroQuadro, *tempo);

    if (tipoAcesso == 'W') {
        setarBitModificacao(entrada);
    }

    tabela->quantidadeEntradasPreenchidas++;
}

void substituirEntradaTabelaDensa(EspecificacaoSimulador *simulador, int pos, int numeroPagina, int numeroQuadro, char tipoAcesso) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    int *tempo = &simulador->simuladorTabelaDensa.tempo;
    InformacoesEntrada *entrada = &tabela->entradas[pos].informacoes;

    inicializarInformacoesEntrada(entrada);
    preencherInformacoesEntrada(entrada, numeroPagina, numeroQuadro, *tempo);

    if (tipoAcesso == 'W') {
        setarBitModificacao(entrada);
    }
}

void acessarPaginaTabelaDensa(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaDensa.estatisticas;
    EstatisticasSimulador *estatisticasSimulador = &simulador->simuladorTabelaDensa.estatisticasSimulador;
    int *tempo = &simulador->simuladorTabelaDensa.tempo;
    
    // Verificar se já existe uma entrada para a página
    for (int i = 0; i < tabela->quantidadeEntradasPreenchidas; i++) {
        InformacoesEntrada *entrada_i = &tabela->entradas[i].informacoes;
        (*tempo)++;
        incrementarAcessosTabela(estatisticas);

        if (entrada_i->numeroPagina == numeroPagina) {
            // Página já está presente, atualizar as informações
            atualizarInformacoesEntrada(entrada_i, *tempo, tipoAcesso);
            (*tempo)++;
            return;
        }
    }

    bool *quadrosLivres = simulador->simuladorTabelaDensa.quadrosLivres;
    // Se não existe uma entrada para a página, adicionar uma nova entrada
    if (tabela->quantidadeEntradasPreenchidas < tabela->capacidade) {
        // Se tem espaço na tabela, adicionar a nova entrada    
        int quadroLivre = achaPrimeiroQuadroLivre(quadrosLivres, simulador->numeroQuadros);

        if (quadroLivre != -1) {
            quadrosLivres[quadroLivre] = false;
            adicionarEntradaTabelaDensa(simulador, numeroPagina, quadroLivre, tipoAcesso);
            (*tempo)++;
            incrementarAcessosTabela(estatisticas);
            estatisticasSimulador->numeroPageFaults++;

        } else {
            printf("Erro: Não há quadros livres disponíveis.\n");
        }
 

    } else {
        // Se a tabela está cheia, selecionar uma página para substituir
        int paginaParaSubstituir = selecionaPaginaParaSubstituirTabelaDensa(simulador);

        if (paginaParaSubstituir != -1) {
            // Substituir a página selecionada
            InformacoesEntrada *entrada_substituir = &tabela->entradas[paginaParaSubstituir].informacoes;

            if (entrada_substituir->bitModificacao) {
                estatisticasSimulador->numeroPaginasSujasEscritas++;
            }

            int quadroSubstituido = entrada_substituir->numeroQuadro;
            quadrosLivres[quadroSubstituido] = false;
            substituirEntradaTabelaDensa(simulador, paginaParaSubstituir, numeroPagina, quadroSubstituido, tipoAcesso);

            (*tempo)++;
            incrementarAcessosTabela(estatisticas);
            estatisticasSimulador->numeroPageFaults++;

        } else {
            printf("Erro: Não foi possível substituir uma página.\n");
        }
    }
}