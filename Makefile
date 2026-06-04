CC = gcc
CFLAGS = -Wall -Wextra

OBJS = obj/funcoesGerais.o obj/tabelaDensa.o obj/tabelaInvertida.o obj/simulador.o obj/main.o

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

run: simulador
	./simulador

clean:
	rm -f simulador obj/*.o