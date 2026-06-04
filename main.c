#include "header/simulador.h"
#include "header/tabelaDensa.h"
#include "header/funcoesGerais.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

int main (int argc, char *argv[]) {
    // processamento inicial
    EspecificacaoSimulador simulador = {0};
    lerArgumentosTerminal(argc, argv, &simulador);
    rodarSimulador(&simulador);
    imprimirSimulador(&simulador);
    destruirSimulador(&simulador);

    return 0;
}