CC = gcc
CFLAGS = -Wall -Wextra

OBJS = obj/funcoesGerais.o obj/tabelaDensa.o obj/tabelaInvertida.o obj/simulador.o obj/main.o

LOG_DIR = testes

all: simulador

simulador: $(OBJS)
	$(CC) $(CFLAGS) -o simulador $(OBJS)

obj/main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o obj/main.o

obj/funcoesGerais.o: header/funcoesGerais.h src/funcoesGerais.c
	$(CC) $(CFLAGS) -c src/funcoesGerais.c -o obj/funcoesGerais.o

obj/tabelaDensa.o: header/tabelaDensa.h src/tabelaDensa.c
	$(CC) $(CFLAGS) -c src/tabelaDensa.c -o obj/tabelaDensa.o

obj/tabelaInvertida.o: header/tabelaInvertida.h src/tabelaInvertida.c
	$(CC) $(CFLAGS) -c src/tabelaInvertida.c -o obj/tabelaInvertida.o

obj/simulador.o: header/simulador.h src/simulador.c
	$(CC) $(CFLAGS) -c src/simulador.c -o obj/simulador.o

# 1: Memória crescente e página fixa (4 KB)
test_tabela1: simulador
	@echo ""
	@echo "--- Iniciando testes da Tabela 1 ---"
	@echo ""
	@for alg in RAND LRU LFU MFU; do \
		for mem in 128 512 2048 8192 16384; do \
			for file in compilador.log matriz.log compressor.log simulador.log; do \
				echo "Executando: $$alg | $$file | 4 KB | Mem: $$mem KB"; \
				./simulador $$alg $(LOG_DIR)/$$file 4 $$mem; \
			done \
		done \
	done

# 2: Página variável e memória constante (Definido como 4096 KB de exemplo)
test_tabela2: simulador
	@echo ""
	@echo "--- Iniciando testes da Tabela 2 ---"
	@echo ""
	@for alg in RAND LRU LFU MFU; do \
		for pag in 2 4 8 16 32 64; do \
			for file in compilador.log matriz.log compressor.log simulador.log; do \
				echo "Executando: $$alg | $$file | Pag: $$pag KB | Mem: 4096 KB"; \
				./simulador $$alg $(LOG_DIR)/$$file $$pag 4096; \
			done \
		done \
	done
# 3: Avaliação das estruturas de tabela (Variação de página)
test_tabela3: simulador
	@echo ""
	@echo "--- Iniciando testes da Tabela 3 ---"
	@echo ""
	@for alg in RAND LRU LFU MFU; do \
		for pag in 2 4 8 16 32 64; do \
			for file in compilador.log matriz.log compressor.log simulador.log; do \
				echo "Executando: $$alg | $$file | Pag: $$pag KB | Mem: 4096 KB"; \
				./simulador $$alg $(LOG_DIR)/$$file $$pag 4096; \
			done \
		done \
	done

# Executa todos os testes sequencialmente
test_all: test_tabela1 test_tabela2 test_tabela3

clean:
	rm -f simulador obj/*.o