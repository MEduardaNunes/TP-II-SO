#include "../header/funcoesGerais.h"

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