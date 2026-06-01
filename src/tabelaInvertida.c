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
    tabela->quantidadeEntradasPreenchidas = 0;
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