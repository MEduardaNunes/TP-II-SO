# Trabalho Prático #2 – Memória Virtual

## Compilação
Compile o projeto executando:

```bash
make
```

O executável `simulador` será gerado automaticamente na pasta bin/.

## Execução
O programa recebe quatro parâmetros obrigatórios e um parâmetro opcional:

```bash
./bin/simulador <algoritmo> <arquivo_log> <tam_pagina_kb> <memoria_kb> [debug]
```

Onde:
* `algoritmo`: RAN, LRU, LFU ou MFU;
* `arquivo_log`: arquivo de entrada localizado em `testes/`;
* `tam_pagina_kb`: tamanho da página em KB;
* `memoria_kb`: tamanho da memória física em KB;
* `debug`: parâmetro opcional que ativa o modo de depuração.

Exemplo:
```bash
./bin/simulador LRU testes/compilador.log 4 4096
```

## Modo Debug
Para executar o simulador em modo de depuração utilizando os parâmetros padrão (LRU, compilador.log, página de 4 KB e memória de 4096 KB):
```bash
make debug
```
Também é possível alterar os parâmetros:
```bash
make debug DEBUG_ALG=LFU DEBUG_LOG=aleatorio.log DEBUG_PAG=8 DEBUG_MEM=2048
```

## Execução dos Testes
Executar os experimentos de cada tabela:

```bash
make test_tabela1
make test_tabela2
make test_tabela3
```

Executar todas as tabelas:
```bash
make test_all
```

As versões completas, que incluem os logs adicionais (`sequencial.log`, `aleatorio.log` e `localidade.log`), podem ser executadas com:

```bash
make test_tabela1_full
make test_tabela2_full
make test_tabela3_full
```

ou
```bash
make test_all_full
```

Os resultados são armazenados automaticamente na pasta `resultados/`.

## Limpeza
Remover arquivos compilados:

```bash
make clean
```

Remover resultados dos experimentos:
```bash
make clean_results
```

Remover arquivos compilados e resultados:
```bash
make clean_all
```
