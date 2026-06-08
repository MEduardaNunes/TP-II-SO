CC = gcc
CFLAGS = -Wall -Wextra -g

OBJ_DIR = obj
RESULTADOS_DIR = resultados
LOG_DIR = testes
BIN_DIR = bin

EXEC = $(BIN_DIR)/simulador
OBJS = \
	$(OBJ_DIR)/funcoesGerais.o \
	$(OBJ_DIR)/tabelaDensa.o \
	$(OBJ_DIR)/tabelaInvertida.o \
	$(OBJ_DIR)/simulador.o \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/tabelaHierarquica2.o \
	$(OBJ_DIR)/tabelaHierarquica3.o

ALGS = RAN LRU LFU MFU
LOGS_BASE = compilador.log matriz.log compressor.log simulador.log
LOGS_EXTRA = sequencial.log aleatorio.log localidade.log
DEBUG_ALG ?= LRU
DEBUG_LOG ?= compilador.log
DEBUG_PAG ?= 4
DEBUG_MEM ?= 4096

.PHONY: all clean clean_results clean_all debug \
	test_all test_all_full \
	test_tabela1 test_tabela1_full \
	test_tabela2 test_tabela2_full \
	test_tabela3 test_tabela3_full
	
all: $(EXEC)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(RESULTADOS_DIR):
	mkdir -p $(RESULTADOS_DIR)

$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c main.c -o $@

$(OBJ_DIR)/funcoesGerais.o: src/funcoesGerais.c header/funcoesGerais.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/funcoesGerais.c -o $@

$(OBJ_DIR)/tabelaHierarquica2.o: src/tabelaHierarquica2.c header/tabelaHierarquica2.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/tabelaHierarquica2.c -o $@

$(OBJ_DIR)/tabelaHierarquica3.o: src/tabelaHierarquica3.c header/tabelaHierarquica3.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/tabelaHierarquica3.c -o $@

$(OBJ_DIR)/tabelaDensa.o: src/tabelaDensa.c header/tabelaDensa.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/tabelaDensa.c -o $@

$(OBJ_DIR)/tabelaInvertida.o: src/tabelaInvertida.c header/tabelaInvertida.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/tabelaInvertida.c -o $@

$(OBJ_DIR)/simulador.o: src/simulador.c header/simulador.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c src/simulador.c -o $@

debug: $(EXEC) | $(RESULTADOS_DIR)
	./$(EXEC) $(DEBUG_ALG) $(LOG_DIR)/$(DEBUG_LOG) $(DEBUG_PAG) $(DEBUG_MEM) debug
	
# Memória crescente e página fixa
test_tabela1: $(EXEC) | $(RESULTADOS_DIR)
	@echo ""
	@echo "--- Iniciando testes da Tabela 1 ---" | tee $(RESULTADOS_DIR)/teste_tabela1.txt
	@for alg in $(ALGS); do \
		for mem in 128 512 2048 8192 16384; do \
			for file in $(LOGS_BASE); do \
				echo "Executando: $$alg | $$file | 4 KB | Mem: $$mem KB" | tee -a $(RESULTADOS_DIR)/teste_tabela1.txt; \
				./$(EXEC) $$alg $(LOG_DIR)/$$file 4 $$mem | tee -a $(RESULTADOS_DIR)/teste_tabela1.txt; \
			done; \
		done; \
	done

# Todos os logs
test_tabela1_full: $(EXEC) | $(RESULTADOS_DIR)
	@echo ""
	@echo "--- Iniciando testes da Tabela 1 (FULL) ---" | tee $(RESULTADOS_DIR)/teste_tabela1_full.txt
	@for alg in $(ALGS); do \
		for mem in 128 512 2048 8192 16384; do \
			for file in $(LOGS_BASE) $(LOGS_EXTRA); do \
				echo "Executando: $$alg | $$file | 4 KB | Mem: $$mem KB" | tee -a $(RESULTADOS_DIR)/teste_tabela1_full.txt; \
				./$(EXEC) $$alg $(LOG_DIR)/$$file 4 $$mem | tee -a $(RESULTADOS_DIR)/teste_tabela1_full.txt; \
			done; \
		done; \
	done

# Página variável e memória constante 
test_tabela2: $(EXEC) | $(RESULTADOS_DIR)
	@echo ""
	@echo "--- Iniciando testes da Tabela 2 ---" | tee $(RESULTADOS_DIR)/teste_tabela2.txt
	@for alg in $(ALGS); do \
		for pag in 2 4 8 16 32 64; do \
			for file in $(LOGS_BASE); do \
				echo "Executando: $$alg | $$file | Pag: $$pag KB | Mem: 4096 KB" | tee -a $(RESULTADOS_DIR)/teste_tabela2.txt; \
				./$(EXEC) $$alg $(LOG_DIR)/$$file $$pag 4096 | tee -a $(RESULTADOS_DIR)/teste_tabela2.txt; \
			done; \
		done; \
	done

# Todos os logs
test_tabela2_full: $(EXEC) | $(RESULTADOS_DIR)
	@echo ""
	@echo "--- Iniciando testes da Tabela 2 (FULL) ---" | tee $(RESULTADOS_DIR)/teste_tabela2_full.txt
	@for alg in $(ALGS); do \
		for pag in 2 4 8 16 32 64; do \
			for file in $(LOGS_BASE) $(LOGS_EXTRA); do \
				echo "Executando: $$alg | $$file | Pag: $$pag KB | Mem: 4096 KB" | tee -a $(RESULTADOS_DIR)/teste_tabela2_full.txt; \
				./$(EXEC) $$alg $(LOG_DIR)/$$file $$pag 4096 | tee -a $(RESULTADOS_DIR)/teste_tabela2_full.txt; \
			done; \
		done; \
	done

# Avaliação das estruturas de tabela
test_tabela3: $(EXEC) | $(RESULTADOS_DIR)
	@echo ""
	@echo "--- Iniciando testes da Tabela 3 ---" | tee $(RESULTADOS_DIR)/teste_tabela3.txt
	@for alg in $(ALGS); do \
		for pag in 2 4 8 16 32 64; do \
			for file in $(LOGS_BASE); do \
				echo "Executando: $$alg | $$file | Pag: $$pag KB | Mem: 4096 KB" | tee -a $(RESULTADOS_DIR)/teste_tabela3.txt; \
				./$(EXEC) $$alg $(LOG_DIR)/$$file $$pag 4096 | tee -a $(RESULTADOS_DIR)/teste_tabela3.txt; \
			done; \
		done; \
	done

test_tabela3_full: $(EXEC) | $(RESULTADOS_DIR)
	@echo ""
	@echo "--- Iniciando testes da Tabela 3 (FULL) ---" | tee $(RESULTADOS_DIR)/teste_tabela3_full.txt
	@for alg in $(ALGS); do \
		for pag in 2 4 8 16 32 64; do \
			for file in $(LOGS_BASE) $(LOGS_EXTRA); do \
				echo "Executando: $$alg | $$file | Pag: $$pag KB | Mem: 4096 KB" | tee -a $(RESULTADOS_DIR)/teste_tabela3_full.txt; \
				./$(EXEC) $$alg $(LOG_DIR)/$$file $$pag 4096 | tee -a $(RESULTADOS_DIR)/teste_tabela3_full.txt; \
			done; \
		done; \
	done

test_all: test_tabela1 test_tabela2 test_tabela3

test_all_full: test_tabela1_full test_tabela2_full test_tabela3_full

clean:
	rm -f $(EXEC) $(EXEC).exe $(OBJ_DIR)/*.o

clean_results:
	rm -f $(RESULTADOS_DIR)/*.txt
	rm -f $(RESULTADOS_DIR)/*.csv

clean_all: clean clean_results