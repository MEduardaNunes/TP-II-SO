#include "../header/tabelaInvertida.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Estrutura complementar
typedef struct {
    int indiceTabela;
    int profundidadeLista;
} LocalizacaoEntrada;

// Funções para manipulação Tabela Invertida
void inicializarEntradaTabelaInvertida(EntradaTabelaInvertida *entrada) {
    inicializarInformacoesEntrada(&entrada->informacoes);
    entrada->proximo = NULL;
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

    //Inicializando o vetor de quadros livres
    simulador->simuladorTabelaInvertida.quadrosLivres = (bool*) malloc(capacidade * sizeof(bool));
    if (simulador->simuladorTabelaInvertida.quadrosLivres == NULL) {
        fprintf(stderr, "Erro de alocação de quadros livres\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < capacidade; i++) 
        simulador->simuladorTabelaInvertida.quadrosLivres[i] = true;

    //Inicializando tempo
    simulador->simuladorTabelaInvertida.tempo = 0;

    //Inicializando estatísticas
    inicializarEstatisticasTabela(&simulador->simuladorTabelaInvertida.estatisticas);
    simulador->simuladorTabelaInvertida.estatisticas.memoriaConsumida = capacidade * sizeof(EntradaTabelaInvertida);
}

void destruirTabelaInvertida(EspecificacaoSimulador *simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = tabela->entradas[i].proximo;
        while (entrada != NULL) {
            EntradaTabelaInvertida *temp = entrada;
            entrada = entrada->proximo;
            free(temp);
        }
    }
    free(tabela->entradas);
    tabela->entradas = NULL;
    
    free(simulador->simuladorTabelaInvertida.quadrosLivres);
    simulador->simuladorTabelaInvertida.quadrosLivres = NULL;
}

LocalizacaoEntrada RANTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;

}

LocalizacaoEntrada LRUTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    
}

LocalizacaoEntrada MFUTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    LocalizacaoEntrada tuplaEntrada = {-1, -1};
    int maxAcessos = -1;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        if (entrada->informacoes.quantidadeAcessos > maxAcessos) {
            maxAcessos = entrada->informacoes.quantidadeAcessos;
            tuplaEntrada.indiceTabela = i;
            tuplaEntrada.profundidadeLista = 0;
        }
        incrementarAcessosTabela(estatisticas);

        int j = 1;
        while (entrada->proximo != NULL) {
            entrada = entrada->proximo;

            if (entrada->informacoes.quantidadeAcessos > maxAcessos) {
                maxAcessos = entrada->informacoes.quantidadeAcessos;
                tuplaEntrada.indiceTabela = i;
                tuplaEntrada.profundidadeLista = j;
            }
            incrementarAcessosTabela(estatisticas);
            j++;
        }
    }
    
    return tuplaEntrada;
}

LocalizacaoEntrada LFUTabelaInvertida(EspecificacaoSimulador * simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    LocalizacaoEntrada tuplaEntrada = {-1, -1};
    int minAcessos = INT_MAX;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        if (entrada->informacoes.quantidadeAcessos < minAcessos) {
            minAcessos = entrada->informacoes.quantidadeAcessos;
            tuplaEntrada.indiceTabela = i;
            tuplaEntrada.profundidadeLista = 0;
        }
        incrementarAcessosTabela(estatisticas);

        int j = 1;
        while (entrada->proximo != NULL) {
            entrada = entrada->proximo;
            if (entrada->informacoes.quantidadeAcessos < minAcessos) {
                minAcessos = entrada->informacoes.quantidadeAcessos;
                tuplaEntrada.indiceTabela = i;
                tuplaEntrada.profundidadeLista = j;
            }
            incrementarAcessosTabela(estatisticas);
            j++;
        }
    }

    return tuplaEntrada;
}

LocalizacaoEntrada selecionaPaginaParaSubstituirTabelaInvertida(EspecificacaoSimulador *simulador) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    char* politicaSubstituicao = simulador->politicaSubstituicao;
    LocalizacaoEntrada tuplaEntrada = {-1, -1};

    if (strcmp(politicaSubstituicao, "LRU") == 0) {
        return LRUTabelaInvertida(simulador);

    } else if (strcmp(politicaSubstituicao, "RAND") == 0) {
        return RANTabelaInvertida(simulador);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaInvertida(simulador);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaInvertida(simulador);
    }

    return tuplaEntrada;
}

void adicionarEntradaTabelaInvertida(EspecificacaoSimulador *simulador, int numeroPagina, int numeroQuadro, char tipoAcesso) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    int *tempo = &simulador->simuladorTabelaInvertida.tempo;

    EntradaTabelaInvertida *novaEntrada = (EntradaTabelaInvertida*) malloc(sizeof(EntradaTabelaInvertida));

    if (novaEntrada == NULL) {
        fprintf(stderr, "Erro de alocação\n");
        exit(EXIT_FAILURE);
    }

    inicializarEntradaTabelaInvertida(novaEntrada);
    preencherInformacoesEntrada(&novaEntrada->informacoes, numeroPagina, numeroQuadro, *tempo);

    int index = numeroPagina % tabela->capacidade;
    novaEntrada->proximo = tabela->entradas[index].proximo;
    tabela->entradas[index].proximo = novaEntrada;
    simulador->simuladorTabelaInvertida.estatisticas.memoriaConsumida += sizeof(EntradaTabelaInvertida);

    if (tipoAcesso == 'W') setarBitModificacao(&novaEntrada->informacoes);
}

void substituirEntradaTabelaInvertida(EspecificacaoSimulador *simulador, LocalizacaoEntrada tupla, int numeroPagina, char tipoAcesso) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    
    EntradaTabelaInvertida *atual = &tabela->entradas[tupla.indiceTabela];
    for (int k = 0; k < tupla.profundidadeLista; k++) {
        atual = atual->proximo;
        incrementarAcessosTabela(estatisticas);
    }

    if (atual->informacoes.bitModificacao) {
        incrementarPaginasSujasEscritas(estatisticas);
    }

    int quadroOriginal = atual->informacoes.numeroQuadro;
    inicializarInformacoesEntrada(&atual->informacoes);
    preencherInformacoesEntrada(&atual->informacoes, numeroPagina, quadroOriginal, simulador->simuladorTabelaInvertida.tempo);
    
    if (tipoAcesso == 'W') setarBitModificacao(&atual->informacoes);
}

void acessarPaginaTabelaInvertida(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaInvertida *tabela = &simulador->simuladorTabelaInvertida.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaInvertida.estatisticas;
    int *tempo = &simulador->simuladorTabelaInvertida.tempo;
    (*tempo)++;

    int index = numeroPagina % tabela->capacidade;

    // Verificar se a página já existe na lista encadeada daquele índice
    EntradaTabelaInvertida *entrada = &tabela->entradas[index];
    while (entrada != NULL) {
        if (entrada->informacoes.numeroPagina == numeroPagina) {
            atualizarInformacoesEntrada(&entrada->informacoes, *tempo, tipoAcesso);
            return; // Encontrado!
        }
        entrada = entrada->proximo;
    }

    // Página não encontrada: é um page fault
    incrementarPageFaults(estatisticas);
    
    // Verificar se há quadros livres na memória física
    int numeroQuadro = achaPrimeiroQuadroLivre(simulador->simuladorTabelaInvertida.quadrosLivres, simulador->numeroQuadros);

    if (numeroQuadro != -1) {
        // Há RAM disponível, inserir sem substituir
        simulador->simuladorTabelaInvertida.quadrosLivres[numeroQuadro] = false;
        adicionarEntradaTabelaInvertida(simulador, numeroPagina, numeroQuadro, tipoAcesso);
        incrementarAcessosTabela(estatisticas);

    } else {
        // Memória cheia: Substituir usando a política escolhida
        LocalizacaoEntrada tupla = selecionaPaginaParaSubstituirTabelaInvertida(simulador);
        substituirEntradaTabelaInvertida(simulador, tupla, numeroPagina, tipoAcesso);
        incrementarAcessosTabela(estatisticas);
    }
}