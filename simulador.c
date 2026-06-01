#include "simulador.h"
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


// Função para simulador
int achaPrimeiroQuadroLivre(bool *quadrosLivres, int numeroQuadros) {
    for (int i = 0; i < numeroQuadros; i++) {
        if (quadrosLivres[i]) {
            return i;
        }
    }
    return -1; // Retorna -1 se não houver quadros livres
}


// Funções para manipulação Entrada
void inicializarInformacoesEntrada(InformacoesEntrada *entrada) {
    entrada->numeroPagina = -1;
    entrada->numeroQuadro = -1;
    entrada->bitPresenca = false;
    entrada->bitModificacao = false;
    entrada->ultimoAcesso = 0;
    entrada->quantidadeAcessos = 0;
}

void preencherInformacoesEntrada(InformacoesEntrada *entrada, int numeroPagina, int numeroQuadro, int ultimoAcesso) {
    entrada->numeroPagina = numeroPagina;
    entrada->numeroQuadro = numeroQuadro;
    entrada->bitPresenca = true;
    entrada->ultimoAcesso = ultimoAcesso;
    entrada->quantidadeAcessos++;
}

void setarBitModificacao(InformacoesEntrada *entrada) {
    entrada->bitModificacao = true;
}

void atualizarInformacoesEntrada(InformacoesEntrada *entrada, int ultimoAcesso, char tipoAcesso) {
    entrada->ultimoAcesso = ultimoAcesso;
    entrada->quantidadeAcessos++;
    if (tipoAcesso == 'W') {
        setarBitModificacao(entrada);
    }
}


// Funções para manipulação Tabela Densa
void inicializarTabelaDensa(TabelaDensa *tabela, int capacidade) {
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
}

void destruirTabelaDensa(TabelaDensa *tabela) {
    free(tabela->entradas);
    tabela->entradas = NULL;
}

// int RANTabelaDensa(TabelaDensa *tabela)
//     // Implementação da política RANDOMICO

// int LRUTabelaDensa(TabelaDensa *tabela)
//     // Implementação da política LRU

int MFUTabelaDensa(TabelaDensa *tabela) {
    int paginaMaisFrequente = -1;
    int maxAcessos = -1;

    for (int i = 0; i < tabela->quantidadeEntradasPreenchidas; i++) {
        estatisticasTabelaDensa.acessosTabela++;
        
        if (tabela->entradas[i].informacoes.quantidadeAcessos > maxAcessos) {
            maxAcessos = tabela->entradas[i].informacoes.quantidadeAcessos;
            paginaMaisFrequente = i;
        }
    }

    return paginaMaisFrequente;
}

int LFUTabelaDensa(TabelaDensa *tabela) {
    int paginaMenosFrequente = -1;
    int minAcessos = __INT_MAX__;

    for (int i = 0; i < tabela->quantidadeEntradasPreenchidas; i++) {
        estatisticasTabelaDensa.acessosTabela++;

        if (tabela->entradas[i].informacoes.quantidadeAcessos < minAcessos) {
            minAcessos = tabela->entradas[i].informacoes.quantidadeAcessos;
            paginaMenosFrequente = i;
        }
    }

    return paginaMenosFrequente;
}

int selecionaPaginaParaSubstituirTabelaDensa(TabelaDensa *tabela, char *politicaSubstituicao) {
    if (strcmp(politicaSubstituicao, "RAN") == 0) {
        // return RANTabelaDensa(tabela);

    } else if (strcmp(politicaSubstituicao, "LRU") == 0) {
        // return LRUTabelaDensa(tabela);

    } else if (strcmp(politicaSubstituicao, "MFU") == 0) {
        return MFUTabelaDensa(tabela);

    } else if (strcmp(politicaSubstituicao, "LFU") == 0) {
        return LFUTabelaDensa(tabela);
    }

    return -1;
}

void acessarPaginaTabelaDensa(TabelaDensa *tabela, int numeroPagina, char tipoAcesso) {
    // Verificar se já existe uma entrada para a página
    for (int i = 0; i < tabela->quantidadeEntradasPreenchidas; i++) {
        tempo++;
        estatisticasTabelaDensa.acessosTabela++;

        if (tabela->entradas[i].informacoes.numeroPagina == numeroPagina) {
            // Página já está presente, atualizar as informações
            atualizarInformacoesEntrada(&tabela->entradas[i].informacoes, tempo, tipoAcesso);
            tempo++;
            return;
        }
    }

    // Se não existe uma entrada para a página, adicionar uma nova entrada
    if (tabela->quantidadeEntradasPreenchidas < tabela->capacidade) {
        // Se tem espaço na tabela, adicionar a nova entrada
        int pos = tabela->quantidadeEntradasPreenchidas;
        int numeroQuadro = achaPrimeiroQuadroLivre(simulador.quadrosLivres, simulador.numeroQuadros);
        simulador.quadrosLivres[numeroQuadro] = false;

        preencherInformacoesEntrada(&tabela->entradas[pos].informacoes, numeroPagina, numeroQuadro, tempo);
        tabela->quantidadeEntradasPreenchidas++;
        if (tipoAcesso == 'W') {
            setarBitModificacao(&tabela->entradas[pos].informacoes);
        }

        tempo++;
        estatisticasTabelaDensa.acessosTabela++;
        simulador.estatisticasSimulador.numeroPageFaults++;

    } else {
        // Se a tabela está cheia, selecionar uma página para substituir
        int paginaParaSubstituir = selecionaPaginaParaSubstituirTabelaDensa(tabela, simulador.politicaSubstituicao);

        if (paginaParaSubstituir != -1) {
            // Substituir a página selecionada
            if (tabela->entradas[paginaParaSubstituir].informacoes.bitModificacao) {
                simulador.estatisticasSimulador.numeroPaginasSujasEscritas++;
            }

            int quadroSubstituido = tabela->entradas[paginaParaSubstituir].informacoes.numeroQuadro;
            simulador.quadrosLivres[quadroSubstituido] = false;
            inicializarInformacoesEntrada(&tabela->entradas[paginaParaSubstituir].informacoes);
            preencherInformacoesEntrada(&tabela->entradas[paginaParaSubstituir].informacoes, numeroPagina, quadroSubstituido, tempo);

            if (tipoAcesso == 'W') {
                setarBitModificacao(&tabela->entradas[paginaParaSubstituir].informacoes);
            }

            tempo++;
            estatisticasTabelaDensa.acessosTabela++;
            simulador.estatisticasSimulador.numeroPageFaults++;

        } else {
            printf("Erro: Não foi possível substituir uma página.\n");
        }
    }
}

int main () {
}