

# 'ponto' não está terminado!
#all: teste ponto aleatorio tempo
all: teste aleatorio tempo barra-de-progresso

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

legivel.o: src/legivel.c
	gcc -c src/legivel.c -o build/legivel.o

terminal.o: src/terminal.c
	gcc -c src/terminal.c -o build/terminal.o

tempo.o: src/tempo.c
	gcc -c -I include/ src/tempo.c -o build/tempo.o

teste.o: src/teste.c
	gcc -I include/ -c src/teste.c -o build/teste.o

FLAGS_TESTE = -I include/ -DUT_TESTE
OBJS_TESTE=  build/legivel.o build/tempo.o build/terminal.o
EXE_TESTE = testes/ut_teste

teste: src/teste.c $(OBJS_TESTE)
	@echo "lincando todos objetos gerados..."
	@echo "compilando testes-unitários de teste.c ..."
	gcc $(FLAGS_TESTE) -o testes/ut_teste src/teste.c $(OBJS_TESTE) -lm -Wall

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

FILES_PONTO_C = testes/unit_tests_ponto.c ponto.c
OBJS_PONTO = ponto.o terminal.o legivel.o tempo.o

ponto: $(FILES_PONTO_C) $(OBJS_PONTO)
	@echo "lincando artefatos para 'ponto.c'..."
	gcc -o ut_ponto testes/unit_tests_ponto.c $(OBJS_PONTO) \
	-Wall -lm -Wno-main

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

aleatorio.o: src/aleatorio.c
	gcc -c src/aleatorio.c -o build/aleatorio.o -lm


OBJETOS = build/teste.o build/teste.o build/tempo.o build/legivel.o 
OBJS = build/teste.o build/tempo.o build/legivel.o build/terminal.o
EXE = testes/ut_aleatorio
SOURCE = src/aleatorio.c
FLAGS = -I include/ -D_UT_ALEATORIO -Wall -Wno-return-type

aleatorio: legivel.o terminal.o tempo.o teste.o
	gcc $(FLAGS) -o $(EXE) $(SOURCE) -lm $(OBJS) 

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

TEMPO_VERBOSE = -D_DEBUG_ALTERA_STATUS \
	-D_DEBUG_CRIA_TEMPORIZADOR \
	-D_DEBUG_DESTROI_TEMPORIZADOR \
	-D_DEBUG_DESTROI_CRONOMETRO \
	-D_DEBUG_CRIA_CRONOMETRO 
TEMPO_DEPEDENCIAS = src/teste.c src/tempo.c src/legivel.c src/terminal.c

tempo: 
	@echo "lincando artefatos para 'unit_tests_tempo.c'..."
	gcc $(TEMPO_VERBOSE) -o testes/ut_tempo \
	testes/unit_tests_tempo.c $(TEMPO_DEPEDENCIAS) \
	-I 'include/' -std=gnu18 -lm -Wall -Wno-main -Wno-unused-functions

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

# limpando todos executáveis no diretório.
clean:
	rm -fv build/*.o testes/ut*

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

barra-de-progresso:
	@echo "criando objetos(artefatos) do 'barra de progresso'..."
	gcc -c src/barra_de_progresso.c -D_UT_BARRA_DE_PROGRESSO -o build/progresso.o
	@echo "lincando todos objetos num executável..."
	gcc -o testes/ut_barra_de_progresso build/progresso.o -Wall -lm

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

# salva mais um backup deste projeto. Entretanto, antes de executar tal,
# mude a atual versão para não reescreve o último, pois é isso que vai 
# acontecer.
VERSAO = v1.1.0
OPCOES = --exclude=ut* -cvf
NOME = utilitarios.$(VERSAO).tar 
LOCAL = ../versions
CONTEUDO = include/ src/ testes/ Makefile pratica.c

salva:
	tar $(OPCOES) $(LOCAL)/$(NOME) $(CONTEUDO)

# visualizar todos backups deste projeto.
backups:
	ls --sort=time -1 $(LOCAL)/utilitarios*.tar

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
barra-de-progresso.o: src/barra_de_progresso.c
	gcc -c src/barra_de_progresso.c -o build/barra_de_progresso.o

EXE_PL = testes/ut_pilha_ligada
MOSTRA_PL = -D_DESTROI_PL
FLAGS_PL = -Wall
ABS_DIR_I = src/estrutura-de-dados/abordagem-i

pilha-ligada-i: barra-de-progresso.o
	gcc $(MOSTRA_PL) -I include/ -D_UT_PILHA_LIGADA -c \
		$(ABS_DIR_I)/pilhaligada.c \
		-o build/pilhaligada.o
	gcc -Og -I include/ -o $(EXE_PL) \
		build/pilhaligada.o build/barra_de_progresso.o -Wall

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
FLAGS_HT_I = -D_UT_HASHTABLE \
					-D_INSERCAO_HT \
					-D_CRIACAO_HT \
					-D_DESTRUICAO_HT \
					-D_CONTIDO_HT \
					-D_ATUALIZA_HT \
					-D_DELETA_HT \
					-D_OBTEM_HT 
EXE_HT_I = testes/ut_hashtable_i
OBJS_HT_I = $(OBJS_TESTE) build/teste.o
SRC_HT_I = $(ABS_DIR_I)/hashtable.c

hashtable-i: teste.o tempo.o legivel.o terminal.o
	gcc -I include/ $(FLAGS_HT_I) -c $(SRC_HT_I) -o build/hashtable-i.o
	gcc -o $(EXE_HT_I) build/hashtable-i.o $(OBJS_HT_I) -lm -Wall

