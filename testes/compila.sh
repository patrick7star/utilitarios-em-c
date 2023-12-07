#!/usr/bin/sh

# compilação de todos testes nesta pasta:

# teste do 'header file' terminal:
gcc -o ut_terminal unit_tests_terminal.c ../teste.c ../terminal.c ../tempo.c ../legivel.c
echo "compilação dos testes unitários de 'terminal.c'"


gcc -o ut_ponto unit_tests_ponto.c  ../ponto.c \
../teste.c ../terminal.c ../legivel.c ../tempo.c -lm
echo "compilação dos testes unitários de 'ponto.c'"

gcc -o ut_legivel unit_tests_legivel.c ../tempo.c ../teste.c ../terminal.c
echo "compilação dos testes unitários de 'legivel.c'"

gcc -o ut_tempo unit_tests_tempo.c ../tempo.c ../teste.c ../terminal.c ../legivel.c -Wall -Wno-main 
echo "compilação dos testes 'tempo.c'"

gcc -o ut_progresso unit_tests_barra_progresso.c ../barra_de_progresso.c \
 ../teste.c ../terminal.c ../tempo.c ../legivel.c -Wall
echo "compilação dos testes 'barra_de_progresso.c'"

gcc -o ut_teste unit_tests_teste.c ../teste.c -lm \
 ../legivel.c ../tempo.c ../terminal.c  \
  -Wno-incompatible-pointer-types
echo "compilação dos testes 'teste.c'"
