obj/funcoesGerais.o: header/funcoesGerais.h src/funcoesGerais.c
	gcc -c src/funcoesGerais.c -o obj/funcoesGerais.o

obj/tabelaDensa.o: header/tabelaDensa.h src/tabelaDensa.c
	gcc -c src/tabelaDensa.c -o obj/tabelaDensa.o

obj/tabelaInvertida.o: header/tabelaInvertida.h src/tabelaInvertida.c
	gcc -c src/tabelaInvertida.c -o obj/tabelaInvertida.o

obj/simulador.o: header/simulador.h src/simulador.c
	gcc -c src/simulador.c -o obj/simulador.o

run: obj/funcoesGerais.o obj/tabelaDensa.o obj/tabelaInvertida.o obj/simulador.o main.c
	gcc -o simulador obj/funcoesGerais.o obj/tabelaDensa.o obj/tabelaInvertida.o obj/simulador.o main.c
	./simulador

clean:
	rm -f simulador obj/*.o