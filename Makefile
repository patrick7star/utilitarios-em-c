

# 'ponto' não está terminado!
.PHONY = estringue

# Cria estrutura onde artefatos compilados, de todos tipos, serão
# depositados:
cria-raiz-de-artefatos:
	mkdir --parents --verbose bin/shared
	mkdir --parents --verbose bin/static
	mkdir --parents --verbose bin/tests
	mkdir --parents --verbose build

# Compila tudo acima, seguindo sua hieraquia copiada(cima para baixo):
compila-tudo: \
	cria-raiz-de-artefatos \
	compila-objetos-principais \
	compila-bibliotecas \
	compila-bibliotecas-colecoes \
	compila-testes-unitarios \
	compila-testes-unitarios-colecoes \
	compila-testes-integrais
	@echo "Todas códigos das bibliotecas(testes e libs) foram compilados com sucesso."

# roda todos testes unitários da biblioteca:
roda-todos-testes-unitarios:
	@echo "Rodandos todos testes-unitários ..."
	@$(EXE_SET_REF)
	@$(EXE_FA_REF)
	bin/tests/ut_conjunto_ref_i
	bin/tests/ut_hashtable_ref
	bin/tests/ut_lista_array_ref
	bin/tests/ut_pilha_ligada

# limpando todos executáveis no diretório.
limpa-build:
	rm -v build/*
limpa-testes-integrais:
	rm -v bin/tests/*
limpa-bibliotecas:
	rm -v bin/static/*
	rm -v bin/shared/*
clean:
	rm -fv build/*.o testes/ut*
	rm -frv bin/
	rmdir build/


# Salva mais um backup deste projeto. Entretanto, antes de executar tal,
# mude a atual versão para não reescreve o último, pois é isso que vai 
# acontecer.
VERSAO = v1.2.1
OPCOES = --exclude=ut* -cvf
NOME = utilitarios.$(VERSAO).tar 
LOCAL = ../versions
CONTEUDO = include/ src/ tests/*.c Makefile

salva:
	tar $(OPCOES) $(LOCAL)/$(NOME) $(CONTEUDO)

# visualizar todos backups deste projeto.
lista-backups:
	@echo "\nTodos backups feitos:\n"
	@ls -hs --sort=time -1 $(LOCAL)/utilitarios*.tar

# Listagem de todos testes por tamanho:
lista-todos-testes:
	@echo "\nTodos testes já compilados:\n"
	@du -hs bin/tests/
	@du -b bin/tests/*


# === === ===  === === === === === === === === === === === === === === ===
#
#							Compilação de todos Objetos
#							 ou bibliotecas Estáticas
#
# === === ===  === === === === === === === === === === === === === === ===

compila-objetos-principais: \
	legivel.o \
	terminal.o \
	tempo.o \
	teste.o \
	aleatorio.o \
	ponto.o \
	progresso.o

limpa-objetos-principais:
	rm -v build/*

legivel.o:
	@gcc -O3 -Os -c src/legivel.c -o build/legivel.o
	@echo "Gerou o arquivo objeto 'legivel.o' em 'build'."

terminal.o:
	@gcc -O3 -Os -I include -c src/terminal.c -o build/terminal.o
	@echo "Gerou o arquivo objeto 'terminal.o' em 'build'."

tempo.o:
	@gcc -O3 -Os -c -I include/ src/tempo.c -o build/tempo.o
	@echo "Gerou o arquivo objeto 'tempo.o' em 'build'."

teste.o:
	@gcc -O3 -Os -I include/ -c src/teste.c -o build/teste.o
	@echo "Gerou o arquivo objeto 'teste.o' em 'build'."

ponto.o:
	@clang -O3 -Os -I include/ -Wall -c -o build/ponto.o src/ponto.c
	@echo "Gerou o arquivo objeto 'ponto.o' em 'build'."

progresso.o:
	@clang -O3 -Os -I include/ -Wall -c -o build/progresso.o src/progresso.c
	@echo "Gerou o arquivo objeto 'progresso.o' em 'build'."

aleatorio.o:
	@gcc -O3 -Os -c -I include/ -Wall -Werror -o build/aleatorio.o src/aleatorio.c
	@echo "Gerou o arquivo objeto 'aleatorio.o' em 'build'."

conversao.o:
	@clang -O3 -Os -I include/ -c -o build/conversao.o src/conversao.c
	@echo "Gerou o arquivo objeto 'conversao.o' em 'build'."

# ~~~ ~~~ ~~~  ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~
compila-objetos-colecoes: \
	cria-raiz-de-artefatos \
	hashtable-ref.o \
	conjunto-ref.o \
	pilha-ligada-ref.o \
	lista-array-ref.o \
	deque-ligada-ref.o \
	fila-ligada-ref.o \
	fila-array-ref.o

hashtable-ref.o:
	@clang -O3 -Oz -std=c2x -I include/ -Wall -c -o \
		build/hashtable_ref.o \
		src/estrutura-de-dados/hashtable_ref.c
	@echo "Gerou o arquivo objeto 'hashtable_ref.o', em 'build'."

conjunto-ref.o:
	@clang -O3 -Oz -I include/ -Wall -c \
		src/estrutura-de-dados/conjunto_ref.c \
		-o build/conjunto_ref.o
	@echo "Gerou o arquivo objeto 'conjunto_ref.o', em 'build'."

pilha-ligada-ref.o:
	@clang -O3 -Oz -std=c2x -I include/ -Wall -c -o \
		build/pilhaligada_ref.o \
		src/estrutura-de-dados/pilhaligada_ref.c
	@echo "Gerou o arquivo objeto 'pilhaligada_ref.o', em 'build'."

lista-array-ref.o:
	@clang -O3 -Oz -I include/ -Wall -c \
		src/estrutura-de-dados/listaarray_ref.c \
		-o build/listaarray_ref.o
	@echo "Gerou o arquivo objeto 'listaarray_ref.o', em 'build'."

deque-ligada-ref.o:
	@clang -O3 -Oz -I include/ -Wall -c \
		-o build/dequeligada_ref.o \
		src/estrutura-de-dados/dequeligada_ref.c
	@echo "Gerou o arquivo objeto 'dequeligada_ref.o', em 'build'."

fila-array-ref.o:
	@clang -O3 -Oz -I include/ -Wall -c \
		-o build/filaarray_ref.o \
		src/estrutura-de-dados/filaarray_ref.c
	@echo "Gerou o arquivo objeto 'filaarray_ref.o', em 'build'."

fila-ligada-ref.o:
	@clang -O3 -Oz -I include/ -Wall -c \
		-o build/filaligada_ref.o \
		src/estrutura-de-dados/filaligada_ref.c
	@echo "Gerou o arquivo objeto 'filaligada_ref.o', em 'build'."

fila-circular-ref.o:
	@clang -O3 -Oz -I include/ -Wall -Werror -c \
		-o build/filacircular_ref.o \
		src/estrutura-de-dados/filacircular_ref.c 
	@echo "Gerou o arquivo objeto 'filacircular_ref.o', em 'build'."

# === === ===  === === === === === === === === === === === === === === ===
#
#						Compilação dos Testes Unitários dos
#							Módulos Principais da Biblioteca
#
# === === ===  === === === === === === === === === === === === === === ===
compila-testes-unitarios: \
	cria-raiz-de-artefatos \
	terminal \
	ponto \
	aleatorio \
	tempo \
	impressao \
	legivel \
	conversao

roda-testes-unitarios:
	@$(EXE_TESTE)
	@$(EXE_TERM)
	@$(EXE_PONTO)
	@$(EXE_RANDOM)
	@$(EXE_TEMPO)
	@$(EXE_STR)

FLAGS_TESTE = -Wno-unused-variable -Wno-unused-function
COMPILA_TESTE = -D_UT_TESTE -D__debug__
DEPS_TESTE = -Lbin/static/ -ltempo -lterminal -llegivel -lprogresso
EXE_TESTE = bin/tests/ut_teste

teste: $(OBJS_TESTE) src/teste.c
	@gcc -I include/ -Wall $(COMPILA_TESTE) \
		-o $(EXE_TESTE) src/teste.c src/teste/amostras.c $(DEPS_TESTE) -lm
	@echo "Compilado os testes-unitários de 'teste.c' em bin/tests."

teste-check:
	@echo "checando a sintaxe do 'teste.c' ..."
	gcc -I include/ $(COMPILA_TESTE) -Wall src/teste.c $(OBJS_TESTE) -lm

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
OBJ_TERM = build/terminal_teste.o
EXE_TERM = ./bin/tests/ut_terminal

terminal:
	@echo "Compilando artefato 'terminal_teste.o' em 'build' ..."
	@clang -std=gnu18 -Wall -I ./include -c \
		-D_UT_TERMINAL \
		-Wno-main-return-type \
		-o $(OBJ_TERM) src/terminal.c
	@echo "Ligamento entre o artefato e o executável ..."
	@clang -O0 -o $(EXE_TERM) $(OBJ_TERM)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
EXE_PONTO = bin/tests/ut_ponto
DEPS_PONTO = -L bin/shared -lteste -ltempo -llegivel -lterminal

ponto:
	@gcc -O0 -Wall -Iinclude/ -D_UT_PONTO -Wno-main \
		-o $(EXE_PONTO) src/ponto.c $(DEPS_PONTO) -lm
	@echo "Compilado os testes-unitários de 'ponto.c' em bin/tests."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
EXE_RANDOM = bin/tests/ut_aleatorio
SRC_RANDOM = src/aleatorio.c
FLAGS_RANDOM = -I include/ -D_UT_ALEATORIO -Wall -Werror
DEPS_RANDOM = -Lbin/shared/ -lteste -llegivel -lterminal -ltempo

aleatorio:
	@gcc -Wall $(FLAGS_RANDOM) -o $(EXE_RANDOM) $(SRC_RANDOM) $(DEPS_RANDOM) -lm
	@echo "Compilado os testes-unitários de 'aleatorio' em bin/tests."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
TEMPO_VERBOSE = -D_DEBUG_ALTERA_STATUS \
	-D_DEBUG_CRIA_TEMPORIZADOR \
	-D_DEBUG_DESTROI_TEMPORIZADOR \
	-D_DEBUG_DESTROI_CRONOMETRO \
	-D_DEBUG_CRIA_CRONOMETRO 
TEMPO_COMPILA = -D_UT_TEMPO -D_DEBUG_CRIA_CRONOMETRO \
					 -D_DEBUG_DESTROI_CRONOMETRO
TEMPO_FLAGS = -I include -std=gnu18 -lm -Wall
TEMPO_DEPEDENCIAS = -L bin/static -llegivel
EXE_TEMPO = bin/tests/ut_tempo

tempo: 
	@gcc $(TEMPO_FLAGS) $(TEMPO_COMPILA) -o $(EXE_TEMPO) \
		src/tempo.c $(TEMPO_DEPEDENCIAS)
	@echo "Compilado os testes-unitários de 'tempo' em bin/tests."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
COMPILAR_STR = -D_PALAVRAS -D_UT_STRING -D_CONCATENA_STRINGS -D__debug__
EXE_STR = bin/tests/ut_estringue
DEPS_STR = $(OBJS_TESTE) build/teste.o

estringue:
	@gcc -O0 -g -Wall -I ./include $(COMPILAR_STR) \
		-o $(EXE_STR) src/estringue.c $(DEPS_STR) \
		-lm -Lbin/shared -lteste -ltempo -llegivel -lterminal
	@echo "Compilado os testes-unitários de 'estringue' em bin/tests."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
impressao:
	@clang -I include/ -D_UT_IMPRESSAO -o bin/tests/ut_impressao src/impressao.c build/terminal.o build/listaarray_ref.o
	@echo "Compilado os testes-unitários de 'impressao' em bin/tests."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
legivel:
	@clang -O0 -std=c2x -I include/ -Wall \
		-D_UT_LEGIVEL \
		-o bin/tests/ut_legivel src/legivel.c -lm \
		-L bin/static -lteste -ltempo -lterminal
	@echo "Compilado os testes-unitários de 'legivel' em bin/tests."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
progresso:
	@clang -O0 -std=gnu2x -I include/ -Wall \
		-D_UT_PROGRESSO \
		-o bin/tests/ut_progresso src/progresso.c -lm \
		-L bin/static -lteste -llegivel -ltempo -lterminal
	@echo "Compilado os testes-unitários de 'progresso' em bin/tests."
	
# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
conversao:
	@clang -c -D__UT_CONVERSAO__ -D__debug__ -I include/ \
		-o build/conversao-teste.o src/conversao.c
	@clang -g -std=gnu18 -Wall -Wextra -I include/ \
		-o bin/tests/ut_conversao build/conversao-teste.o \
		-lm -Lbin/shared -lteste -ltempo -llegivel -lterminal
	@echo "Compilado os testes-unitários de 'conversao' em bin/tests."

run-conversao:
	./bin/tests/ut_conversao
# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

combinatoria:
	clang -Iinclude/ -O0 -Wall -D__unit_tests__ \
		$(DEPS_RANDOM) -lm -o bin/tests/ut_combinatoria combinatoria.c

run-combinatoria:
	./bin/tests/ut_combinatoria

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
impressao:
	clang -Iinclude/ -O0 -Wall -D__unit_tests__ \
		-o bin/tests/ut_impressao src/impressao.c \
		-Lbin/shared -llaref -lterminal -lteste -llegivel -ltempo -lm

run-impressao:
	./bin/tests/ut_impressao

# === === ===  === === === === === === === === === === === === === === ===
#
# 					Compilação e Coleção de Estruturas de Dados
# 					 			e seus Testes Unitários
#
# === === ===  === === === === === === === === === === === === === === ===
compila-testes-unitarios-colecoes: \
	cria-raiz-de-artefatos \
	conjunto-ref \
	hashtable-ref \
	pilha-ligada-ref \
	lista-posicional-ref \
	lista-array-ref \
	fila-ligada-ref

roda-testes-unitarios-colecoes:
	@$(EXE_PL)
	@$(EXE_AL)
	@$(EXE_SET_REF)
	@$(EXE_HT_I)
	@$(EXE_FA_I)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
OBJS_SET_REF = -L bin/static -lteste -llegivel -ltempo -lterminal
COMPILA_SET_REF = -D_MEDIA_DOS_SLOTS -D_UT_CONJUNTO -D__debug__
EXE_SET_REF = bin/tests/ut_conjunto_ref
SRC_SET_REF = src/estrutura-de-dados/conjunto_ref.c
DEPS_SET_REF_SHARED = -Lbin/shared -lteste -llegivel -ltempo -lterminal

conjunto-ref:
	@clang -O0 -std=gnu18 -Wall -I include/ $(COMPILA_SET_REF) \
		-Wno-gnu-folding-constant \
		-o bin/tests/ut_conjunto_ref $(SRC_SET_REF) \
		$(OBJS_SET_REF) -lm \
		-L bin/shared -lteste -llegivel -ltempo -lterminal
	@echo "Teste 'ut_conjunto_ref' compilado."

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
EXE_PL = bin/tests/ut_pilha_ligada
MOSTRA_PL = -D_UT_PILHA_LIGADA -D_DESTROI_PL -D__debug__
DEPS_PL = build/progresso.o -L bin/static -lteste -ltempo -llegivel -lterminal

pilha-ligada-ref:
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
SRC_FA_REF = src/estrutura-de-dados/filaarray_ref.c
EXE_FA_REF = bin/tests/ut_fila_array_ref
COMPILA_FA_REF = -D_UT_FILA_ARRAY \
					-DALOCACAO_E_DEALOCACAO \
					-D_REDIMENSIONAMENTO
DEPS_FA_REF = -L bin/shared -lteste -llegivel -lterminal -ltempo -laleatorio

fila-array-ref: aleatorio.o
	@echo "Compilando teste-unitário '$(EXE_FA_REF)'..."
	@gcc -I include/ -Wall $(COMPILA_FA_REF) \
		-o $(EXE_FA_REF) $(SRC_FA_REF) $(DEPS_FA_REF) -lm

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
COMPILA_LA_REF = -D_ALOCACAO_E_DESALOCACAO -D_UT_ARRAY_LISTA -D_TO_STRING
SRC_LA_REF = src/estrutura-de-dados/listaarray_ref.c
DEPS_LA_REF = build/terminal.o build/tempo.o -L bin/static -lteste -llegivel -lm -ltempo
EXE_LA_REF = bin/tests/ut_lista_array_ref

lista-array-ref: libteste
	gcc -I include/ -Wall $(COMPILA_LA_REF) \
		-o $(EXE_LA_REF) $(SRC_LA_REF) $(DEPS_LA_REF)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
SRC_LP_REF = src/estrutura-de-dados/listaposicional_ref.c
EXE_LP_REF = bin/tests/ut_listaposicional
DEPS_LP_REF = -L bin/static -lteste -ltempo -llegivel -lm -lterminal

lista-posicional-ref:
	clang -I include -D_UT_LISTA_POSICIONAL -Wall -O0 \
		-Wno-incompatible-pointer-types-discards-qualifiers \
		-Wno-incompatible-pointer-types \
		-o $(EXE_LP_REF) $(SRC_LP_REF) $(DEPS_LP_REF)

# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
fila-circular-ref:
	clang -O0 -I include/ -Wall -Werror -DUT_FILA_CIRCULAR -D__debug__ \
		-o bin/tests/ut_filacirular_ref \
		src/estrutura-de-dados/filacircular_ref.c \
		-Lbin/static -lteste -ltempo -llegivel -lterminal -lprogresso -lm
	
# --- --- ---  --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
fila-ligada-ref:
	clang -Wall -Wextra -Iinclude/ -D__UT_FILA_LIGADA_REF__ \
		-Wno-unused-parameter \
		-o bin/tests/ut_fila_ligada_ref \
		src/estrutura-de-dados/filaligada_ref.c

# === === ===  === === === === === === === === === === === === === === ===
#
# 					 Compilação de Bibliotecas Estáticas
# 				tanto do Pacote Principal como Coleçoes e
# 						demais Estruturas de Dados
#
# === === ===  === === === === === === === === === === === === === === ===
compila-bibliotecas: \
	cria-raiz-de-artefatos \
	lib-progresso \
	lib-legivel \
	lib-tempo \
	lib-terminal \
	lib-aleatorio \
	lib-teste \
	lib-ponto \
	lib-conversao

compila-bibliotecas-colecoes: \
	cria-raiz-de-artefatos \
	lib-hashtable-ref \
	lib-pilha-ligada-ref \
	lib-fila-circular-ref

EXE_LIBPROGRESSO_I = ./bin/shared/libprogresso.so
EXE_LIBPROGRESSO_II = ./bin/static/libprogresso.a
SRC_LIBPROGR = src/progresso.c
OBJ_LIBPROGR = build/progresso.o

lib-progresso: progresso.o
	@clang -O3 -Oz -I include -o $(EXE_LIBPROGRESSO_I) -fPIC -shared $(SRC_LIBPROGR) -lc
	@echo "compilação de uma biblioteca compartilhada."
	@ar crs $(EXE_LIBPROGRESSO_II) $(OBJ_LIBPROGR)
	@echo "compilação de uma biblioteca estática."

EXE_LEGIVEL_I = bin/shared/liblegivel.so
EXE_LEGIVEL_II = bin/static/liblegivel.a
OBJ_LEGIVEL = build/legivel.o

lib-legivel: legivel.o
	@gcc -o $(EXE_LEGIVEL_I) -fPIC -shared $(OBJ_LEGIVEL) -lc
	@echo "compilação de uma biblioteca compartilhada."
	@ar crs $(EXE_LEGIVEL_II) $(OBJ_LEGIVEL)
	@echo "compilação de uma biblioteca estática."

EXE_TST_I = bin/shared/libteste.so
EXE_TST_II = bin/static/libteste.a
SRC_TST = src/teste.c
OBJS_LIB_TST = build/tempo.o build/legivel.o build/terminal.o

lib-teste: teste.o
	@ar crs $(EXE_TST_II) build/teste.o
	@echo "compilação de uma biblioteca estática."
	@gcc -O3 -Os -Wall -I include/ -fPIC -shared \
		-o $(EXE_TST_I) $(SRC_TST) $(OBJS_LIB_TST)
	@echo "compilação de uma biblioteca compartilhada."

EXE_TIME_I = bin/shared/libtempo.so
SRC_TIME = build/tempo.o build/legivel.o
EXE_TIME_II = bin/static/libtempo.a

lib-tempo: legivel.o tempo.o
	@gcc -I include/ -fPIC -shared -o $(EXE_TIME_I) $(SRC_TIME) -Wall
	@echo "compilação de uma biblioteca compartilhada."
	@ar crs $(EXE_TIME_II) build/tempo.o
	@echo "compilação de uma biblioteca estática."

lib-terminal:
	@clang -I include/ -shared -fPIC -Wall \
		-o bin/shared/libterminal.so build/terminal.o
	@echo "biblioteca compartilhada 'libterminal.so'compilada."
	@ar crs bin/static/libterminal.a build/terminal.o
	@echo "compilação de uma biblioteca estática."

lib-ponto: ponto.o
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libponto.so build/ponto.o
	@echo "biblioteca compartilhada 'libponto.so' compilada."
	@ar crs bin/static/libponto.a build/ponto.o
	@echo "biblioteca estática 'libponto.a' compilada."

lib-aleatorio: aleatorio.o
	@gcc -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libaleatorio.so build/aleatorio.o
	@echo "Biblioteca compartilhada 'libaleatorio.so' compilada."
	@ar crs bin/static/libaleatorio.a build/aleatorio.o
	@echo "Biblioteca estática 'libaleatorio.a' compilada."

lib-conversao: conversao.o
	@clang -std=gnu2x -I include/ -shared -fPIC -Wextra -Wall \
		-o bin/shared/libconversao.so build/conversao.o
	@echo "Biblioteca compartilhada 'libconversao.so' compilada."
	@ar crs bin/static/libconversao.a build/conversao.o
	@echo "Biblioteca estática 'libconversao.a' compilada."


lib-hashtable-ref: hashtable-ref.o
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libhtref.so build/hashtable_ref.o
	@echo "biblioteca compartilhada 'libhtref.so' compilada."
	@ar crs bin/static/libhtref.a build/hashtable_ref.o
	@echo "biblioteca estática 'libhtref.a' compilada."

lib-pilha-ligada-ref: pilha-ligada-ref.o
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libplref.so build/pilhaligada_ref.o 
	@echo "biblioteca compartilhada 'libplref.so' compilada."
	@ar crs bin/static/libplref.a build/pilhaligada_ref.o
	@echo "biblioteca estática 'libplref.a' compilada."

lib-lista-array-ref: lista-array-ref.o
	@clang -O3 -Os -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/liblaref.so src/estrutura-de-dados/listaarray_ref.c
	@echo "biblioteca compartilhada 'liblaref.so' compilada."
	@ar crs bin/static/liblaref.a build/listaarray_ref.o
	@echo "biblioteca estática 'liblaref.a' compilada."

lib-fila-circular-ref: fila-circular-ref.o
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libfcref.so build/filacircular_ref.o
	@echo "biblioteca compartilhada 'libflref.so' compilada."
	@ar crs bin/static/libfcref.a build/filacircular_ref.o
	@echo "biblioteca estática 'libflref.a' compilada."

# === === ===  === === === === === === === === === === === === === === ===
#
#						Compilação dos Testes Integrais
#
# === === ===  === === === === === === === === === === === === === === ===
compila-testes-integrais: \
	cria-raiz-de-artefatos \
	fatorizacao-com-hashtable \
	testando-todos-objetos-gerados \
	distribuicao-de-numeros-gerados-randomicamente \
	usando-iteradores-de-cada-colecao \
	frequencia-de-letras-do-dicionario

fatorizacao-com-hashtable:
	clang -O0 -I include/ -Wall \
		tests/fatorizacao_com_hashtable.c \
		-o bin/tests/it_fatorizacao_com_hashtable \
		-L bin/shared -lhtref

testando-todos-objetos-gerados:
	clang -O0 -I include/ -Wall \
		-o bin/tests/it_testando_todos_objetos_gerados \
		tests/testando_todos_objetos_gerados.c \
		-L bin/shared -ltempo -lprogresso -laleatorio -llegivel -lponto -lm


distribuicao-de-numeros-gerados-randomicamente:
	clang -O0 -std=gnu2x -I include/ -Wall \
		-D_DESTRUICAO_HT \
		-o bin/tests/it_distruibacao_de_numeros_gerados_randomicamente \
		tests/distribuicao_de_numeros_gerados_randomicamente.c \
		-L bin/shared -lprogresso -laleatorio -lhtref -lm

usando-iteradores-de-cada-colecao:
	clang -O0 -std=gnu2x -I include/ -Wall \
		-o bin/tests/it_usando_iteradores_de_cada_colecao \
		tests/usando_iteradores_de_cada_colecao.c \
		-L bin/shared -lhtref -lplref

frequencia-de-letras-do-dicionario:
	clang -O0 -std=gnu2x -I include/ -Wall \
		-o bin/tests/it_frequencia_de_letras_do_dicionario \
		tests/frequencia_de_letras_do_dicionario.c \
		-L bin/shared -lhtref -lplref -lprogresso -laleatorio

processo-de-desalocacao:
	@gcc -O0 -std=gnu2x -I include/ -Wall \
	-o bin/tests/it_processo_de_desalocacao tests/processo_de_desalocacao.c \
	-L bin/shared -lteste -ltempo -llegivel -lterminal \
		-lprogresso -lplref -lfcref -laleatorio -lm
