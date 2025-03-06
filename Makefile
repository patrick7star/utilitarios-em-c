#   Todas bibliotecas de módulos e submódulos, e testes unitários e 
# integrais serão encontrados organizados abaixo. Você poderá compilar,
# copia-los, exclui-los de forma coletiva ou indivídual.
# --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- -
# ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~~~ ~
# 'ponto' não está terminado!
.PHONY = test-estringue test-conversao test-impressao 

# Quase todos testes de módulo abaixo usam o 'módulo teste'. Aqui vai 
# a ligação ao sua lib compartilhada, assim não é preciso escrever toda
# vez isso no final de cada teste, apenas usar a variável.
TESTADOR = -Lbin/shared -lteste -ltempo -llegivel -lterminal -lm
HEADERS = ./include/
DLL = bin/shared/

# Cria estrutura onde artefatos compilados, de todos tipos, serão
# depositados:
cria-raiz-de-artefatos:
	@mkdir --parents --verbose bin/shared
	@mkdir --parents --verbose bin/static
	@mkdir --parents --verbose bin/tests
	@mkdir --parents --verbose build

#lista todos compilados:
listagem-compilacao:
	exa --tree --sort=changed --reverse bin/ build/

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
VERSAO = v1.2.2
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

limpa:
	@echo "\nExcluindo tudo que já foi compilado ...\n"
	@rm -frv bin/ build/

# === === ===  === === === === === === === === === === === === === === ====
#
#							Compila/ou Executa Todos Grupos:
#			   	Objetos, Testes Unitários, Bibliotecas e etc.
#
# === === ===  === === === === === === === === === === === === === === ====
compila-principais-objetos: obj-legivel obj-terminal obj-tempo obj-teste \
	obj-aleatorio obj-ponto obj-progresso obj-conversao

compila-principais-bibliotecas: lib-progresso lib-legivel lib-tempo \
	lib-terminal lib-aleatorio lib-teste lib-ponto lib-conversao

compila-principais-tudo: cria-raiz-de-artefatos \
	compila-principais-objetos compila-principais-bibliotecas \
	all-teste all-legivel all-terminal all-tempo all-ponto all-conversao \
	all-progresso all-lista-array-ref all-impressao all-memoria

compila-testes-unitarios: test-terminal test-ponto test-aleatorio \
	test-tempo test-impressao test-legivel test-conversao test-estringue

roda-testes-unitarios: run-teste run-terminal run-ponto run-tempo \
	run-estringue
	
compila-bibliotecas-colecoes: lib-hashtable-ref lib-pilha-ligada-ref \
	lib-fila-circular-ref lib-lista-array-ref lib-fila-ligada-ref

compila-objetos-colecoes: obj-hashtable-ref obj-conjunto-ref \
	obj-pilha-ligada-ref obj-lista-array-ref obj-deque-ligada-ref \
	obj-fila-ligada-ref obj-fila-array-ref

compila-testes-unitarios-colecoes: test-conjunto-ref test-hashtable-ref \
	test-pilha-ligada-ref test-lista-posicional-ref test-lista-array-ref \
	test-fila-ligada-ref

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Teste
# === === ===  === === === === === === === === === === === === === === ====
COMPILA_TST = -D__unit_tests__ -D__debug__ -D__debug__executa_testes_b
DEPS_TST 	= build/tempo.o build/legivel.o build/terminal.o
EXE_TST 		= bin/tests/ut_teste
EXE_TST_I 	= bin/shared/libteste.so
EXE_TST_II 	= bin/static/libteste.a

all-teste: obj-teste test-teste lib-teste

obj-teste:
	@gcc -Wall -O3 -Os -I include/ -c -o build/teste.o src/teste.c
	@echo "Gerou o arquivo objeto 'teste.o' em 'build'."

lib-teste: obj-teste
	@gcc -Iinclude/ -o $(EXE_TST_I) -shared -fPIC src/teste.c $(DEPS_TST)
	@echo "Compilação da biblioteca compartilhada 'libteste.so'."
	@ar crs $(EXE_TST_II) build/teste.o
	@echo "Compilação da biblioteca estática 'libteste.a'."

test-teste:
	@gcc -Isrc/teste -Iinclude -Wall -Werror \
		-c -o build/amostras.o src/teste/amostras.c
	@gcc -Wall -Iinclude $(COMPILA_TST) \
		-c -o build/test-teste.o src/teste.c
	@gcc -O0 -g3 -Iinclude/ $(COMPILA_TST) \
		-o $(EXE_TST) build/test-teste.o \
		$(DEPS_TST) build/progresso.o build/amostras.o -lm
	@echo "Compilado os testes-unitários de 'teste.c' em bin/tests."

check-teste:
	@echo "checando a sintaxe do 'teste.c' ..."
	gcc -I include/ $(COMPILA_TESTE) -Wall src/teste.c $(OBJS_TESTE) -lm

run-teste:
	./$(EXE_TST)

clean-teste:
	rm -v $(EXE_TST) $(EXE_TST_I) $(EXE_TST_II)

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Terminal
# === === ===  === === === === === === === === === === === === === === ====
OBJ_TERM = build/terminal_teste.o
EXE_TERM = ./bin/tests/ut_terminal

all-terminal: obj-terminal test-terminal lib-terminal

obj-terminal:
	@gcc -O3 -Os -I include -c src/terminal.c -o build/terminal.o
	@echo "Gerou o arquivo objeto 'terminal.o' em 'build'."

lib-terminal:
	@clang -I include/ -shared -fPIC -Wall \
		-o bin/shared/libterminal.so build/terminal.o
	@echo "biblioteca compartilhada 'libterminal.so'compilada."
	@ar crs bin/static/libterminal.a build/terminal.o
	@echo "compilação de uma biblioteca estática."

test-terminal:
	@echo "Compilando artefato 'terminal_teste.o' em 'build' ..."
	@clang -std=gnu18 -Wall -I ./include -c \
		-D__debug__ -D_UT_TERMINAL \
		-o $(OBJ_TERM) src/terminal.c
	@echo "Ligamento entre o artefato e o executável ..."
	@clang -O0 -o $(EXE_TERM) $(OBJ_TERM)

run-terminal:
	./$(EXE_TERM)

clean-terminal:
	@rm -vf $(EXE_TERM)

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Ponto
# === === ===  === === === === === === === === === === === === === === ====
EXE_PONTO = bin/tests/ut_ponto
DEPS_PONTO = -L bin/shared -lteste -ltempo -llegivel -lterminal

all-ponto: obj-ponto test-ponto lib-ponto

obj-ponto:
	@clang -O3 -Os -I include/ -Wall -c -o build/ponto.o src/ponto.c
	@echo "Gerou o arquivo objeto 'ponto.o' em 'build'."

lib-ponto:
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libponto.so build/ponto.o
	@echo "biblioteca compartilhada 'libponto.so' compilada."
	@ar crs bin/static/libponto.a build/ponto.o
	@echo "biblioteca estática 'libponto.a' compilada."

test-ponto:
	@gcc -O0 -Wall -Iinclude/ -D_UT_PONTO -Wno-main \
		-o $(EXE_PONTO) src/ponto.c $(DEPS_PONTO) -lm
	@echo "Compilado os testes-unitários de 'ponto.c' em bin/tests."

run-ponto:
	./$(EXE_PONTO)

clean-ponto:
	@rm -vf $(EXE_PONTO)

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Aleatório
# === === ===  === === === === === === === === === === === === === === ====
EXE_RANDOM = bin/tests/ut_aleatorio
SRC_RANDOM = src/aleatorio.c
FLAGS_RANDOM = -I include/ -D__debug__ -D_UT_ALEATORIO -Wall -Werror
DEPS_RANDOM = -Lbin/shared/ -lteste -llegivel -lterminal -ltempo \
				  -limpressao

all-aleatorio: obj-aleatorio test-aleatorio lib-aleatorio

obj-aleatorio:
	@gcc -O3 -Os -c -I include/ -Wall -Werror -o build/aleatorio.o src/aleatorio.c
	@echo "Gerou o arquivo objeto 'aleatorio.o' em 'build'."

test-aleatorio:
	@gcc -Wall $(FLAGS_RANDOM) -o $(EXE_RANDOM) $(SRC_RANDOM) $(DEPS_RANDOM) -lm
	@echo "Compilado os testes-unitários de 'aleatorio' em bin/tests."

lib-aleatorio:
	@gcc -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libaleatorio.so build/aleatorio.o
	@echo "Biblioteca compartilhada 'libaleatorio.so' compilada."
	@ar crs bin/static/libaleatorio.a build/aleatorio.o
	@echo "Biblioteca estática 'libaleatorio.a' compilada."

run-aleatorio:
	./$(EXE_RANDOM)

clean-aleatorio:
	@rm -vf $(EXE_RANDOM)

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Tempo
# === === ===  === === === === === === === === === === === === === === ====
TEMPO_VERBOSE = -D_DEBUG_ALTERA_STATUS \
					 -D_DEBUG_CRIA_TEMPORIZADOR \
					 -D_DEBUG_DESTROI_TEMPORIZADOR \
					 -D_DEBUG_DESTROI_CRONOMETRO \
					 -D_DEBUG_CRIA_CRONOMETRO 
TEMPO_FLAGS  = -I include -std=gnu18 -lm -Wall

SRC_TIME 	 = build/tempo.o build/legivel.o
EXE_TIME 	 = bin/tests/ut_tempo
EXE_TIME_I 	 = bin/shared/libtempo.so
EXE_TIME_II  = bin/static/libtempo.a
DEPS_TIME 	 = -Lbin/shared -llegivel
COMPILA_TIME = -D_UT_TEMPO -D_DEBUG_CRIA_CRONOMETRO \
					-D_DEBUG_DESTROI_CRONOMETRO

all-tempo: obj-tempo test-tempo lib-tempo

obj-tempo:
	@gcc -O3 -Os -Wall -Werror -c -I include/ src/tempo.c -o build/tempo.o
	@echo "Gerou o arquivo objeto 'tempo.o' em 'build'."

test-tempo: 
	@gcc $(COMPILA_TIME) -Wall -Werror -Iinclude -c -o build/tempo-teste.o src/tempo.c
	@gcc -O0 -o bin/tests/ut_tempo build/tempo-teste.o $(DEPS_TIME)
	@echo "Compilado os testes-unitários de 'tempo' em bin/tests."

lib-tempo:
	@gcc -I include/ -fPIC -shared -o $(EXE_TIME_I) $(SRC_TIME) -Wall
	@echo "compilação de uma biblioteca compartilhada."
	@ar crs $(EXE_TIME_II) build/tempo.o
	@echo "compilação de uma biblioteca estática."

run-tempo:
	./$(EXE_TIME)

clean-tempo:
	@rm -vf $(EXE_TIME_II) $(EXE_TIME)

# === === ===  === === === === === === === === === === === === === === ====
# 									Modulo Estringue
# === === ===  === === === === === === === === === === === === === === ====
COMPILAR_STR = -D_PALAVRAS -D_UT_STRING -D_CONCATENA_STRINGS -D__debug__
EXE_STR 		 = bin/tests/ut_estringue
EXE_STR_I 	 = bin/shared/libestringue.so
BUILD_STR 	 = build/estringue.o
BUILD_STR_I  = build/estringue-teste.o
DEPS_STR 	 = $(OBJS_TESTE) build/teste.o
SRC_STR 	 	 = src/estringue.c

all-estringue: obj-estringue test-estringue lib-estringue

obj-estringue:
	@gcc -Os -Wall -Werror -Iinclude -c -o $(BUILD_STR) $(SRC_STR)
	@echo "Gerou o arquivo objeto 'estringue.o' em 'build'."

lib-estringue:
	@gcc -Iinclude -shared -fPIC -o $(EXE_STR_I) $(BUILD_STR) \
		-Lbin/static -llaref
	@echo "Biblioteca compartilhada 'libestringue.so' compilada."
	@ar crs bin/static/libestringue.a build/estringue.o
	@echo "Biblioteca estática 'libestringue.a' compilada."

test-estringue:
	@gcc -Iinclude $(COMPILAR_STR) -c -o $(BUILD_STR_I) $(SRC_STR)
	@gcc -O0 -g -Wall -I ./include \
		-o $(EXE_STR) $(BUILD_STR_I) $(DEPS_STR) $(TESTADOR)
	@echo "Compilado os testes-unitários de 'estringue' em bin/tests."

run-estringue:
	./$(EXE_STR)

clean-estringue:
	@rm -vf $(BUILD_STR_I) $(BUILD_STR)  $(EXE_STR) $(EXE_STR_I) \
		bin/static/libestringue.a 


# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Legível
# === === ===  === === === === === === === === === === === === === === ====
EXE_LEGIVEL		= bin/tests/ut_legivel
EXE_LEGIVEL_I  = bin/shared/liblegivel.so
EXE_LEGIVEL_II = bin/static/liblegivel.a
DEPS_LEGIVEL 	= -Lbin/shared -lteste -ltempo -lterminal -lm

all-legivel: obj-legivel test-legivel lib-legivel

obj-legivel:
	@gcc -I$(HEADERS) -O3 -Os -c src/legivel.c -o build/legivel.o
	@echo "Gerou o arquivo objeto 'legivel.o' em 'build'."

test-legivel:
	@gcc -I$(HEADERS) -std=gnu2x -O0 -o -Wall -Werror -c -o \
		build/legivel-teste.o src/legivel.c
	@gcc -I$(HEADERS) -D_UT_LEGIVEL \
		-o $(EXE_LEGIVEL) src/legivel.c $(DEPS_LEGIVEL)
	@echo "Compilado os testes-unitários de 'legivel' em bin/tests."

lib-legivel:
	@gcc -o $(EXE_LEGIVEL_I) -fPIC -shared -lc build/legivel.o -lm
	@echo "Compilação de uma biblioteca compartilhada 'liblegivel.so'."
	@ar crs $(EXE_LEGIVEL_II) build/legivel.o
	@echo "Compilação de uma biblioteca estática 'libprogresso.a'."

run-legivel:
	./$(EXE_LEGIVEL)

clean-legivel:
	@rm -vf $(EXE_LEGIVEL) $(EXE_LEGIVEL_I)  $(EXE_LEGIVEL_II) \
		build/legivel.o build/legivel-teste.o

# === === ===  === === === === === === === === === === === === === === ===
# 									Modulo Progresso
# === === ===  === === === === === === === === === === === === === === ===
EXE_PROG    = bin/tests/ut_progresso
LIB_SO_PROG = ./bin/shared/libprogresso.so
LIB_A_PROG  = ./bin/static/libprogresso.a
SRC_PROG = src/progresso.c

all-progresso: obj-progresso lib-progresso test-progresso

obj-progresso:
	@clang -O3 -Os -I include/ -Wall -c -o build/progresso.o $(SRC_PROG)
	@echo "Gerou o arquivo objeto 'progresso.o' em 'build'."

lib-progresso: obj-progresso
	@clang -I include -o $(LIB_SO_PROG) -fPIC -shared build/progresso.o -lc
	@echo "Compilação de uma biblioteca compartilhada 'libprogresso.so' em 'bin/shared'."
	@ar crs $(LIB_A_PROG) build/progresso.o
	@echo "Compilação de uma biblioteca estática 'libprogresso.a' em 'bin/static'."

test-progresso:
	@clang -O0 -std=gnu2x -I include/ -Wall -D_UT_PROGRESSO \
		-c -o build/progresso-teste.o $(SRC_PROG)
	@clang -O0 -std=gnu2x -I include/ -Wall -D_UT_PROGRESSO \
		-o $(EXE_PROG) build/progresso-teste.o -lm $(TESTADOR)
	@echo "Compilado os testes-unitários de 'progresso' em bin/tests."

run-progresso:
	./bin/tests/ut_progresso

clean-progresso:
	@rm -vf build/progresso.o build/progresso-teste.o \
		bin/tests/ut_progresso bin/shared/libprogresso.so \
		bin/static/libprogresso.a
	
# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Conversão
# === === ===  === === === === === === === === === === === === === === ====
all-conversao: obj-conversao lib-conversao test-conversao

obj-conversao:
	@clang -Iinclude/ -Wall -O3 -Oz -c -o build/conversao.o src/conversao.c
	@echo "Gerou o arquivo objeto 'conversao.o' em 'build'."

lib-conversao:
	@clang -std=gnu2x -I include/ -shared -fPIC -Wextra -Wall \
		-o bin/shared/libconversao.so build/conversao.o
	@echo "Biblioteca compartilhada 'libconversao.so' compilada."
	@ar crs bin/static/libconversao.a build/conversao.o
	@echo "Biblioteca estática 'libconversao.a' compilada."

test-conversao:
	@clang -c -D__UT_CONVERSAO__ -D__debug__ -I include/ \
		-o build/conversao-teste.o src/conversao.c
	@clang -g -std=gnu18 -Wall -Wextra -I include/ \
		-o bin/tests/ut_conversao build/conversao-teste.o \
		-lm -Lbin/shared -lteste -ltempo -llegivel -lterminal
	@echo "Compilado os testes-unitários de 'conversao' em bin/tests."

run-conversao:
	./bin/tests/ut_conversao

clean-conversao:
	@rm -vf build/conversao.o build/conversao-teste.o \
		bin/tests/ut_conversao bin/shared/libconversao.o \
		bin/static/libconversao.a

# === === ===  === === === === === === === === === === === === === === ===
# 									Modulo Impressão
# === === ===  === === === === === === === === === === === === === === ===
all-impressao: obj-impressao lib-impressao test-impressao

obj-impressao:
	@clang -O3 -Iinclude/ -std=gnu2x -Wall -Werror \
		-c -o build/impressao.o src/impressao.c
	@echo "Gerou o arquivo objeto 'impressao.o' em 'build'."

lib-impressao: obj-impressao
	@clang -Iinclude/ -shared -fPIC -o bin/shared/libimpressao.so build/impressao.o -Lbin/static -llaref
	@echo "Biblioteca compartilhada 'libimpressao.so' compilada."
	@ar crs bin/static/libimpressao.a build/impressao.o
	@echo "Biblioteca estática 'libimpressao.a' compilada."

test-impressao:
	@clang -Iinclude/ -g3 -O0 -Wall -Werror -D__unit_tests__ -D__debug__ \
		-c -o build/impressao-teste.o src/impressao.c 
	@clang -o bin/tests/ut_impressao build/impressao-teste.o \
		$(TESTADOR) -llaref
	@echo "Compilado os testes-unitários de 'impressao' em bin/tests."

run-impressao:
	./bin/tests/ut_impressao

clean-impressao:
	@rm -v -f bin/tests/ut_impressao build/impressao-teste.o \
		build/impressao.o bin/shared/libimpressao.so bin/static/libimpressao.a

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Combinatória
# === === ===  === === === === === === === === === === === === === === ====
STLIB_CBT 	= bin/static/libcombinatoria.a
DYLIB_CBT 	= bin/shared/libcombinatoria.so
EXE_CBT 		= bin/tests/ut_combinatoria
OBJ_TST_CBT = build/combinatoria-teste.o
OBJ_CBT		= build/combinatoria.o

all-combinatoria: obj-combinatoria lib-combinatoria test-combinatoria

obj-combinatoria:
	@clang -I$(HEADERS) -O3 -Oz -Wall -c -o $(OBJ_CBT) src/combinatoria.c
	@echo "Objeto do módulo 'combinatória' gerado."

lib-combinatoria: obj-combinatoria
	@clang -I$(HEADERS) -shared -fPIC -o $(DYLIB_CBT) $(OBJ_CBT) \
		-Lbin/static -lmemoria
	@echo "Biblioteca compartilhada 'libcombinatoria.so' compilada."
	@ar crs  $(STLIB_CBT) $(OBJ_CBT)
	@echo "Biblioteca estática 'libcombinatoria.a' compilada."

test-combinatoria:
	clang -I$(HEADERS) -c -Wall -D__unit_tests__ -D__debug__ \
		-o $(OBJ_TST_CBT) src/combinatoria.c
	clang -I$(HEADERS) -o $(EXE_CBT) $(OBJ_TST_CBT) \
		 $(DEPS_RANDOM) -lm -limpressao -llegivel -lmemoria
	@echo "Teste do módulo 'combinatória' gerado."

run-combinatoria:
	./bin/tests/ut_combinatoria

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Fio
# === === ===  === === === === === === === === === === === === === === ====
test-fio:
	gcc -I$(HEADERS) -D__unit_tests__ -Wall -Werror -O0 -c -o build/fio-teste.o src/fio.c
	gcc -I$(HEADERS) -o bin/tests/ut_fio build/fio-teste.o -L$(DLL) -llegivel

run-fio:
	@bin/tests/ut_fio

# === === ===  === === === === === === === === === === === === === === ====
# 									Módulo Memória
# === === ===  === === === === === === === === === === === === === === ====
BUILD_MEM 		= build/memoria.o
BUILD_TST_MEM 	= build/memoria-teste.o
EXE_MEM 			= bin/tests/ut_memoria
DYLIB_MEM 		= bin/shared/libmemoria.so
STLIB_MEM 		= bin/static/libmemoria.a

all-memoria: obj-memoria test-memoria lib-memoria

obj-memoria:
	@gcc -O3 -I$(HEADERS) -Wall -Werror -c -o $(BUILD_MEM) src/memoria.c

test-memoria:
	@gcc -I$(HEADERS) -D__unit_tests__ -Wall \
		-c -o $(BUILD_TST_MEM) src/memoria.c
	@gcc -I$(HEADERS) -o $(EXE_MEM) $(BUILD_TST_MEM)

lib-memoria:
	@gcc -I$(HEADERS) -shared -fPIC -o $(DYLIB_MEM) $(BUILD_MEM)
	@echo "Biblioteca compartilhada 'libmemoria.so' compilada."
	@ar crs $(STLIB_MEM) $(BUILD_MEM)
	@echo "Biblioteca estática 'libmemoria.a' compilada."

run-memoria:
	@bin/tests/ut_memoria

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo HashTable Referência
# === === ===  === === === === === === === === === === === === === === ====
EXE_HT_REF 	 = bin/tests/ut_hashtable_ref
SRC_HT_REF	 = src/estrutura-de-dados/hashtable_ref.c
BUILD_HT_REF = build/hashtable-ref-teste.o
COMPILA_HT_REF = -D_UT_HASHTABLE -D_INSERCAO_HT -D_CRIACAO_HT \
					-D_DESTRUICAO_HT -D_CONTIDO_HT \
					-D_ATUALIZA_HT -D_DELETA_HT -D_OBTEM_HT 
FLAGS_HT_REF = -Wno-unused-variable -Wno-main

all-hashtable-ref: obj-hashtable-ref lib-hashtable-ref test-hashtable-ref

obj-hashtable-ref:
	@clang -O3 -Oz -std=c2x -I include/ -Wall \
		-c -o build/hashtable-ref.o $(SRC_HT_REF)
	@echo "Gerou o arquivo objeto 'hashtable-ref.o', em 'build'."

lib-hashtable-ref: 
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libhtref.so build/hashtable-ref.o
	@echo "Biblioteca compartilhada 'libhtref.so' compilada."
	@ar crs bin/static/libhtref.a build/hashtable-ref.o
	@echo "Biblioteca estática 'libhtref.a' compilada."

test-hashtable-ref:
	@gcc -I include/ $(COMPILA_HT_REF) $(FLAGS_HT_REF) -std=c18 -Wall \
		-c -o $(BUILD_HT_REF) $(SRC_HT_REF) 
	@gcc -o $(EXE_HT_REF) $(BUILD_HT_REF) $(TESTADOR)
	@echo "Teste 'ut_hashtable_ref' compilado."

check-hashtable-ref:
	gcc -fanalyzer -Wall -std=c18 $(BUILD_HT_REF) $(TESTADOR)

clean-hashtable-ref:
	@rm -f -v $(EXE_HT_REF) $(BUILD_HT_REF) build/hashtable-ref.o \
		bin/static/libhtref.a bin/shared/libhtref.so

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Conjunto Referência
# === === ===  === === === === === === === === === === === === === === ====
COMPILA_SET_REF 	  = -D_MEDIA_DOS_SLOTS -D_UT_CONJUNTO -D__debug__
EXE_SET_REF 		  = bin/tests/ut_conjunto_ref
LIB_SO_SET_REF      = bin/shared/libconjref.so
LIB_A_SET_REF 		  = bin/static/libconjref.a
SRC_SET_REF 		  = src/estrutura-de-dados/conjunto_ref.c

# Compila tudo acima, mas não rota o testes, ou exclui o que foi compilado.
all-conjunto-ref: obj-conjunto-ref lib-conjunto-ref test-conjunto-ref

obj-conjunto-ref:
	@clang -std=gnu2x -O3 -Oz -I include/ -Wall -c \
		-o build/conjunto-ref.o $(SRC_SET_REF) 
	@echo "Gerou o arquivo objeto 'conjunto-ref.o', em 'build'."

test-conjunto-ref:
	@clang -O0 -std=gnu18 -Wall -I include/ $(COMPILA_SET_REF) \
		-Wno-gnu-folding-constant -o $(EXE_SET_REF) $(SRC_SET_REF) \
		$(OBJS_SET_REF) -lm $(TESTADOR)
	@echo "Teste 'ut_conjunto_ref' compilado."

lib-conjunto-ref: 
	@clang -I include/ -shared -fPIC -Wall \
		-o bin/shared/libconjref.so build/conjunto-ref.o
	@echo "Biblioteca compartilhada 'libconjref.so' compilada."
	@ar crs bin/static/libconjref.a build/conjunto-ref.o
	@echo "Biblioteca estática 'libconjref.a' compilada."

run-conjunto-ref:
	./$(EXE_SET_REF)

clean-conjunto-ref:
	@rm -fv $(EXE_SET_REF) build/conjunto-ref.o $(LIB_A_SET_REF) \
		$(LIB_SO_SET_REF) 

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Lista-Array Referência
# === === ===  === === === === === === === === === === === === === === ====
COMPILA_LA_REF = -D_ALOCACAO_E_DESALOCACAO -D__unit_tests__ -D_TO_STRING
SRC_LA_REF = src/estrutura-de-dados/listaarray_ref.c
BUILD_LA_REF = build/lista-array-ref-teste.o
DEPS_LA_REF = -Lbin/shared -lteste -llegivel -lm -ltempo
EXE_LA_REF = bin/tests/ut_lista_array_ref

all-lista-array-ref: obj-lista-array-ref lib-lista-array-ref \
	test-lista-array-ref

obj-lista-array-ref:
	@clang -O3 -Oz -I include/ -Wall -c \
		src/estrutura-de-dados/listaarray_ref.c \
		-o build/lista-array-ref.o
	@echo "Gerou o arquivo objeto 'lista-array-ref.o', em 'build'."

lib-lista-array-ref: 
	@clang -O3 -Os -std=gnu2x -I include/ -shared -fPIC \
		-o bin/shared/liblaref.so build/lista-array-ref.o
	@echo "Biblioteca compartilhada 'liblaref.so' compilada."
	@ar crs bin/static/liblaref.a build/lista-array-ref.o
	@echo "Biblioteca estática 'liblaref.a' compilada."

test-lista-array-ref:
	@gcc -c -Iinclude -Wall -Werror $(COMPILA_LA_REF) \
		-o $(BUILD_LA_REF) $(SRC_LA_REF)
	@gcc -O0 -I include/ -Wall $(COMPILA_LA_REF) \
		-o $(EXE_LA_REF) $(BUILD_LA_REF) $(DEPS_LA_REF)
	@echo "Teste 'ut_lista_array_ref' compilado."

clean-lista-array-ref:
	@rm -fv $(EXE_LA_REF) $(BUILD_LA_REF) build/lista-array-ref.o \
		bin/static/liblaref.a bin/shared/liblaref.so

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Pilha-Ligada Referência
# === === ===  === === === === === === === === === === === === === === ====
EXE_PL 	 	 = bin/tests/ut_pilha_ligada
MOSTRA_PL 	 = -D_UT_PILHA_LIGADA -D_DESTROI_PL -D__debug__
DEPS_PL 	 	 = $(TESTADOR) -lprogresso
BUILD_PL_REF = build/pilha-ligada-ref-teste.o
SRC_PL_REF 	 = src/estrutura-de-dados/pilhaligada_ref.c

all-pilha-ligada-ref: obj-pilha-ligada-ref lib-pilha-ligada-ref test-pilha-ligada-ref

obj-pilha-ligada-ref:
	@clang -O3 -Oz -std=c2x -I include/ -Wall \
		-c -o build/pilha-ligada-ref.o $(SRC_PL_REF)
	@echo "Gerou o arquivo objeto 'pilhaligada_ref.o', em 'build'."

lib-pilha-ligada-ref:
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libplref.so build/pilha-ligada-ref.o 
	@echo "Biblioteca compartilhada 'libplref.so' compilada."
	@ar crs bin/static/libplref.a build/pilha-ligada-ref.o
	@echo "Biblioteca estática 'libplref.a' compilada."

test-pilha-ligada-ref:
	@clang $(MOSTRA_PL) -Iinclude -Wall -c -o $(BUILD_PL_REF) $(SRC_PL_REF)
	@clang -Iinclude/ -O0 -o $(EXE_PL) $(BUILD_PL_REF) -lm $(DEPS_PL)
	@echo "Teste 'ut_pilha_ligada_ref' compilado."

clean-pilha-ligada-ref:
	@rm -v -f $(EXE_PL) $(BUILD_PL_REF) bin/shared/libplref.so \
		build/pilha-ligada-ref.o bin/static/libplref.a 

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Fila-Array Referência
# === === ===  === === === === === === === === === === === === === === ====
SRC_FA_REF 		= src/estrutura-de-dados/filaarray_ref.c
EXE_FA_REF 		= bin/tests/ut_fila_array_ref
BUILD_FA_REF	= build/fila-array-ref.o
COMPILA_FA_REF = -D_UT_FILA_ARRAY -DALOCACAO_E_DEALOCACAO \
					  -D_REDIMENSIONAMENTO
DEPS_FA_REF 	= $(TESTADOR) -laleatorio

all-fila-array-ref: obj-fila-array-ref lib-fila-array-ref \
	test-fila-array-ref

obj-fila-array-ref:
	@clang -O3 -Oz -I include/ -Wall -c -o build/fila-array-ref.o \
		src/estrutura-de-dados/filaarray_ref.c
	@echo "Gerou o arquivo objeto 'fila-array-ref.o', em 'build'."

lib-fila-array-ref:
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/libfaref.so build/fila-array-ref.o 
	@echo "Biblioteca compartilhada 'libfaref.so' compilada."
	@ar crs bin/static/libfaref.a build/fila-array-ref.o
	@echo "Biblioteca estática 'libfaref.a' compilada."

test-fila-array-ref:
	@gcc -O0 -Iinclude -Wall -Werror $(COMPILA_FA_REF) \
		-c -o $(BUILD_FA_REF) $(SRC_FA_REF)
	@gcc -Iinclude -o $(EXE_FA_REF) $(BUILD_FA_REF) $(DEPS_FA_REF)
	@echo "Teste 'ut_fila_array_ref' compilado com sucesso."

clean-fila-array-ref:
	@rm -v -f $(EXE_FA_REF) $(BUILD_FA_REF) bin/shared/libfaref.so \
		build/fila-array-ref.o bin/static/libfaref.a 

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Lista-Posicional Referência(dev)
# === === ===  === === === === === === === === === === === === === === ====
SRC_LP_REF = src/estrutura-de-dados/listaposicional_ref.c
EXE_LP_REF = bin/tests/ut_lista_posicional
BUILD_LP_REF = build/lista-posicional-ref.o
DEPS_LP_REF = -L bin/static -lteste -ltempo -llegivel -lm -lterminal

all-lista-posicional-ref: obj-lista-posicional-ref \
	lib-lista-posicional-ref test-lista-posicional-ref

test-lista-posicional-ref:
	clang -Iinclude -D_UT_LISTA_POSICIONAL -Wall -Werror -O0 \
		-c -o $(BUILD_LP_REF) $(SRC_LP_REF)
	clang -Iinclude -o $(EXE_LP_REF) $(BUILD_LP_REF) $(DEPS_LP_REF)

obj-lista-posicional-ref:
	@clang -O3 -Oz -I include/ -Wall -c -o build/lista-posicional-ref.o \
		src/estrutura-de-dados/listaposicional_ref.c
	@echo "Gerou o arquivo objeto 'lista-posicional-ref.o', em 'build'."

lib-lista-posicional-ref:
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o bin/shared/liblpref.so build/lista-posicional-ref.o 
	@echo "Biblioteca compartilhada 'liblpref.so' compilada."
	@ar crs bin/static/liblpref.a build/lista-posicional-ref.o
	@echo "Biblioteca estática 'liblpref.a' compilada."

clean-lista-posicional-ref:
	@rm -v -f $(EXE_LP_REF) $(BUILD_LP_REF) bin/shared/liblpref.so \
		build/lista-posicional-ref.o bin/static/liblpref.a 

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Fila-Circular Referência
# === === ===  === === === === === === === === === === === === === === ====
SRC_FC_REF 	 = src/estrutura-de-dados/filacircular_ref.c 
OBJ_FC_REF 	 = build/fila-circular-ref.o
OBJ_FC_REF_I = build/fila-circular-ref-teste.o
EXE_FC_REF 	 = bin/tests/ut_filacirular_ref 
DYLIB_FC_REF = bin/shared/libfcref.so
STLIB_FC_REF = bin/static/libfcref.a

all-fila-circular-ref: obj-fila-circular-ref lib-fila-circular-ref \
	test-fila-circular-ref

obj-fila-circular-ref:
	@clang -O3 -Oz -I include/ -Wall -Werror \
		-c -o $(OBJ_FC_REF) $(SRC_FC_REF)
	@echo "Gerou o arquivo objeto 'fila-circular-ref.o', em 'build'."

test-fila-circular-ref:
	clang -O0 -I include/ -Wall -Werror -DUT_FILA_CIRCULAR -D__debug__ \
		-o $(EXE_FC_REF) $(SRC_FC_REF) $(TESTADOR) -lprogresso
	@echo "Teste 'ut_fila_array_ref' compilado com sucesso."
	
lib-fila-circular-ref: obj-fila-circular-ref
	@clang -std=gnu2x -I include/ -shared -fPIC -Wall \
		-o $(DYLIB_FC_REF) $(OBJ_FC_REF) 
	@echo "Biblioteca compartilhada 'libfcref.so' compilada."
	@ar crs $(STLIB_FC_REF) $(OBJ_FC_REF)
	@echo "Biblioteca estática 'libfcref.a' compilada."

clean-fila-circular-ref:
	@rm -v -f $(EXE_FC_REF) $(OBJ_FC_REF) $(DYLIB_FC_REF) \
		$(OBJ_FC_REF_I) $(STLIB_FC_REF)

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Fila-Ligada Referência
# === === ===  === === === === === === === === === === === === === === ====
SRC_FL_REF   = src/estrutura-de-dados/filaligada_ref.c
OBJ_FL_REF 	 = build/filaligada_ref.o
EXE_FL_REF 	 = bin/tests/ut_fila_ligada_ref
DYLIB_FL_REF = bin/shared/libflref.so
STLIB_FL_REF = bin/static/libflref.a

all-fila-ligada-ref: obj-fila-ligada-ref lib-fila-ligada-ref \
	test-fila-ligada-ref

obj-fila-ligada-ref:
	@clang -O3 -Oz -I$(HEADERS) -Wall -c -o $(OBJ_FL_REF) $(SRC_FL_REF)
	@echo "Gerou o arquivo objeto 'filaligada_ref.o', em 'build'."

lib-fila-ligada-ref: obj-fila-ligada-ref
	@clang -std=gnu2x -I$(HEADERS) -shared -fPIC -Wall \
		-o $(DYLIB_FL_REF) $(OBJ_FL_REF) 
	@echo "Biblioteca compartilhada 'libflref.so' compilada."
	@ar crs $(STLIB_FL_REF) $(OBJ_FL_REF)
	@echo "Biblioteca estática 'libflref.a' compilada."

test-fila-ligada-ref:
	@clang -Wall -Wextra -Iinclude/ -D__UT_FILA_LIGADA_REF__ \
		-Wno-unused-parameter \
		-o $(EXE_FL_REF) $(SRC_FL_REF)
	@echo "Teste de 'fila-ligada-ref' compilada."

run-fila-ligada-ref:
	@echo "Execução dos testes unitários de 'fila-ligada(ref)' ..."
	@bin/tests/ut_fila_ligada_ref

# === === ===  === === === === === === === === === === === === === === ====
# 						 	Modulo Deque-Ligada Referência
# === === ===  === === === === === === === === === === === === === === ====
obj-deque-ligada-ref:
	@clang -O3 -Oz -I include/ -Wall -c \
		-o build/dequeligada_ref.o \
		src/estrutura-de-dados/dequeligada_ref.c
	@echo "Gerou o arquivo objeto 'dequeligada_ref.o', em 'build'."


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
	clang -O0 -std=gnu2x -I$(HEADERS) -Wall \
		-o bin/tests/it_frequencia_de_letras_do_dicionario \
		tests/frequencia_de_letras_do_dicionario.c \
		-L$(DLL) -lhtref -lplref -lprogresso -laleatorio

processo-de-desalocacao:
	@gcc -O0 -std=gnu2x -I include/ -Wall \
	-o bin/tests/it_processo_de_desalocacao tests/processo_de_desalocacao.c \
	-L bin/shared -lteste -ltempo -llegivel -lterminal \
		-lprogresso -lplref -lfcref -laleatorio -lm

permutacoes-com-info:
	gcc -g -std=gnu2x -Iinclude/ -Wall \
		-o bin/tests/it_permutacoes_com_info testes/permutacoes_com_info.c \
		$(TESTADOR) -limpressao -lterminal -lmemoria -lcombinatoria -lflref
