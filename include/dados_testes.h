/* Muitos testes unitários ou de integração, se faz necessários a inserção
 * atualiza, remoção de dados simples, e toda maldita vez, tenho que criar
 * algunas array de strings, valores, caractéres e etc... para sastifazer
 * a demanda. Por isso crio tal arquivo, com vários dados estáticos com 
 * temas conhecidos para que todos testes criados poderem usa-lô como 
 * um quebra-galho.
 *
 * Nunca serão adicionados novos nomes nas variáveis recém criadas abaixo,
 * pois é feito uma contagem, e adicionar novos terá que ser feita uma
 * recontagem. Ao invés disso, novas variáveis com nomes relacionados serão
 * criadas, e tais nomes adicionados. Mas como nomear tal identificador, se
 * o nome relacionado já está sendo usado? -- Você pergunta. Bem, apenas 
 * crie-a com uma contagem diferente(criando uma se não tem), de preferência
 * em números romanos.
 */

#include <stdint.h>

#ifndef DADOS_TESTES_H
#define DADOS_TESTES_H

// tamanho de cada array abaixo:
#define FRUTAS 9llu
#define NOMES_DE_MENINOS 14llu
#define NOMES_DE_MENINAS 15llu
#define GIRLS_NAMES 9llu
#define BOYS_NAMES 11llu
#define LEGUMES 9llu
#define SEXO 3llu
#define VALORES_PADRONIZADOS 10llu
#define VALORES_PADRONIZADOS_I 16llu
#define VEICULOS 16llu


// contei 9 frutas.
const char* frutas[] = {
   "morango", "banana", "pêra", "uva", "mamão",
   "laranja", "abacaxi", "melância", "goiaba",
};

// eu contei 15 nomes.
const char* nomes_de_meninas[] = {
   "Júlia", "Amanda", "Sara", "Elisa", "Mariana",
   "Gabriela", "Maria", "Joana", "Carol", "Maísa",
   "Rosana", "Larissa", "Geovana", "Alice", "Marta"
};

// Eu contei 14 nomes.
const char* nomes_de_meninos[] = {
   "João", "César", "Maurício", "Júlio", "Marcos",
   "Paulo", "Johnatan", "Mateus", "Pedro", "Ronaldo",
   "Romeu", "Gilberto", "Carlos", "Ivan"
};

// eu contei 9 nomes.
char* girls_names[] = {
   "Sarah", "Mayce", "Cristine", "Deborah", "Lauren",
   "Samantha", "Jane", "Madison", "Megan"
};

// eu contei 11 nomes.
char* boys_names[] = {
   "Peter", "Noah", "Matthew", "Dickson", "Carl",
   "John", "Taylor", "Charlie", "Dave", "Thomas", "Jacob"
};

// eu contei 9 legumes.
char* legumes[] = { 
   "batata", "tomate", "cenoura", "beterraba", "pimentão",
   "mandioca", "alface", "cebola", "repolho"
};

char sexo[] = { 'F', 'M', 'N'};

// eu contei 10 inteiros.
uint8_t valores_padronizados[] = { 9, 0, 8, 1, 7, 2, 6, 3, 5, 4};

// eu contei 36 inteiros positivos.
uint16_t valores_padronizados_i[] = {
   1, 11, 111, 1111,
   9, 99, 999, 9999, 
   8, 88, 888, 8888,
   7, 77, 777, 7777,
   3, 33, 333, 3333,
   4, 44, 444, 4444,
   2, 22, 222, 2222,
   5, 55, 555, 5555,
   6, 66, 666, 6666
};

// eu contei 16 strings.
char* veiculos[] = {
   "bicicleta", "skate", "carro", "ônibus", 
   "patins", "cavalo", "carroça", "escada rolante",
   "prancha de surf", "bote", "barco", "vela", "navio",
   "avião", "paraquedas", "helicoptero"
};
#endif
