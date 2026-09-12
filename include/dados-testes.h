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
#define VOGAIS 10llu
#define ALFABETO 52llu
/* Computados de forma automática pelo compilador:
 *    - LIVROS_DIDATICOS_GENEROS
 *    - OBJETOS
 */
// Tamanho padrão do pointeiro de caractéres.
#define StrSz sizeof(char*)

const char* alfabeto = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* vogais = "aeiouAEIOU";

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
const char* vehicles[] = {
   "car", "truck", "motorcycle", "bus", "bicycle", "scooter", "train",
   "tram", "subway", "tractor", "van", "pickup truck", "snowmobile",
   "golf cart", "tank", "quad bike", "bulldozer", "forklift", "ambulance",
   "fire truck", "airplane", "helicopter", "rocket", "space shuttle",
   "hot air balloon", "blimp", "drone", "glider", "fighter jet",
   "seaplane", "airship", "satellite", "ship", "boat", "submarine",
   "yacht", "jet ski", "ferry", "sailboat", "hovercraft", "canoe",
   "kayak", "speedboat", "tugboat", "barge", "gondola"
};
#define VEHICLES sizeof(vehicles) / StrSz

const char silly_poem[] = {
   "roses are red; violets are blue; i see you big pretty"
   " eyes... as a huge cloud comes me soon"
};

const char* silly_poems[] = {
   "The moon ate a spoon and burped at noon.",
   "A potato wore a hat and danced with a cat.",
   "The fish took a dish to a swishy parade.",
   "My sock told a rock, 'Please don't tickle my clock.'",
   "The frog bought a log and jogged through the fog.",
   "A pickle rode a bicycle through a popsicle.",
   "The turtle wore purple and sang to a circle.",
   "A chicken named Lou sneezed out a shoe.",
   "A banana in pajamas did cartwheels with llamas.",
   "The jellybean queen wore a limousine.",
   "The carrot grew a parrot and taught it to sing.",
   "The spoon hugged the moon and hummed a tune.",
   "A noodle met a poodle and both became doodles.",
   "A goat in a coat sailed a boat made of toast.",
   "A mouse in a blouse ordered pancakes for the house.",
   "A tiny canoe floated through a bowl of stew.",
   "A penguin named Pete ate a ten-foot beet.",
   "The bear brushed his hair with a pear.",
   "A crab played a cab and drove it to the lab.",
   "A wizard named Buzz turned a fuzzball into jazz.",
   "The duck wore a truck and got stuck in the muck.",
   "The fox in socks put locks on a box.",
   "The snail sent mail by riding a giant quail.",
   "A pancake wore a raincoat and skateboarded on a moat.",
   "A skunk played funk on a very tiny trunk.",
   "The worm wore a uniform and performed in a storm.",
   "A robot ate a carrot and started a dance duet.",
   "A squirrel twirled a pearl while wearing a swirl.",
   "A parrot wore a carrot and marched around the garret.",
   "The cow said wow when she saw a purple plow.",
   "A bee drank tea and rode a bumblebee.",
   "A sheep counted sheep and fell asleep in a heap.",
   "A hippo named Flip did a sideways flip.",
   "A rooster in a booster flew over a toaster.",
   "The pancake met a snake and decided to bake.",
   "A zebra wore a sombrero and danced with a flamingo.",
   "A kangaroo flew a canoe to Timbuktu.",
   "The octopus wore eight shoes and sang the blues.",
   "A llama in pajamas played drums with bananas.",
   "A vampire admired a tire and retired by the fire.",
   "A walrus named Morris married a chorus.",
   "The cactus practiced taxes while relaxing in waxes.",
   "A duckling did juggling while wearing a snugling.",
   "The mouse made a house out of a gingerbread blouse.",
   "The cheese climbed trees and sneezed in the breeze.",
   "A monster named Gus rode a bus made of fuzz.",
   "The cupcake joined a wake-up parade with a lemonade brigade.",
   "The dragon ate a wagon and bragged about his snack.",
   "A biscuit played cricket with a cricket named Cricket.",
   "A jellyfish wished for a selfish shellfish dish.",
   "The moose wore caboose pajamas and sneezed at the llamas.",
   "A raven ate gravy and joined a navy.",
   "A donkey played hockey with a broccoli jockey.",
   "A hedgehog wore a vest and aced a spelling test.",
   "The cloud wore a shroud and laughed very loud.",
   "A teapot tap-danced with a robot in a hot spot.",
   "The potato became a tornado and flew past Colorado.",
   "The noodle rode a poodle to a doodle-filled school.",
   "The giraffe took a laugh on a giraffe-shaped raft.",
   "The hedgehog wore a vest and aced a spelling test."
};
#define SILLY_POEMS sizeof(silly_poems) / StrSz

// Contabilizei quize.
const char* objetos[] = {
   "copo",  "cerca", "porta", "cadeado", "mala",
   "bolsa", "garrafa", "ármario", "cesto",
   "mola", "balde", "capacete", "moeda", "livro",
   "colher"
};

const char* const objects[] = {
   "glasses", "door", "luggage", "chair", "table", "phone", "book", "pen",
   "pencil", "cup", "plate", "fork", "spoon", "knife", "bottle", "wallet",
   "key", "lamp", "window", "bed", "pillow", "blanket", "shoe", "shirt",
   "pants", "clock", "mirror", "bag", "box", "towel", "soap", "brush",
   "comb", "remote", "computer", "keyboard", "mouse", "television", "camera",
   "watch", "umbrella", "backpack", "stapler", "notebook", "paper",
   "scissors", "tray", "bin"
};

// Contando ...
#define OBJETOS sizeof(objetos) / StrSz
#define OBJECTS sizeof(objects) / StrSz

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

const char* phrases[] = {
   "Government of the people, by the people, for the people. — Abraham Lincoln",
   "With malice toward none; with charity for all. — Abraham Lincoln",
   "To be, or not to be: that is the question. — William Shakespeare",
   "Darkness cannot drive out darkness: only light can do that. — Martin Luther King Jr.",
   "Hate cannot drive out hate: only love can do that. — Martin Luther King Jr.",
   "All the world's a stage. — William Shakespeare",
   "The quality of mercy is not strained. — William Shakespeare",
   "A house divided against itself cannot stand. — Abraham Lincoln",
   "True peace is not merely the absence of tension; it is the presence of justice. — Martin Luther King Jr.",
   "Believe you can and you're halfway there. — Theodore Roosevelt",
   "Love all, trust a few, do wrong to none. — William Shakespeare",
   "Don't watch the clock; do what it does. Keep going. — Sam Levenson"
};
// Contabilização de todas listas com strings:
#define FRASES_I sizeof(frases_i) / StrSz
#define PHRASES sizeof(phrases) / StrSz

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
   "blackberry", "strawberry", "grape", "banana", "orange", "tomato",
   "blueberry", "pear", "apple", "lemon", "mango", "cherry", "pineapple",
   "watermelon", "papaya", "melon", "avocado", "grapefruit", "kiwi",
   "lime", "eggplant", "plum", "pomegranate", "rambute", "apricot",
   "peach", "raspberry",
};
// Contabilizando o total ...
#define FRUITS sizeof(fruits) / StrSz

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
