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

int achaPrimeiroQuadroLivre(bool *quadrosLivres, int numeroQuadros);
void inicializarInformacoesEntrada(InformacoesEntrada *entrada);
void preencherInformacoesEntrada(InformacoesEntrada *entrada, int numeroPagina, int numeroQuadro, int ultimoAcesso);
void setarBitModificacao(InformacoesEntrada *entrada); 
void atualizarInformacoesEntrada(InformacoesEntrada *entrada, int ultimoAcesso, char tipoAcesso);

#endif