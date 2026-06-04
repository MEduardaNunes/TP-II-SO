#include "../header/tabelaDensa.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


// Funções para manipulação Tabela Densa
void inicializarTabelaDensa(EspecificacaoSimulador *simulador, int capacidade) {
    // Inicializando a tabela densa
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    tabela->entradas = (EntradaTabelaDensa*) malloc(capacidade * sizeof(EntradaTabelaDensa));

    if (tabela->entradas == NULL) {
        fprintf(stderr, "Erro de alocação\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < capacidade; i++) {
        inicializarInformacoesEntrada(&tabela->entradas[i].informacoes);
        tabela->entradas[i].valido = false;
    }

    tabela->capacidade = capacidade;
}


void destruirTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;

    free(tabela->entradas);
    tabela->entradas = NULL;
    free(simulador->simuladorTabelaDensa.quadrosLivres);
    simulador->simuladorTabelaDensa.quadrosLivres = NULL;
    free(simulador->simuladorTabelaDensa.paginasPorQuadro);
    simulador->simuladorTabelaDensa.paginasPorQuadro = NULL;
    tabela->capacidade = 0;
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

    for (int i = 0; i < simulador->numeroQuadros; i++) {
        int pagina = simulador->simuladorTabelaDensa.paginasPorQuadro[i];
        incrementarAcessosTabela(estatisticas);

        if (pagina == -1) continue;

        EntradaTabelaDensa *entrada_i = &tabela->entradas[pagina];
        if (entrada_i->valido && entrada_i->informacoes.quantidadeAcessos > maxAcessos) {
            maxAcessos = entrada_i->informacoes.quantidadeAcessos;
            paginaMaisFrequente = pagina;
        }
    }

    return paginaMaisFrequente;
}


int LFUTabelaDensa(EspecificacaoSimulador *simulador) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaDensa.estatisticas;
    int paginaMenosFrequente = -1;
    int minAcessos = __INT_MAX__;

    for (int i = 0; i < simulador->numeroQuadros; i++) {
        int pagina = simulador->simuladorTabelaDensa.paginasPorQuadro[i];
        incrementarAcessosTabela(estatisticas);

        if (pagina == -1) continue;

        EntradaTabelaDensa *entrada_i = &tabela->entradas[pagina];
        if (entrada_i->valido && entrada_i->informacoes.quantidadeAcessos < minAcessos) {
            minAcessos = entrada_i->informacoes.quantidadeAcessos;
            paginaMenosFrequente = pagina;
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


void adicionarEntradaTabelaDensa(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    int numeroQuadro = achaPrimeiroQuadroLivre(simulador->simuladorTabelaDensa.quadrosLivres, simulador->numeroQuadros);
    int tempo = simulador->simuladorTabelaDensa.tempo;
    InformacoesEntrada *entradaInformacoes = &tabela->entradas[numeroPagina].informacoes;

    preencherInformacoesEntrada(entradaInformacoes, numeroPagina, numeroQuadro, tempo);
    tabela->entradas[numeroPagina].valido = true;
    simulador->simuladorTabelaDensa.quadrosLivres[numeroQuadro] = false;
    simulador->simuladorTabelaDensa.paginasPorQuadro[numeroQuadro] = numeroPagina;
}


void substituirEntradaTabelaDensa(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    int tempo = simulador->simuladorTabelaDensa.tempo;
    EntradaTabelaDensa *entrada = &tabela->entradas[numeroPagina];

    int posSubstituir = selecionaPaginaParaSubstituirTabelaDensa(simulador);
    if (posSubstituir == -1) {
        printf("Erro: Política de substituição inválida.\n");
        return;
    }

    EntradaTabelaDensa *entradaSubstituir = &tabela->entradas[posSubstituir];
    incrementarAcessosTabela(&simulador->simuladorTabelaDensa.estatisticas);

    if (entradaSubstituir->informacoes.bitModificacao) 
        incrementarPaginasSujasEscritas(&simulador->simuladorTabelaDensa.estatisticas);

    int numeroQuadro = entradaSubstituir->informacoes.numeroQuadro;
    entradaSubstituir->valido = false;
    entradaSubstituir->informacoes.numeroQuadro = -1;
    simulador->simuladorTabelaDensa.paginasPorQuadro[numeroQuadro] = numeroPagina;

    inicializarInformacoesEntrada(&entrada->informacoes);
    preencherInformacoesEntrada(&entrada->informacoes, numeroPagina, numeroQuadro, tempo);
    entrada->valido = true;
}


void acessarPaginaTabelaDensa(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    if (numeroPagina >= simulador->simuladorTabelaDensa.tabela.capacidade) {
        printf("Erro: Número de página inválido.\n");
        return;
    }

    TabelaDensa *tabela = &simulador->simuladorTabelaDensa.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaDensa.estatisticas;
    int *tempo = &simulador->simuladorTabelaDensa.tempo;
    (*tempo)++;
    
    // Verificar se já existe um quadro para a página
    EntradaTabelaDensa *entrada =  &tabela->entradas[numeroPagina];

    if (entrada->valido) {
        atualizarInformacoesEntrada(&entrada->informacoes, *tempo, tipoAcesso);
        incrementarAcessosTabela(estatisticas);
        return;
    }

    // Se não existe quadro para página
    if (simulador->simuladorTabelaDensa.numeroQuadroOcupados < simulador->numeroQuadros) {
        // Se existem quadro livres, aloque no primeiro para a página
        adicionarEntradaTabelaDensa(simulador, numeroPagina, tipoAcesso);  
        simulador->simuladorTabelaDensa.numeroQuadroOcupados++;

    } else {
        // Se não tem mais quadros livres, selecione uma página para pegar o quadro
        substituirEntradaTabelaDensa(simulador, numeroPagina, tipoAcesso);
    }

    if (tipoAcesso == 'W')  setarBitModificacao(&entrada->informacoes);
    incrementarAcessosTabela(estatisticas);
    incrementarPageFaults(estatisticas);
}
