#!/usr/bin/sh

# compilação de todos testes nesta pasta:

# teste do 'header file' terminal:
# gcc -o ut_terminal unit_tests_terminal.c ../teste.c ../terminal.c


gcc -o ut_ponto unit_tests_ponto.c  ../ponto.c \
../teste.c ../terminal.c ../legivel.c ../tempo.c -lm
echo "compilação dos testes unitários de 'ponto.c'"
