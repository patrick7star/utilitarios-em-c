

# 'ponto' não está terminado!
.PHONY = estringue

# Compila todos objetos no diretório de construção ...
compila-objetos-principais: \
	cria_diretorio_build \
	legivel.o \
	terminal.o \
	tempo.o \
	teste.o \
	barra-de-progresso.o \
	aleatorio.o \
	ponto.o

compila-bibliotecas-estaticas: \
	libprogresso \
	liblegivel \
	libtempo \
	libterminal \
	libaleatorio \
	libteste

compila-testes-unitarios: \
	cria-diretorio-bin \
	terminal \
	ponto \
	aleatorio \
	tempo \
	impressao \
	legivel

compila-testes-unitarios-colecoes: \
	conjunto-ref \
	hashtable-ref \
	pilha-ligada-ref \
	lista-posicional-ref \
	lista-array-ref

compila-testes-integrais: \
	fatorizacao-com-hashtable \
	testando-todos-objetos-gerados \
	distribuicao-de-numeros-gerados-randomicamente

# Compila tudo acima, seguindo sua hieraquia copiada(cima para baixo):
compila-tudo: \
	compila-objetos-principais \
	compila-bibliotecas-estaticas \
	compila-testes-unitarios \
	compila-testes-unitarios-colecoes \
	compila-testes-integrais
	@echo "Todas códigos das bibliotecas(testes e libs) foram compilados com sucesso."

# limpando todos executáveis no diretório.
clean:
	rm -fv build/*.o testes/ut*
	rm -frv bin/
	rmdir build/

# Salva mais um backup deste projeto. Entretanto, antes de executar tal,
# mude a atual versão para não reescreve o último, pois é isso que vai 
# acontecer.
VERSAO = v1.1.6
OPCOES = --exclude=ut* -cvf
NOME = utilitarios.$(VERSAO).tar 
LOCAL = ../versions
CONTEUDO = include/ src/ tests/*.c Makefile

salva:
	tar $(OPCOES) $(LOCAL)/$(NOME) $(CONTEUDO)

# visualizar todos backups deste projeto.
backups:
	ls --sort=time -1 $(LOCAL)/utilitarios*.tar

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
cria_diretorio_build:
	mkdir --verbose build/

legivel.o: src/legivel.c
	gcc -c src/legivel.c -o build/legivel.o

terminal.o: src/terminal.c
	gcc -I include -c src/terminal.c -o build/terminal.o

tempo.o: src/tempo.c
	gcc -c -I include/ src/tempo.c -o build/tempo.o

teste.o: src/teste.c
	gcc -I include/ -c src/teste.c -o build/teste.o

barra-de-progresso.o: src/barra_de_progresso.c
	gcc -I include/ -c src/barra_de_progresso.c -o build/barra_de_progresso.o

ponto.o:
	clang -I include/ -Wall -c -o build/ponto.o src/ponto.c

# === === ===  === === === === === === === === === === === === === === ===
#
#						Compilação dos Testes Unitários dos
#							Módulos Principais da Biblioteca
#
# === === ===  === === === === === === === === === === === === === === ===
cria-diretorio-bin:
	mkdir --parents --verbose bin/tests

FLAGS_TESTE = -Wno-unused-variable -Wno-unused-function
COMPILA_TESTE = -D_UT_TESTE -DALLOW_DEAD_CODE \
				  -D_NOVO_SUITE_COMPLETO -D_NOVO_SUITE
OBJS_TESTE=  build/legivel.o build/tempo.o build/terminal.o build/barra_de_progresso.o
EXE_TESTE = bin/tests/ut_teste

teste: $(OBJS_TESTE) src/teste.c
	@echo "lincando todos objetos gerados..."
	@echo "compilando testes-unitários de teste.c ..."
	gcc -I include/ $(COMPILA_TESTE) -Wall -o $(EXE_TESTE) src/teste.c $(OBJS_TESTE) -lm

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
OBJ_TERM = build/terminal_teste.o
EXE_TERM = ./bin/tests/ut_terminal

terminal:
	@echo "Compilando artefato 'terminal.o' em 'build' ..."
	clang -std=gnu18 -Wno-main-return-type -I ./include \
		-D_UT_TERMINAL -Wall \
		-c -o $(OBJ_TERM) src/terminal.c
	@echo "Ligamento entre o artefato e o executável ..."
	clang -O0 -o $(EXE_TERM) $(OBJ_TERM)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
OBJS_PONTO = build/terminal.o build/legivel.o build/tempo.o build/teste.o
EXE_PONTO = bin/tests/ut_ponto

ponto: teste.o
	@echo "lincando artefatos para 'ponto.c'..."
	gcc -D_UT_PONTO -I include/ -o $(EXE_PONTO) src/ponto.c $(OBJS_PONTO) \
		-Wall -lm -Wno-main

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
aleatorio.o: src/aleatorio.c
	gcc -I include/ -c src/aleatorio.c -o build/aleatorio.o -lm

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
# TEMPO_DEPEDENCIAS = src/teste.c src/tempo.c src/legivel.c src/terminal.c
TEMPO_COMPILA = -D_UT_TEMPO -D_DEBUG_CRIA_CRONOMETRO \
					 -D_DEBUG_DESTROI_CRONOMETRO
TEMPO_FLAGS = -I include -std=gnu18 -lm -Wall
TEMPO_DEPEDENCIAS = -L bin/static -llegivel

tempo: 
	@echo "lincando artefatos para 'unit_tests_tempo.c'..."
	gcc $(TEMPO_FLAGS) $(TEMPO_COMPILA) -o bin/tests/ut_tempo \
		src/tempo.c $(TEMPO_DEPEDENCIAS)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
COMPILAR_STR = -D_PALAVRAS -D_UT_STRING -D_CONCATENA_STRINGS
EXE_STR = bin/tests/ut_estringue
DEPS_STR = $(OBJS_TESTE) build/teste.o

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
estringue: terminal.o
	gcc -Wall -I ./include $(COMPILAR_STR) -o $(EXE_STR) src/estringue.c $(DEPS_STR) -lm

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
barra-de-progresso:
	@echo "criando objetos(artefatos) do 'barra de progresso'..."
	gcc -c src/barra_de_progresso.c -D_UT_BARRA_DE_PROGRESSO -o build/progresso.o
	@echo "lincando todos objetos num executável..."
	gcc -o bin/tests/ut_barra_de_progresso build/progresso.o -Wall -lm

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
impressao: terminal.o lista-array-ref.o
	clang -I include/ -D_UT_IMPRESSAO -o bin/tests/ut_impressao src/impressao.c build/terminal.o build/listaarray_ref.o

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
legivel: teste.o
	clang -O0 -std=c2x -I include/ -Wall \
		-D_UT_LEGIVEL \
		-o bin/tests/ut_legivel src/legivel.c -lm \
		-L bin/static -lteste -ltempo -lterminal

progresso: teste.o
	clang -O0 -std=gnu2x -I include/ -Wall \
		-D_UT_PROGRESSO \
		-o bin/tests/ut_progresso src/progresso.c -lm \
		-L bin/static -lteste -llegivel -ltempo -lterminal
# === === ===  === === === === === === === === === === === === === === ===
#
# 					Compilação e Coleção de Estruturas de Dados
# 					 			e seus Testes Unitários
#
# === === ===  === === === === === === === === === === === === === === ===
OBJS_SET_REF = -L bin/static -lteste -llegivel -ltempo -lterminal
COMPILA_SET_REF = -D_MEDIA_DOS_SLOTS -D_UT_CONJUNTO 
EXE_SET_REF = bin/tests/ut_conjunto
SRC_SET_REF = src/estrutura-de-dados/conjunto_ref.c

conjunto-ref: libaleatorio
	clang -O0 -std=c18 -Wall $(COMPILA_SET_REF) -o $(EXE_SET_REF) -I include $(SRC_SET_REF) $(OBJS_SET_REF) -lm

EXE_PL = bin/tests/ut_pilha_ligada
MOSTRA_PL = -D_UT_PILHA_LIGADA -D_DESTROI_PL
DEPS_PL = build/barra_de_progresso.o -L bin/static -lteste -ltempo -llegivel -lterminal

pilha-ligada-ref: barra-de-progresso.o
	clang $(MOSTRA_PL) -I include/ -O0 -Wall -Wall \
		-o $(EXE_PL) src/estrutura-de-dados/pilhaligada_ref.c -lm $(DEPS_PL)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
COMPILA_HT_I = -D_UT_HASHTABLE -D_INSERCAO_HT -D_CRIACAO_HT \
				-D_DESTRUICAO_HT -D_CONTIDO_HT \
				-D_ATUALIZA_HT -D_DELETA_HT -D_OBTEM_HT 
EXE_HT_I = bin/tests/ut_hashtable_ref
OBJS_HT_I = $(OBJS_TESTE) build/teste.o
SRC_HT_I = src/estrutura-de-dados/hashtable_ref.c

hashtable-ref: teste.o tempo.o legivel.o terminal.o
	gcc -I include/ $(COMPILA_HT_I) -c $(SRC_HT_I) -o build/hashtable-ref.o
	gcc -o $(EXE_HT_I) build/hashtable-ref.o $(OBJS_HT_I) -lm -Wall -std=c18

hashtable-ref-cheque: teste.o tempo.o legivel.o terminal.o
	gcc -fanalyzer -Wall -std=c18 build/hashtable-ref.o $(OBJS_HT_I) -lm

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

COMPILA_LA_I = -D_ALOCACAO_E_DESALOCACAO -D_UT_ARRAY_LISTA -D_TO_STRING
SRC_LA_I = src/estrutura-de-dados/listaarray_ref.c
DEPS_LA_I = build/terminal.o build/tempo.o -L bin/static -lteste -llegivel -lm
EXE_LA_I = bin/tests/ut_lista_array_ref
lista-array-ref: libteste
	gcc $(COMPILA_LA_I) -o $(EXE_LA_I) -I include/ $(SRC_LA_I) $(DEPS_LA_I) -Wall

SRC_LP_REF = src/estrutura-de-dados/listaposicional_ref.c
EXE_LP_REF = bin/tests/ut_listaposicional
DEPS_LP_REF = -L bin/static -lteste -ltempo -llegivel -lm -lterminal

lista-posicional-ref:
	clang -I include -D_UT_LISTA_POSICIONAL -Wall -O0 \
		-Wno-incompatible-pointer-types-discards-qualifiers \
		-Wno-incompatible-pointer-types \
		-o $(EXE_LP_REF) $(SRC_LP_REF) $(DEPS_LP_REF)
# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

lista-array-ref.o:
	clang -I include/ -Wall -std=c2x -c -o build/listaarray_ref.o src/estrutura-de-dados/listaarray_ref.c 


# === === ===  === === === === === === === === === === === === === === ===
#
# 					 Compilação de bibliotecas estáticas
#
# === === ===  === === === === === === === === === === === === === === ===
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
OBJS_LIB_TST = build/tempo.o build/legivel.o build/terminal.o

libteste: teste.o cria_diretorio_bin
	@echo "compilação de uma biblioteca estática."
	ar crs $(EXE_TST_II) build/teste.o
	@echo "compilação de uma biblioteca compartilhada."
	gcc -I include/ -fPIC -shared -o $(EXE_TST_I) \
		$(SRC_TST) $(OBJS_LIB_TST) -lm -Wall

EXE_TIME_I = bin/shared/libtempo.so
SRC_TIME = build/tempo.o build/legivel.o
EXE_TIME_II = bin/static/libtempo.a

libtempo: legivel.o tempo.o
	@echo "compilação de uma biblioteca compartilhada."
	gcc -I include/ -fPIC -shared -o $(EXE_TIME_I) $(SRC_TIME) -Wall
	@echo "compilação de uma biblioteca estática."
	ar crs $(EXE_TIME_II) build/tempo.o

libterminal: terminal.o
	@echo "compilação de uma biblioteca estática."
	ar crs bin/static/libterminal.a build/terminal.o

libaleatorio: aleatorio.o
	@echo "compila gerador/embalharador."
	ar crs bin/static/libaleatorio.a build/terminal.o

# === === ===  === === === === === === === === === === === === === === ===
#
#						Compilação dos Testes Integrais
#
# === === ===  === === === === === === === === === === === === === === ===

fatorizacao-com-hashtable:
	clang -I include/ -Wall tests/fatorizacao_com_hashtable.c src/estrutura-de-dados/hashtable_ref.c -o bin/tests/it_fatorizacao_com_hashtable

testando-todos-objetos-gerados:
	clang -I include/ -Wall -o bin/tests/it_testando_todos_objetos_gerados tests/testando_todos_objetos_gerados.c build/tempo.o build/barra_de_progresso.o build/aleatorio.o build/legivel.o build/ponto.o -lm

hashtable-ref.o:
	@clang -std=c2x -I include/ -Wall -c -o \
		build/hashtable_ref.o \
		src/estrutura-de-dados/hashtable_ref.c

distribuicao-de-numeros-gerados-randomicamente: hashtable-ref.o
	clang -std=c2x -I include/ -Wall \
		-D_DESTRUICAO_HT \
		-o bin/tests/it_distruibacao_de_numeros_gerados_randomicamente \
		tests/distribuicao_de_numeros_gerados_randomicamente.c \
		build/barra_de_progresso.o build/aleatorio.o -lm \
		build/hashtable_ref.o
# === === ===  === === === === === === === === === === === === === === ===
