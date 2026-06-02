#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/simulador.h"

void incrementarAcessosTabela(EstatisticasTabela *estatisticas) {
    estatisticas->acessosTabela++;
}