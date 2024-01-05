

all: ut_teste

CFLAGS=-DUT_TESTE
OBJS_TESTE=  legivel.o tempo.o terminal.o

teste: teste.c $(OBJS_TESTE)
	echo "lincando todos objetos gerados..."
	gcc $(CFLAGS) -o ut_teste teste.c $(OBJS_TESTE) -lm -Wall


FILES_PONTO_C = testes/unit_tests_ponto.c ponto.c
OBJS_PONTO = ponto.o terminal.o legivel.o tempo.o

ponto: $(FILES_PONTO_C) $(OBJS_PONTO)
	echo "lincando artefatos para 'teste.c'..."
	gcc -o ut_ponto testes/unit_tests_ponto.c $(OBJS_PONTO) 
	\ -Wall -lm

aleatorio: 
	gcc -o ut_aleatorio aleatorio.c -DUT_ALEATORIO -lm -Wall \
	-Wno-return-type

clean:
	echo "executaveis aleatorios inicialmente."
	rm -fv string teste_exe ut_teste ut_ponto \
	ut_aleatorio
	echo "removendo todos objetos..."
	rm -fv *.o
