run:
	gcc -o simulador simulador.c -lm
	./simulador

clean:
	rm -f simulador