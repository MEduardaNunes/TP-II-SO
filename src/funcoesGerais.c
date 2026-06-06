#include "../header/funcoesGerais.h"

// Funções para manipulação Entrada
void inicializarInformacoesEntrada(InformacoesEntrada *entrada) {
    entrada->numeroPagina = -1;
    entrada->numeroQuadro = -1;
    entrada->bitModificacao = false;
    entrada->ultimoAcesso = 0;
    entrada->quantidadeAcessos = 0;
}

void preencherInformacoesEntrada(InformacoesEntrada *entrada, unsigned int numeroPagina, int numeroQuadro, int ultimoAcesso) {
    entrada->numeroPagina = numeroPagina;
    entrada->numeroQuadro = numeroQuadro;
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

int achaPrimeiroQuadroLivre(bool *quadrosLivres, int numeroQuadros) {
    for (int i = 0; i < numeroQuadros; i++) {
        if (quadrosLivres[i]) {
            return i;
        }
    }
    return -1; // Retorna -1 se não houver quadros livres
}

// Funções das estatísticas

void inicializarEstatisticasTabela(EstatisticasTabela *estatisticas) {
    estatisticas->numeroReferenciasMemoria = 0;
    estatisticas->numeroPageFaults = 0;
    estatisticas->numeroPaginasSujasEscritas = 0;
    estatisticas->acessosTabela = 0;
    estatisticas->memoriaConsumida = 0;
}


void incrementarAcessosTabela(EstatisticasTabela *estatisticas) {
    estatisticas->acessosTabela++;
}

void incrementarReferenciasMemoria(EstatisticasTabela *estatisticas) {
    estatisticas->numeroReferenciasMemoria++;
}

void incrementarPageFaults(EstatisticasTabela *estatisticas) {
    estatisticas->numeroPageFaults++;
}

void incrementarPaginasSujasEscritas(EstatisticasTabela *estatisticas) {
    estatisticas->numeroPaginasSujasEscritas++;
}

void incrementarMemoriaConsumida(EstatisticasTabela *estatisticas, long quantidade) {
    estatisticas->memoriaConsumida += quantidade;
}

unsigned int calcularDeslocamento(unsigned int tamanhoPagina) {
    unsigned int deslocamento = 0;
    unsigned int temp = tamanhoPagina * 1024;
    while (temp > 1) {
        deslocamento++;
        temp >>= 1; 
    }
    return deslocamento;
}

unsigned int extrairNumeroPagina(unsigned int endereco, unsigned int deslocamento) {
    return endereco >> deslocamento;
}