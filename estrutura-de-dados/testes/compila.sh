

# compila os testes da deque-ligada.
gcc -o ut_deque_ligada unit_tests_deque_ligada.c \
../dado.c ../deque_ligada.c \
../../teste.c ../../terminal.c ../../tempo.c ../../legivel.c \
../../aleatorio.c -lm
echo "unit-test do 'deque-ligada.c' compilado."

# compila o dado.
gcc -o ut_dado unit_tests_dado.c \
../dado.c ../../teste.c \
../../terminal.c ../../tempo.c ../../legivel.c
echo "unit-test do 'dado.c' compilado."

# compila os testes da deque.
gcc -o ut_deque unit_tests_deque.c ../dado.c ../deque.c \
../../aleatorio.c -lm \
../../legivel.c ../../tempo.c ../../teste.c ../../terminal.c 
echo "unit-test do 'deque.c' compilado."

# compila os testes da pilha-ligada.
gcc -o ut_pilha_ligada unit_tests_pilha_ligada.c \
../deque_ligada.c ../dado.c ../pilha_ligada.c \
../../teste.c ../../terminal.c ../../tempo.c ../../legivel.c \
../../aleatorio.c -lm -Wno-main -Wall
echo "unit-test do 'pilha_ligada.c' compilado."

# compila os testes da fila-ligada.
gcc -Wall -Wno-main -o ut_fila_ligada unit_tests_fila_ligada.c \
../dado.c ../fila_ligada.c ../deque_ligada.c \
../../teste.c ../../terminal.c ../../tempo.c ../../legivel.c
echo "unit-test do 'fila_ligada.c' compilado."

# compila os testes da fila-ligada.
gcc -o ut_tabela_dispersao \
unit_tests_tabela_dispersao.c ../tabela_dispersao.c \
../tabeladispersao/deque_ligada.c ../tabeladispersao/dado.c \
../../teste.c ../../terminal.c ../../tempo.c ../../legivel.c \
-Wall -lm -Wno-main -Wno-int-conversion
echo "unit-test do 'tabela_dispersao.c' compilado."

# compila o conjunto.
gcc -o ut_conjunto unit_tests_conjunto.c \
../conjunto.c  ../tabela_dispersao.c \
../tabeladispersao/deque_ligada.c ../tabeladispersao/dado.c \
../../teste.c ../../terminal.c ../../tempo.c ../../legivel.c 
echo "unit-test da 'conjunto.c' compilado."

