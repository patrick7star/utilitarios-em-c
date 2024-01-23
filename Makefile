

# 'ponto' não está terminado!
#all: teste ponto aleatorio tempo
all: teste aleatorio tempo


CFLAGS=-DUT_TESTE
OBJS_TESTE=  legivel.o tempo.o terminal.o

teste: teste.c $(OBJS_TESTE)
	echo "lincando todos objetos gerados..."
	echo "compilando testes-unitários de teste.c ..."
	gcc $(CFLAGS) -o ut_teste teste.c $(OBJS_TESTE) -lm -Wall


FILES_PONTO_C = testes/unit_tests_ponto.c ponto.c
OBJS_PONTO = ponto.o terminal.o legivel.o tempo.o

ponto: $(FILES_PONTO_C) $(OBJS_PONTO)
	echo "lincando artefatos para 'ponto.c'..."
	gcc -o ut_ponto testes/unit_tests_ponto.c $(OBJS_PONTO) \
	-Wall -lm -Wno-main


aleatorio:
	gcc -o ut_aleatorio aleatorio.c teste.c $(OBJS_TESTE) \
	-D_UT_ALEATORIO -lm -Wall -Wno-return-type


TEMPO_VERBOSE = -D_DEBUG_ALTERA_STATUS \
	-D_DEBUG_CRIA_TEMPORIZADOR \
	-D_DEBUG_DESTROI_TEMPORIZADOR \
	-D_DEBUG_DESTROI_CRONOMETRO \
	-D_DEBUG_CRIA_CRONOMETRO 
TEMPO_DEPEDENCIAS = teste.c tempo.c legivel.c terminal.c

tempo: testes/unit_tests_tempo.c
	echo "lincando artefatos para 'unit_tests_tempo.c'..."
	gcc $(TEMPO_VERBOSE) -o ut_tempo \
	testes/unit_tests_tempo.c $(TEMPO_DEPEDENCIAS) \
	-I '.' -std=gnu18 -lm -Wall -Wno-main


clean:
	echo "executaveis aleatorios inicialmente."
	rm -fv string teste_exe ut_teste ut_ponto \
	ut_aleatorio
	echo "removendo todos objetos..."
	rm -fv *.o
