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
 * 
 *   Este é o mesmo que o 'dados_testes.h', mas com um traço, ao invés do
 * 'underline'. Eu deixo o anterior, pois muitos contam com ele. Então, 
 * até a transição está completa. Eu preciso deixa-lo, até reescrever todos
 * usando esta.
 */

#ifndef DADOS_TESTES_H
#define DADOS_TESTES_H
#include <stdint.h>

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
/* Computados de forma automática pelo compilador:
 *    - LIVROS_DIDATICOS_GENEROS
 *    - OBJETOS
 */
// Tamanho padrão do pointeiro de caractéres.
#define StrSz sizeof(char*)


// contei 9 frutas.
const char* frutas[] = {
   "morango", "banana", "pêra", "uva", "mamão",
   "laranja", "abacaxi", "melância", "goiaba",
};

// eu contei 15 nomes.
const char* nomes_de_meninas[] = {
   "Júlia", "Amanda", "Sara", "Elisa", "Mariana", "Gabriela", "Maria", 
   "Joana", "Carol", "Maísa", "Rosana", "Larissa", "Geovana", "Alice", 
   "Marta"
};

// Eu contei 14 nomes.
const char* nomes_de_meninos[] = {
   "João", "César", "Maurício", "Júlio", "Marcos",
   "Paulo", "Johnatan", "Mateus", "Pedro", "Ronaldo",
   "Romeu", "Gilberto", "Carlos", "Ivan"
};

// eu contei 9 nomes.
const char* girls_names[] = {
   "Sarah", "Mayce", "Cristine", "Deborah", "Lauren",
   "Samantha", "Jane", "Madison", "Megan"
};

// eu contei 11 nomes.
const char* boys_names[] = {
   "Peter", "Noah", "Matthew", "Dickson", "Carl",
   "John", "Taylor", "Charlie", "Dave", "Thomas", "Jacob"
};

// eu contei 9 legumes.
const char* legumes[] = { 
   "batata", "tomate", "cenoura", "beterraba", "pimentão",
   "mandioca", "alface", "cebola", "repolho"
};

const char sexo[] = { 'F', 'M', 'N'};

// eu contei 10 inteiros.
const uint8_t valores_padronizados[] = { 9, 0, 8, 1, 7, 2, 6, 3, 5, 4};

// eu contei 36 inteiros positivos.
const uint16_t valores_padronizados_i[] = {
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

const int8_t valores_padronizados_ii[] = {
   -9, -8, -7, -6, -5, -4, -3, -2, -1, 
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};

// Extremos do determinado tipo.
const int8_t valores_padronizados_iii[] = { INT8_MIN, 0, INT8_MAX };

const int32_t valores_padronizados_iv[] = {
   -99999, -88888, -77777, -66666, -55555, -44444, -33333, -22222, -11111,
   -9999, -8888, -7777, -6666, -5555, -4444, -3333, -2222, -1111,
   -999, -888, -777, -666, -555, -444, -333, -222, -111,
   -99, -88, -77, -66, -55, -44, -33, -22, -11,
   -9, -8, -7, -6, -5, -4, -3, -2, -1, 
   0,
   9, 8, 7, 6, 5, 4, 3, 2, 1, 
   99, 88, 77, 66, 55, 44, 33, 22, 11,
   999, 888, 777, 666, 555, 444, 333, 222, 111,
   9999, 8888, 7777, 6666, 5555, 4444, 3333, 2222, 1111,
   99999, 88888, 77777, 66666, 55555, 44444, 33333, 22222, 11111
};

// eu contei 16 strings.
const char* veiculos[] = {
   "bicicleta", "skate", "carro", "ônibus", 
   "patins", "cavalo", "carroça", "escada rolante",
   "prancha de surf", "bote", "barco", "vela", "navio",
   "avião", "paraquedas", "helicoptero"
};

const char silly_poem[] = (
   "roses are red; violets are blue; i see you big pretty"
   " eyes... as a huge cloud comes me soon"
);

// Contabilizei quize.
const char* objetos[] = {
   "copo",  "cerca", "porta", "cadeado", "mala", 
   "bolsa", "garrafa", "ármario", "cesto",
   "mola", "balde", "capacete", "moeda", "livro",
   "colher"
};
// Contando ...
#define OBJETOS sizeof(objetos) / StrSz

const char frase_i[] = "hoje será um bom dia!";
const char frase_ii[] = "Todas generalizações são falsas, incluse esta. -- Mark Twain";
const char phrase_i[] = "When you stare an abyss, the abyss stares back";
const char phrase_ii[] = "I do desire we may be better strangers.";
const char phrase_iii[] = "Tempt not a desperate man.";

const char* frases_i[] = {
   "Hoje é um novo dia, de um tempo, que começou!",
   "Terça-feira depois do almoço é o tempo cósmico da semana",
   "sua mira está para cima, e à direita também",
   "Independentemente",
   "Você é sensato, justo e amoroso"
};

// Contei doze.
const char* livros_didaticos_generos[] = {
   "Matemática", "Português", "Inglês", "Geografia", "História", 
   "Sociologia", "Filosofia", "Gramática", "Artes", "Física", "Química", 
   "Biologia"
};
#define LIVROS_DIDATICOS_GENEROS \
   sizeof(livros_didaticos_generos) / StrSz

const char* fruits[] = {
   "strawberry", "grape", "banana", "orange", "tomato", "blueberry",
   "pear", "apple", "lemon", "mango", "cherry", "pineapple", "watermelon",
   "papaya", "melon", "avocado", "grapefruit", "kiwi", "lime", "eggplant",
   "plum", "pomegranate", "rambute", "apricot", "peach", "raspberry",
   "blackberry"
};
// Contabilizando o total ...
#define FRUITS sizeof(fruits) / StrSz

// Contabilização de todas listas com strings:
#define FRASES_I sizeof(frases_i) / StrSz

/*   Às vezes, como amostra, quero todas strings daqui numa única lista, 
 * todas constantes definidas abaixo ajudam à fazer tal concatenação de 
 * forma bem fácil e simples. 
 *
 * Obs.: a lista de todas listas é obrigada a seguir a ordem da lista
 *       com seus respectivos comprimentos, e claro, não é aceitável
 *       qualquer faltante.
 */
#define TODAS_LISTAS_COM_STRS ( \
   VEICULOS + GIRLS_NAMES + BOYS_NAMES + NOMES_DE_MENINAS + \
   NOMES_DE_MENINOS + OBJETOS + LEGUMES + FRUITS + FRASES_I \
   + LIVROS_DIDATICOS_GENEROS \
)
const int TLCS_COMPRIMENTOS[] = {
   VEICULOS, GIRLS_NAMES, BOYS_NAMES, NOMES_DE_MENINAS, NOMES_DE_MENINOS,
   OBJETOS, LEGUMES, FRUITS, FRASES_I , LIVROS_DIDATICOS_GENEROS
};
const char** todas_listas_com_strs[] = {
   veiculos, girls_names, boys_names, nomes_de_meninas, nomes_de_meninos,
   objetos, legumes, fruits, frases_i, livros_didaticos_generos
};
#endif
