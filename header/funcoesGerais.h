#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include <stdbool.h>

// Informações gerais da entrada da tabela de páginas
typedef struct {
    int numeroPagina;
    int numeroQuadro;
    bool bitPresenca;
    bool bitModificacao;
    int ultimoAcesso;
    int quantidadeAcessos;
} InformacoesEntrada;

void inicializarInformacoesEntrada(InformacoesEntrada *entrada);
void preencherInformacoesEntrada(InformacoesEntrada *entrada, int numeroPagina, int numeroQuadro, int ultimoAcesso);
void setarBitModificacao(InformacoesEntrada *entrada); 
void atualizarInformacoesEntrada(InformacoesEntrada *entrada, int ultimoAcesso, char tipoAcesso);
int achaPrimeiroQuadroLivre(bool *quadrosLivres, int numeroQuadros);


// Estrutura estatísticas da tabela de páginas
typedef struct {
    long numeroReferenciasMemoria;
    long numeroPageFaults;
    long numeroPaginasSujasEscritas;
    long acessosTabela;
    long memoriaConsumida;
} EstatisticasTabela;

void inicializarEstatisticasTabela(EstatisticasTabela *estatisticas);
void incrementarAcessosTabela(EstatisticasTabela *estatisticas);
void incrementarReferenciasMemoria(EstatisticasTabela *estatisticas);
void incrementarPageFaults(EstatisticasTabela *estatisticas);
void incrementarPaginasSujasEscritas(EstatisticasTabela *estatisticas);
void incrementarMemoriaConsumida(EstatisticasTabela *estatisticas, long quantidade);

#endif
