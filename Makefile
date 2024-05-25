

# 'ponto' não está terminado!
#all: teste ponto aleatorio tempo
all: teste aleatorio tempo barra-de-progresso estringue
.PHONY = estringue

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

legivel.o: src/legivel.c
	gcc -c src/legivel.c -o build/legivel.o

terminal.o: src/terminal.c
	gcc -c src/terminal.c -o build/terminal.o

tempo.o: src/tempo.c
	gcc -c -I include/ src/tempo.c -o build/tempo.o

teste.o: src/teste.c
	gcc -I include/ -c src/teste.c -o build/teste.o

FLAGS_TESTE = -I include/ -D_UT_TESTE -DALLOW_DEAD_CODE
OBJS_TESTE=  build/legivel.o build/tempo.o build/terminal.o
EXE_TESTE = testes/ut_teste

teste: $(OBJS_TESTE) src/teste.c
	@echo "lincando todos objetos gerados..."
	@echo "compilando testes-unitários de teste.c ..."
	gcc $(FLAGS_TESTE) -o testes/ut_teste src/teste.c $(OBJS_TESTE) -lm -Wall

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---


OBJS_PONTO = build/terminal.o build/legivel.o build/tempo.o build/teste.o
EXE_PONTO = bin/tests/ut_ponto

ponto: teste.o
	@echo "lincando artefatos para 'ponto.c'..."
	gcc -D_UT_PONTO -I include -o $(EXE_PONTO) src/ponto.c $(OBJS_PONTO) \
		-Wall -lm -Wno-main

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

aleatorio.o: src/aleatorio.c
	gcc -c src/aleatorio.c -o build/aleatorio.o -lm

OBJETOS = build/teste.o build/teste.o build/tempo.o build/legivel.o 
OBJS = build/teste.o build/tempo.o build/legivel.o build/terminal.o
EXE = bin/tests/ut_aleatorio
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
	rm -rv bin/

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
VERSAO = v1.1.3
OPCOES = --exclude=ut* -cvf
NOME = utilitarios.$(VERSAO).tar 
LOCAL = ../versions
CONTEUDO = include/ src/ testes/*.c Makefile

salva:
	tar $(OPCOES) $(LOCAL)/$(NOME) $(CONTEUDO)

# visualizar todos backups deste projeto.
backups:
	ls --sort=time -1 $(LOCAL)/utilitarios*.tar

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
barra-de-progresso.o: src/barra_de_progresso.c
	gcc -c src/barra_de_progresso.c -o build/barra_de_progresso.o

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
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
COMPILA_HT_I = -D_UT_HASHTABLE -D_INSERCAO_HT -D_CRIACAO_HT \
				-D_DESTRUICAO_HT -D_CONTIDO_HT \
				-D_ATUALIZA_HT -D_DELETA_HT -D_OBTEM_HT 
EXE_HT_I = bin/tests/ut_hashtable_ref
OBJS_HT_I = $(OBJS_TESTE) build/teste.o
SRC_HT_I = src/estrutura-de-dados/hashtable.c

hashtable-ref: teste.o tempo.o legivel.o terminal.o
	gcc -I include/ $(COMPILA_HT_I) -c $(SRC_HT_I) -o build/hashtable-ref.o
	gcc -o $(EXE_HT_I) build/hashtable-ref.o $(OBJS_HT_I) -lm -Wall -std=c18

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
SRC_FC_I = src/estrutura-de-dados/abordagem-i/filacircular.c
FLAGS_FC_I = -lm -Wall -Wno-main -std=c2x
COMPILA_FC_I = -DUT_FILA_CIRCULAR
EXE_FC_I = bin/tests/ut_filacircular
DEPS_FC_I = build/teste.o $(OBJS_TESTE)

fila-circular-i:
	gcc -I include/ $(COMPILA_FC_I) -o $(EXE_FC_I) \
		$(SRC_FC_I) $(DEPS_FC_I) $(FLAGS_FC_I)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
SRC_ADT_I = src/estrutura-de-dados/abordagem-i
EXE_FA_I = bin/tests/ut_fila_array
COMPILA_FA_I = -D_UT_FILA_ARRAY \
					-DALOCACAO_E_DEALOCACAO \
					-D_REDIMENSIONAMENTO
DEPS_FA_I = $(OBJS_HT_I) build/aleatorio.o

fila-array-i: aleatorio.o
	@echo "compilando 'testes unitários' da 'fila-array'..."
	gcc -I include $(COMPILA_FA_I) -o $(EXE_FA_I) $(SRC_ADT_I)/filaarray.c $(DEPS_FA_I) -lm -Wall

COMPILA_LA_I = -D_ALOCACAO_E_DESALOCACAO -D_UT_ARRAY_LISTA
SRC_LA_I = src/estrutura-de-dados/abordagem-i/listaarray.c
DEPS_LA_I = build/terminal.o build/tempo.o -L bin/static -lteste -llegivel -lm
EXE_LA_I = bin/tests/ut_lista_array
lista-array-i:
	gcc $(COMPILA_LA_I) -o $(EXE_LA_I) -I include/ $(SRC_LA_I) $(DEPS_LA_I) -Wall
# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
COMPILAR_STR = -D_PALAVRAS -D_UT_STRING -D_CONCATENA_STRINGS
EXE_STR = testes/estringue
LIB_STR = ../utilitarios/include

estringue: 
	gcc -I $(LIB_STR) $(COMPILAR_STR) -o $(EXE_STR) src/estringue.c $(OBJS_TESTE) build/teste.o -lm -Wall

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
compila-libs: libprogresso liblegivel libteste libtempo

cria_diretorio_bin:
	mkdir --parents --verbose bin/shared
	mkdir --parents --verbose bin/static

EXE_LIBPROGRESSO_I = ./bin/shared/libprogresso.so
EXE_LIBPROGRESSO_II = ./bin/static/libprogresso.a
SRC_LIBPROGR = src/barra_de_progresso.c
OBJ_LIBPROGR = build/barra_de_progresso.o

libprogresso: cria_diretorio_bin 
	@echo "compilação de uma biblioteca compartilhada."
	clang -o $(EXE_LIBPROGRESSO_I) -fPIC -shared $(SRC_LIBPROGR) -lc
	@echo "compilação de uma biblioteca estática."
	ar crs $(EXE_LIBPROGRESSO_II) $(OBJ_LIBPROGR)

EXE_LEGIVEL_I = bin/shared/liblegivel.so
EXE_LEGIVEL_II = bin/static/liblegivel.a
SRC_LEGIVEL = src/legivel.c
OBJ_LEGIVEL = build/legivel.o

liblegivel:
	@echo "compilação de uma biblioteca compartilhada."
	gcc -o $(EXE_LEGIVEL_I) -fPIC -shared $(SRC_LEGIVEL) -lc
	@echo "compilação de uma biblioteca estática."
	ar crs $(EXE_LEGIVEL_II) $(OBJ_LEGIVEL)

EXE_TST_I = bin/shared/libteste.so
EXE_TST_II = bin/static/libteste.a
SRC_TST = src/teste.c

libteste: teste.o cria_diretorio_bin
	@echo "compilação de uma biblioteca compartilhada."
	gcc $(FLAGS_TESTE) -fPIC -shared -o $(EXE_TST_I) \
		$(SRC_TST) $(OBJS_TESTE) -lm -Wall
	@echo "compilação de uma biblioteca estática."
	ar crs $(EXE_TST_II) build/teste.o

EXE_TIME_I = bin/shared/libtempo.so
SRC_TIME = build/tempo.o build/legivel.o
EXE_TIME_II = bin/static/libtempo.a

libtempo: legivel.o tempo.o
	@echo "compilação de uma biblioteca compartilhada."
	gcc -I include/ -fPIC -shared -o $(EXE_TIME_I) $(SRC_TIME) -Wall
	@echo "compilação de uma biblioteca estática."
	ar crs $(EXE_TIME_II) build/tempo.o
# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

