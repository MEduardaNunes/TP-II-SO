// Estrutura de tempo
int tempo = 0;
void incrementarTempo(int tempo) {}

// Informações gerais da entrada da tabela de páginas
struct InformacoesEntrada {
    int numeroPagina;
    int numeroQuadro;
    bool bitPresenca;
    bool bitModificacao;
    int ultimoAcesso;
    int quantidadeAcessos;
};

// Estrutura da Tabela de Página densa
struct EntradaTabelaDensa {
    struct InformacoesEntrada informacoes;
};

struct TabelaDensa {
    struct EntradaTabelaDensa* entradas;
    int quantidadeEntradasPreenchidas;
    int capacidade;
};

// Estrutura da Tabela de Páginas Invertida
struct EntradaTabelaInvertida {
    struct InformacoesEntrada informacoes;
    struct EntradaTabelaInvertida* proximo;
};

struct TabelaInvertida {
    struct EntradaTabelaInvertida* entradas;
    int capacidade;
}

// Estrutura da Tabela de Páginas Hierárquica de dois níveis
struct EntradaTabelaHierarquicaNivel1_2 {
    struct EntradaTabelaHierarquicaNivel2* entradas;
    int capacidadeEntradasNivel2;
};

struct EntradaTabelaHierarquicaNivel2_2 {
    struct InformacoesEntrada informacoes;
};

struct TabelaHierarquica_2 {
    struct EntradaTabelaHierarquicaNivel1 tabelaBase;
    int quantidadeEntradasPreenchidas;
    int capacidadeNivel1;
};

// Estrutura da Tabela de Páginas Hierárquica de três níveis
struct EntradaTabelaHierarquicaNivel1_3 {
    struct EntradaTabelaHierarquicaNivel2* entradas;
    int capacidadeEntradasNivel2;
};

struct EntradaTabelaHierarquicaNivel2_3 {
    struct EntradaTabelaHierarquicaNivel3* entradas;
    int capacidadeEntradasNivel3;
};

struct EntradaTabelaHierarquicaNivel3_3 {
    struct InformacoesEntrada informacoes;
};

struct TabelaHierarquica_3 {
    struct EntradaTabelaHierarquicaNivel1 tabelaBase;
    int quantidadeEntradasPreenchidas;
    int capacidadeNivel1;
};