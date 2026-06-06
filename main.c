#include "header/simulador.h"
#include "header/tabelaDensa.h"
#include "header/funcoesGerais.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

int main (int argc, char *argv[]) {
    // processamento inicial
    srandom(time(NULL));
    EspecificacaoSimulador simulador = {0};
    lerArgumentosTerminal(argc, argv, &simulador);
    int erro = rodarSimulador(&simulador);
    if (erro == 0) imprimirSimulador(&simulador);
    destruirSimulador(&simulador);

    return 0;
}