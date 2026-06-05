#include "../header/tabelaHierarquica3.h"
#include "../header/simulador.h"
#include "../header/funcoesGerais.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void calcularIndicesHierarquicos3(EspecificacaoSimulador *simulador, int numeroPagina, int *p1, int *p2, int *p3) {
    unsigned int deslocamento = calcularDeslocamento(simulador->tamanhoPagina);
    unsigned int bitsPaginaLogica = 32 - deslocamento;
    unsigned int bitsNivel1 = bitsPaginaLogica / 3;
    unsigned int bitsNivel2 = bitsPaginaLogica / 3;
    unsigned int bitsNivel3 = bitsPaginaLogica - bitsNivel1 - bitsNivel2;

    *p1 = numeroPagina >> (bitsNivel2 + bitsNivel3);
    *p2 = (numeroPagina >> bitsNivel3) & ((1 << bitsNivel2) - 1);
    *p3 = numeroPagina & ((1 << bitsNivel3) - 1);
}

void inicializarTabelaHierarquica3(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    
    unsigned int deslocamento = calcularDeslocamento(simulador->tamanhoPagina);
    unsigned int bitsPaginaLogica = 32 - deslocamento;
    unsigned int bitsNivel1 = bitsPaginaLogica / 3;
    unsigned int bitsNivel2 = bitsPaginaLogica / 3;
    unsigned int bitsNivel3 = bitsPaginaLogica - bitsNivel1 - bitsNivel2;

    tabela->tamanhoNivel1 = 1 << bitsNivel1;
    tabela->tamanhoNivel2 = 1 << bitsNivel2;
    tabela->tamanhoNivel3 = 1 << bitsNivel3;

    tabela->tabelaNivel1 = (EntradaTabelaHierarquicaNivel1_3*) malloc(tabela->tamanhoNivel1 * sizeof(EntradaTabelaHierarquicaNivel1_3));
    
    if (tabela->tabelaNivel1 == NULL) {
        fprintf(stderr, "Erro ao alocar tabela nivel 1 hierarquica\n");
        exit(1);
    }

    for (int i = 0; i < tabela->tamanhoNivel1; i++) {
        tabela->tabelaNivel1[i].tabelaNivel2 = NULL;
        tabela->tabelaNivel1[i].alocada = false;
    }

    incrementarMemoriaConsumida(&simulador->simuladorTabelaHierarquica3.estatisticas, tabela->tamanhoNivel1 *sizeof(EntradaTabelaHierarquicaNivel1_3));
}

// alocação sob demanda
void alocarTabelaNivel2SeNecessario(TabelaHierarquica_3 *tabela, int indiceNivel1, EstatisticasTabela *estatisticas){
    if (tabela->tabelaNivel1[indiceNivel1].alocada)
        return;

    tabela->tabelaNivel1[indiceNivel1].tabelaNivel2 = malloc(tabela->tamanhoNivel2 * sizeof(EntradaTabelaHierarquicaNivel2_3));

    if (tabela->tabelaNivel1[indiceNivel1].tabelaNivel2 == NULL) {
        fprintf(stderr, "Erro ao alocar tabela nivel 2\n");
        exit(1);
    }

    for (int i = 0; i < tabela->tamanhoNivel2; i++) {
        tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[i].tabelaNivel3 = NULL;
        tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[i].alocada = false;
    }

    tabela->tabelaNivel1[indiceNivel1].alocada = true;
    estatisticas->memoriaConsumida += (tabela->tamanhoNivel2 * sizeof(EntradaTabelaHierarquicaNivel2_3));
}

// alocação sob demanda
void alocarTabelaNivel3SeNecessario(TabelaHierarquica_3 *tabela, int indiceNivel1, int indiceNivel2, EstatisticasTabela *estatisticas){
    if (tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[indiceNivel2].alocada)
        return;

    tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[indiceNivel2].tabelaNivel3 = malloc(tabela->tamanhoNivel3 * sizeof(EntradaTabelaHierarquicaNivel3_3));
    
    if (tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[indiceNivel2].tabelaNivel3 == NULL) {
        fprintf(stderr, "Erro ao alocar tabela nivel 3\n");
        exit(1);
    }

    for (int i = 0; i < tabela->tamanhoNivel3; i++) {
        inicializarInformacoesEntrada(&tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[indiceNivel2].tabelaNivel3[i].informacoes);
        tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[indiceNivel2].tabelaNivel3[i].valido = false;
    }

    tabela->tabelaNivel1[indiceNivel1].tabelaNivel2[indiceNivel2].alocada = true;
    estatisticas->memoriaConsumida += (tabela->tamanhoNivel3 * sizeof(EntradaTabelaHierarquicaNivel3_3));
}

void destruirTabelaHierarquica3(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;

    if (tabela->tabelaNivel1 != NULL) {
        for (int i = 0; i < tabela->tamanhoNivel1; i++) {
            if (tabela->tabelaNivel1[i].alocada && tabela->tabelaNivel1[i].tabelaNivel2 != NULL) {
                for (int j = 0; j < tabela->tamanhoNivel2; j++) {
                    if (tabela->tabelaNivel1[i].tabelaNivel2[j].alocada && tabela->tabelaNivel1[i].tabelaNivel2[j].tabelaNivel3 != NULL) {
                        free(tabela->tabelaNivel1[i].tabelaNivel2[j].tabelaNivel3);
                        tabela->tabelaNivel1[i].tabelaNivel2[j].tabelaNivel3 = NULL;
                        tabela->tabelaNivel1[i].tabelaNivel2[j].alocada = false;
                    }
                }

                free(tabela->tabelaNivel1[i].tabelaNivel2);
                tabela->tabelaNivel1[i].tabelaNivel2 = NULL;
                tabela->tabelaNivel1[i].alocada = false;
            }   
        }

        free(tabela->tabelaNivel1);
        tabela->tabelaNivel1 = NULL;
    }

    free(simulador->simuladorTabelaHierarquica3.quadrosLivres);
    simulador->simuladorTabelaHierarquica3.quadrosLivres = NULL;

    free(simulador->simuladorTabelaHierarquica3.paginasPorQuadro);
    simulador->simuladorTabelaHierarquica3.paginasPorQuadro = NULL;

    simulador->simuladorTabelaHierarquica3.numeroQuadroOcupados = 0;
    simulador->simuladorTabelaHierarquica3.tempo = 0;

    tabela->tamanhoNivel1 = 0;
    tabela->tamanhoNivel2 = 0;
    tabela->tamanhoNivel3 = 0;
}


int RANTabelaHierarquica_3(EspecificacaoSimulador *simulador) {
    int paginaVitima = -1;

    while (paginaVitima == -1) {
        int quadroRandom = random() % simulador->numeroQuadros;
        paginaVitima = simulador->simuladorTabelaHierarquica3.paginasPorQuadro[quadroRandom];
    }

    return paginaVitima;
}

int LRUTabelaHierarquica_3(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica3.estatisticas;

    int paginaMaisAntiga = -1;
    int minTempo = INT_MAX;

    for (int i = 0; i < simulador->numeroQuadros; i++) {
        int pagina = simulador->simuladorTabelaHierarquica3.paginasPorQuadro[i];
        incrementarAcessosTabela(estatisticas);

        if (pagina == -1) continue;

        int p1, p2, p3;
        calcularIndicesHierarquicos3(simulador, pagina, &p1, &p2, &p3);

        EntradaTabelaHierarquicaNivel3_3 *entrada = &tabela->tabelaNivel1[p1].tabelaNivel2[p2].tabelaNivel3[p3];
        
        if (entrada->valido && entrada->informacoes.ultimoAcesso < minTempo) {
            minTempo = entrada->informacoes.ultimoAcesso;
            paginaMaisAntiga = pagina; 
        }
    }

    return paginaMaisAntiga;
}

int MFUTabelaHierarquica_3(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica3.estatisticas;

}


int LFUTabelaHierarquica_3(EspecificacaoSimulador *simulador) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica3.estatisticas;
}


int selecionaPaginaParaSubstituirTabelaHierarquica_3(EspecificacaoSimulador *simulador) {
    char *politicaSubstituicao = simulador->politicaSubstituicao;

    if (strcmp(politicaSubstituicao, "RAN") == 0) {
        return RANTabelaHierarquica_3(simulador);

    } else if (strcmp(politicaSubstituicao, "LRU") == 0) {
        return LRUTabelaHierarquica_3(simulador);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaHierarquica_3(simulador);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaHierarquica_3(simulador);
    }

    return -1;
}


void adicionarEntradaTabelaHierarquica_3(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica3.estatisticas;

    int p1, p2, p3;
    calcularIndicesHierarquicos3(simulador, numeroPagina, &p1, &p2, &p3);

    alocarTabelaNivel2SeNecessario(tabela, p1, estatisticas);
    alocarTabelaNivel3SeNecessario(tabela, p1, p2, estatisticas);

    int numeroQuadro = achaPrimeiroQuadroLivre(simulador->simuladorTabelaHierarquica3.quadrosLivres, simulador->numeroQuadros);
    int tempo = simulador->simuladorTabelaHierarquica3.tempo;

    EntradaTabelaHierarquicaNivel3_3 *entradaFinal = &tabela->tabelaNivel1[p1].tabelaNivel2[p2].tabelaNivel3[p3];
    preencherInformacoesEntrada(&entradaFinal->informacoes, numeroPagina, numeroQuadro, tempo);
    entradaFinal->valido = true;

    simulador->simuladorTabelaHierarquica3.quadrosLivres[numeroQuadro] = false;
    simulador->simuladorTabelaHierarquica3.paginasPorQuadro[numeroQuadro] = numeroPagina;
    simulador->simuladorTabelaHierarquica3.numeroQuadroOcupados++;

    if (tipoAcesso == 'W') {
        setarBitModificacao(&entradaFinal->informacoes);
    }
}


void substituirEntradaTabelaHierarquica_3(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica3.estatisticas;
    int tempo = simulador->simuladorTabelaHierarquica3.tempo;

    int paginaVitima = selecionaPaginaParaSubstituirTabelaHierarquica_3(simulador);
    if (paginaVitima == -1) {
        printf("Erro: Política de substituição falhou ao encontrar página vítima.\n");
        return;
    }

    int p1Antigo, p2Antigo, p3Antigo;
    calcularIndicesHierarquicos3(simulador, paginaVitima, &p1Antigo, &p2Antigo, &p3Antigo);

    EntradaTabelaHierarquicaNivel3_3 *entradaAntiga = &tabela->tabelaNivel1[p1Antigo].tabelaNivel2[p2Antigo].tabelaNivel3[p3Antigo];

    if (entradaAntiga->informacoes.bitModificacao) {
        incrementarPaginasSujasEscritas(estatisticas);
    }

    int numeroQuadro = entradaAntiga->informacoes.numeroQuadro; 
    entradaAntiga->valido = false; 

    int p1Novo, p2Novo, p3Novo;
    calcularIndicesHierarquicos3(simulador, numeroPagina, &p1Novo, &p2Novo, &p3Novo);

    alocarTabelaNivel2SeNecessario(tabela, p1Novo, estatisticas);
    alocarTabelaNivel3SeNecessario(tabela, p1Novo, p2Novo, estatisticas);

    EntradaTabelaHierarquicaNivel3_3 *entradaNova = &tabela->tabelaNivel1[p1Novo].tabelaNivel2[p2Novo].tabelaNivel3[p3Novo];

    inicializarInformacoesEntrada(&entradaNova->informacoes); 
    preencherInformacoesEntrada(&entradaNova->informacoes, numeroPagina, numeroQuadro, tempo);
    entradaNova->valido = true;

    simulador->simuladorTabelaHierarquica3.paginasPorQuadro[numeroQuadro] = numeroPagina;

    if (tipoAcesso == 'W') {
        setarBitModificacao(&entradaNova->informacoes);
    }
}

void acessarPaginaTabelaHierarquica_3(EspecificacaoSimulador *simulador, int numeroPagina, char tipoAcesso) {
    TabelaHierarquica_3 *tabela = &simulador->simuladorTabelaHierarquica3.tabela;
    EstatisticasTabela *estatisticas = &simulador->simuladorTabelaHierarquica3.estatisticas;
    int *tempo = &simulador->simuladorTabelaHierarquica3.tempo;
    
    (*tempo)++;

    int p1, p2, p3;
    calcularIndicesHierarquicos3(simulador, numeroPagina, &p1, &p2, &p3);

    incrementarAcessosTabela(estatisticas);

    if (tabela->tabelaNivel1[p1].alocada) {
        incrementarAcessosTabela(estatisticas);
        EntradaTabelaHierarquicaNivel2_3 *nivel2 = tabela->tabelaNivel1[p1].tabelaNivel2;
        
        if (nivel2[p2].alocada) {
            EntradaTabelaHierarquicaNivel3_3 *nivel3 = nivel2[p2].tabelaNivel3;

            incrementarAcessosTabela(estatisticas);

            if (nivel3[p3].valido) {
                atualizarInformacoesEntrada(&nivel3[p3].informacoes, *tempo, tipoAcesso);
                return;
            }
        }
    }

    incrementarPageFaults(estatisticas);

    if (simulador->simuladorTabelaHierarquica3.numeroQuadroOcupados < simulador->numeroQuadros) {
        adicionarEntradaTabelaHierarquica_3(simulador, numeroPagina, tipoAcesso);  
    } else {
        substituirEntradaTabelaHierarquica_3(simulador, numeroPagina, tipoAcesso);
    }
}