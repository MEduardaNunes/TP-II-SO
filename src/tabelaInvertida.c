#include "../header/tabelaInvertida.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h> 


// Estruturas e funções auxiliares para manipulação da tabela densa
extern int tempo;
extern EspecificacaoSimulador simulador;
extern EstatisticasTabela estatisticasTabelaInvertida;

// Funções para manipulação Tabela Invertida
void inicializarEntradaTabelaInvertida(EntradaTabelaInvertida *entrada) {
    inicializarInformacoesEntrada(&entrada->informacoes);
    entrada->proximo = NULL;
}

void inicializarTabelaInvertida(TabelaInvertida *tabela, int capacidade) {
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

void destruirTabelaInvertida(TabelaInvertida *tabela) {
    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        while (entrada != NULL) {
            EntradaTabelaInvertida *temp = entrada;
            entrada = entrada->proximo;
            free(temp);
        }
    }
    free(tabela->entradas);
    tabela->entradas = NULL;
}

int RANTabelaInvertida(TabelaInvertida *tabela) {

}

int LRUTabelaInvertida(TabelaInvertida *tabela) {
    
}

int MFUTabelaInvertida(TabelaInvertida *tabela) {
    int paginaMaisFrequente = -1;
    int maxAcessos = -1;
    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        if (entrada->informacoes.quantidadeAcessos > maxAcessos) {
            maxAcessos = entrada->informacoes.quantidadeAcessos;
            paginaMaisFrequente = i;
        }
        estatisticasTabelaInvertida.acessosTabela++;

        while (entrada->proximo != NULL) {
            entrada = entrada->proximo;
            if (entrada->informacoes.quantidadeAcessos > maxAcessos) {
                maxAcessos = entrada->informacoes.quantidadeAcessos;
                paginaMaisFrequente = i;
            }
            estatisticasTabelaInvertida.acessosTabela++;
        }
    }
    
    return paginaMaisFrequente;
}

int LFUTabelaInvertida(TabelaInvertida *tabela) {
    int paginaMenosFrequente = -1;
    int minAcessos = INT_MAX;

    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        if (entrada->informacoes.quantidadeAcessos < minAcessos) {
            minAcessos = entrada->informacoes.quantidadeAcessos;
            paginaMenosFrequente = i;
        }
        estatisticasTabelaInvertida.acessosTabela++;

        while (entrada->proximo != NULL) {
            entrada = entrada->proximo;
            if (entrada->informacoes.quantidadeAcessos < minAcessos) {
                minAcessos = entrada->informacoes.quantidadeAcessos;
                paginaMenosFrequente = i;
            }
            estatisticasTabelaInvertida.acessosTabela++;
        }
    }

    return paginaMenosFrequente;
}

int selecionaPaginaParaSubstituirTabelaInvertida(TabelaInvertida *tabela, char *politicaSubstituicao) {
    if (strcmp(politicaSubstituicao, "LRU") == 0) {
        return LRUTabelaInvertida(tabela);

    } else if (strcmp(politicaSubstituicao, "RAND") == 0) {
        return RANTabelaInvertida(tabela);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaInvertida(tabela);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaInvertida(tabela);
    }

    return -1;
}

void acessarPaginaTabelaInvertida(TabelaInvertida *tabela, int numeroPagina, char tipoAcesso) {
    // Verificar se a página já está na tabela
    for (int i = 0; i < tabela->capacidade; i++) {
        EntradaTabelaInvertida *entrada = &tabela->entradas[i];
        while (entrada != NULL) {
            if (entrada->informacoes.numeroPagina == numeroPagina) {
                atualizarInformacoesEntrada(&entrada->informacoes, tempo, tipoAcesso);
                return;
            }
            entrada = entrada->proximo;
        }
    }

    // Página não encontrada, precisa ser inserida
    if (tabela->quantidadeEntradasPreenchidas < tabela->capacidade) {
        // Ainda há espaço na tabela, inserir a nova página
        EntradaTabelaInvertida *novaEntrada = (EntradaTabelaInvertida*) malloc(sizeof(EntradaTabelaInvertida));
        if (novaEntrada == NULL) {
            fprintf(stderr, "Erro de alocação\n");
            exit(EXIT_FAILURE);
        }
        inicializarEntradaTabelaInvertida(novaEntrada);
        preencherInformacoesEntrada(&novaEntrada->informacoes, numeroPagina, -1, tempo);
        novaEntrada->proximo = tabela->entradas[tabela->quantidadeEntradasPreenchidas];
        tabela->entradas[tabela->quantidadeEntradasPreenchidas] = *novaEntrada;
        tabela->quantidadeEntradasPreenchidas++;
        
    } else {
        // A tabela está cheia, precisa substituir uma página
        int indicePaginaSubstituir = selecionaPaginaParaSubstituirTabelaInvertida(tabela, simulador.politicaSubstituicao);
        if (indicePaginaSubstituir != -1) {
            EntradaTabelaInvertida *entradaSubstituir = &tabela->entradas[indicePaginaSubstituir];
            if (entradaSubstituir->informacoes.bitModificacao) {
                simulador.estatisticasSimulador.numeroPaginasSujasEscritas++;
            }
            preencherInformacoesEntrada(&entradaSubstituir->informacoes, numeroPagina, -1, tempo);
        }
    }
}