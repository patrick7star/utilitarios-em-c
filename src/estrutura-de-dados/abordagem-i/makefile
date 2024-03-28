
# compilação dos testes unitários:

deque-ligada:
	gcc -I .. -D_UT_DEQUE_LIGADA -o ut_dequeligada dequeligada.c \
	../aleatorio.c ../teste.c ../terminal.c ../legivel.c ../tempo.c \
	-Wall -lm -Wno-unused-function


array-lista:
	gcc -I .. -D_UT_ARRAY_LISTA -o ut_arraylista arraylista.c \
	../aleatorio.c ../teste.c ../terminal.c ../legivel.c ../tempo.c \
	-Wall -lm -Wno-unused-function


.PHONY= hashtable pilha-ligada


FLAGS_HT = -D_UT_HASHTABLE -D_INSERCAO_HT -D_CRIACAO_HT -D_DESTRUICAO_HT \
	-D_CONTIDO_HT -D_ATUALIZA_HT -D_DELETA_HT  -D_OBTEM_HT 
EXE_HT = ut_hashtable
DEPS_HT = ../aleatorio.c ../teste.c ../terminal.c ../legivel.c ../tempo.c

hashtable:
	gcc -Og $(FLAGS_HT) hashtable.c -o $(EXE_HT) -I .. $(DEPS_HT) \
	-Wall -lm -Wno-unused-function -Wno-unused-variable -Wno-main

DESATIVA_HT = -U_INSERCAO_HT -U_CRIACAO_HT -U_DESTRUICAO_HT \
	-U_ATUALIZA_HT -U_DELETA_HT -U_OBTEM_HT 
EXE_HT_I = ut_hashtable_print_off

hashtable-debug-off:
	gcc -Og $(FLAGS_HT) hashtable.c -o $(EXE_HT_I) -I .. $(DEPS_HT) \
	$(DESATIVA_HT) -Wall -lm -Wno-unused-function -Wno-unused-variable \
	-Wno-main

EXE_PL = ut_pilha_ligada
OBJS_PL = pilhaligada.o tempo.o legivel.o tempo.o terminal.o teste.o
MOSTRA_PL = -D_DESTROI_PL

pilha-ligada:
	gcc -c ../barra_de_progresso.c
	gcc $(MOSTRA_PL) -I .. -D_UT_PILHA_LIGADA -c pilhaligada.c 
	gcc -Og -I .. -o $(EXE_PL) pilhaligada.o barra_de_progresso.o -Wall -lm

