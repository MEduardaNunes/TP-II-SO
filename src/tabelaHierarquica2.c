#include "../header/tabelaHierarquica2.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void calcularIndicesHierarquicos(EspecificacaoSimulador *simulador, int numeroPagina, int *p1, int *p2) {
    unsigned int deslocamento = calcularDeslocamento(simulador->tamanhoPagina);
    unsigned int bitsPaginaLogica = 32 - deslocamento;
    unsigned int bitsNivel1 = bitsPaginaLogica / 2;
    unsigned int bitsNivel2 = bitsPaginaLogica - bitsNivel1;

    *p1 = numeroPagina >> bitsNivel2;
    *p2 = numeroPagina & ((1 << bitsNivel2) - 1);
}

void inicializarTabelaHierarquica2(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    
    unsigned int deslocamento = calcularDeslocamento(simulador->tamanhoPagina);
    unsigned int bitsPaginaLogica = 32 - deslocamento;
    unsigned int bitsNivel1 = bitsPaginaLogica / 2;
    unsigned int bitsNivel2 = bitsPaginaLogica - bitsNivel1;

    tabela->tamanhoTabelaExterna = 1 << bitsNivel1;
    tabela->tamanhoTabelaInterna = 1 << bitsNivel2;

    tabela->tabelaExterna = (EntradaTabelaHierarquicaNivel1_2*) malloc(tabela->tamanhoTabelaExterna * sizeof(EntradaTabelaHierarquicaNivel1_2));
    
    if (tabela->tabelaExterna == NULL) {
        fprintf(stderr, "Erro ao alocar tabela externa hierarquica\n");
        exit(1);
    }

    for (int i = 0; i < tabela->tamanhoTabelaExterna; i++) {
        tabela->tabelaExterna[i].tabelaInterna = NULL;
        tabela->tabelaExterna[i].alocada = false;
    }
}

// alocação sob demanda
void alocarTabelaInternaSeNecessario(TabelaHierarquica_2 *tabela, int indiceNivel1, EstatisticasTabela *estatisticas){
    if (tabela->tabelaExterna[indiceNivel1].alocada)
        return;

    tabela->tabelaExterna[indiceNivel1].tabelaInterna =
        malloc(tabela->tamanhoTabelaInterna * sizeof(EntradaTabelaHierarquicaNivel2_2));

    if (tabela->tabelaExterna[indiceNivel1].tabelaInterna == NULL){
        fprintf(stderr, "Erro ao alocar tabela interna\n");
        exit(1);
    }

    for (int i = 0; i < tabela->tamanhoTabelaInterna; i++) {
        inicializarInformacoesEntrada(&tabela->tabelaExterna[indiceNivel1].tabelaInterna[i].informacoes
        );

        tabela->tabelaExterna[indiceNivel1].tabelaInterna[i].valido = false;
    }

    tabela->tabelaExterna[indiceNivel1].alocada = true;
    estatisticas->memoriaConsumida += (tabela->tamanhoTabelaInterna * sizeof(EntradaTabelaHierarquicaNivel2_2));
}

void destruirTabelaHierarquica2(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;

    if (tabela->tabelaExterna != NULL) {
        for (int i = 0; i < tabela->tamanhoTabelaExterna; i++) {
            if (tabela->tabelaExterna[i].alocada && tabela->tabelaExterna[i].tabelaInterna != NULL) {
                free(tabela->tabelaExterna[i].tabelaInterna);
                tabela->tabelaExterna[i].tabelaInterna = NULL;
                tabela->tabelaExterna[i].alocada = false;
            }
        }
        free(tabela->tabelaExterna);
        tabela->tabelaExterna = NULL;
    }

    free(simulador->simuladorTabelaHierarquica2.quadrosLivres);
    simulador->simuladorTabelaHierarquica2.quadrosLivres = NULL;

    free(simulador->simuladorTabelaHierarquica2.paginasPorQuadro);
    simulador->simuladorTabelaHierarquica2.paginasPorQuadro = NULL;

    simulador->simuladorTabelaHierarquica2.numeroQuadroOcupados = 0;
    simulador->simuladorTabelaHierarquica2.tempo = 0;

    tabela->tamanhoTabelaExterna = 0;
    tabela->tamanhoTabelaInterna = 0;
}


int RANTabelaHierarquica_2(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    
}

int LRUTabelaHierarquica_2(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica2.estatisticas;
}

int MFUTabelaHierarquica_2(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica2.estatisticas;

}


int LFUTabelaHierarquica_2(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica2.estatisticas;

    int paginaMenosFrequente = -1;
    int minAcessos = __INT_MAX__;

    for (int i = 0; i < simulador->numeroQuadros; i++) {
        int pagina = simulador->simuladorTabelaHierarquica2.paginasPorQuadro[i];
        incrementarAcessosTabela(estatisticas);

        if (pagina == -1) continue;

        int p1, p2;
        calcularIndicesHierarquicos(simulador, pagina, &p1, &p2);

        EntradaTabelaHierarquicaNivel2_2 *entrada_i = &tabela->tabelaExterna[p1].tabelaInterna[p2];
        
        if (entrada_i->valido && entrada_i->informacoes.quantidadeAcessos < minAcessos) {
            minAcessos = entrada_i->informacoes.quantidadeAcessos;
            paginaMenosFrequente = pagina;
        }
    }

    return paginaMenosFrequente;
}


int selecionaPaginaParaSubstituirTabelaHierarquica_2(EspecificacaoSimulador *simulador) {
    char *politicaSubstituicao = simulador->politicaSubstituicao;

    if (strcmp(politicaSubstituicao, "RAN") == 0) {
        return RANTabelaHierarquica_2(simulador);

    } else if (strcmp(politicaSubstituicao, "LRU") == 0) {
        return LRUTabelaHierarquica_2(simulador);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaHierarquica_2(simulador);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaHierarquica_2(simulador);
    }

    return -1;
}


void adicionarEntradaTabelaHierarquica_2(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica2.estatisticas;

    int p1, p2;
    calcularIndicesHierarquicos(simulador, numeroPagina, &p1, &p2);

    alocarTabelaInternaSeNecessario(tabela, p1, estatisticas);

    int numeroQuadro = achaPrimeiroQuadroLivre(simulador->simuladorTabelaHierarquica2.quadrosLivres, simulador->numeroQuadros);
    int tempo = simulador->simuladorTabelaHierarquica2.tempo;

    EntradaTabelaHierarquicaNivel2_2 *entradaFinal = &tabela->tabelaExterna[p1].tabelaInterna[p2];
    preencherInformacoesEntrada(&entradaFinal->informacoes, numeroPagina, numeroQuadro, tempo);
    entradaFinal->valido = true;

    simulador->simuladorTabelaHierarquica2.quadrosLivres[numeroQuadro] = false;
    simulador->simuladorTabelaHierarquica2.paginasPorQuadro[numeroQuadro] = numeroPagina;
    simulador->simuladorTabelaHierarquica2.numeroQuadroOcupados++;

    if (tipoAcesso == 'W') {
        setarBitModificacao(&entradaFinal->informacoes);
    }
}


void substituirEntradaTabelaHierarquica_2(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica2.estatisticas;
    int tempo = simulador->simuladorTabelaHierarquica2.tempo;

    int paginaVitima = selecionaPaginaParaSubstituirTabelaHierarquica_2(simulador);
    if (paginaVitima == -1) {
        printf("Erro: Política de substituição falhou ao encontrar página vítima.\n");
        return;
    }

    int p1Antigo, p2Antigo;
    calcularIndicesHierarquicos(simulador, paginaVitima, &p1Antigo, &p2Antigo);

    EntradaTabelaHierarquicaNivel2_2 *entradaAntiga = &tabela->tabelaExterna[p1Antigo].tabelaInterna[p2Antigo];

    if (entradaAntiga->informacoes.bitModificacao) {
        incrementarPaginasSujasEscritas(estatisticas);
    }

    int numeroQuadro = entradaAntiga->informacoes.numeroQuadro; 
    entradaAntiga->valido = false; 

    int p1Novo, p2Novo;
    calcularIndicesHierarquicos(simulador, numeroPagina, &p1Novo, &p2Novo);

    alocarTabelaInternaSeNecessario(tabela, p1Novo, estatisticas);

    EntradaTabelaHierarquicaNivel2_2 *entradaNova = &tabela->tabelaExterna[p1Novo].tabelaInterna[p2Novo];

    inicializarInformacoesEntrada(&entradaNova->informacoes); 
    preencherInformacoesEntrada(&entradaNova->informacoes, numeroPagina, numeroQuadro, tempo);
    entradaNova->valido = true;

    simulador->simuladorTabelaHierarquica2.paginasPorQuadro[numeroQuadro] = numeroPagina;

    if (tipoAcesso == 'W') {
        setarBitModificacao(&entradaNova->informacoes);
    }
}

void acessarPaginaTabelaHierarquica_2(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaHierarquica_2 *tabela = &simulador->simuladorTabelaHierarquica2.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica2.estatisticas;
    int *tempo = &simulador->simuladorTabelaHierarquica2.tempo;
    (*tempo)++;
    
    incrementarAcessosTabela(estatisticas);

    int p1, p2;
    calcularIndicesHierarquicos(simulador, numeroPagina, &p1, &p2);

    if (tabela->tabelaExterna[p1].alocada && tabela->tabelaExterna[p1].tabelaInterna[p2].valido) {
        atualizarInformacoesEntrada(&tabela->tabelaExterna[p1].tabelaInterna[p2].informacoes, *tempo, tipoAcesso);
        return;
    }

    incrementarPageFaults(estatisticas);

    if (simulador->simuladorTabelaHierarquica2.numeroQuadroOcupados < simulador->numeroQuadros) {
        adicionarEntradaTabelaHierarquica_2(simulador, numeroPagina, tipoAcesso);  
    } else {
        substituirEntradaTabelaHierarquica_2(simulador, numeroPagina, tipoAcesso);
    }
}