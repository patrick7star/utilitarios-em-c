

/* Criando o tipo de dado, muito importante para qualquer várias
 * soluções, o conjunto. Ele adiciona apenas um dado, se o mesmo dado,
 * ou seja, eles são iguais, for adicionado novamente, ele simplesmente
 * não o faz.
 * A aplicação usa agora como motor interno destra estrutura uma tabela
 * de dispersão, ela em sí evita adicação de elementos repetidos.
 */

#include "tabela_dispersao.h"
#include "tabeladispersao/dado.h"

// outros possíveis nomes para a estrutura.
typedef HashTable Conjunto;
// Só a chave é importante aqui, já que ela é essencial para o cálculo
// hash, por isso ela está sendo renomeada.
typedef CHAVE Elemento;

/* Como o Conjunto é apenas um apelido de uma 'tabela de dispersão', 
 * então, os métodos serão apenas embrulhados pelos métodos originais
 * da estrutura de HashTable. Claro, que algumas modificações especiais
 * o conjunto não recebe uma 'chave' e um 'valor', apenas o "valor",
 * que aqui é chave, já o valor é irrelevante, então na inserção
 * um valor arbitrário será usado. 
 */

Conjunto cria_set() { cria_ht(); }

void destroi_set(Conjunto s) { destroi_ht(s); }

bool insere_set(Conjunto s, Elemento e) 
   { return insere_ht((HashTable)s, e, -1); }

bool remove_set(Conjunto s, Elemento e)
   { return deleta_ht(s, e); }

bool pertence_set(Conjunto s, Elemento e)
   { return contem_ht(s, e); }

bool vazio_set(Conjunto s)
   { return vazia_ht(s); }

uint64_t tamanho_set(Conjunto s)
   { return tamanho_ht(s); }

uint64_t total_de_instancias_set()
   { return total_de_instancias_ht(); }

Conjunto clona_set(Conjunto s) 
   { return clona_ht(s); }
      

#include <stdio.h>

void visualiza_set(Conjunto s) {
   IterHT i = iter_ht(s);

   printf("{..");
   for (Dado e = next_ht(i); e != NULL; e = next_ht(i)) 
      printf("%s, ", get_chave_dado(e));
   printf("\b\b}\n");

   // destruindo iterador todo consumido.
   destroi_iter_ht(i);
}

/* Também renomeando apelidos do iterador, assim como seus métodos. */
typedef IterHT IterSet;

IterSet iter_set(Conjunto s) { 
   return iter_ht(s);
} 

Elemento next_set(IterSet i) 
   { return get_chave_dado(next_ht(i)); }

void destroi_iter_set(IterSet i)
   { destroi_iter_ht(i); }


#include <stdlib.h>
#include <string.h>

/* Mesmo algorítimo de todos outros, iterar os elementos do Conjunto,
 * então concatena eles numa string, dinâmicamente ajustada, no
 * fim, retorna a string como representação textual da estrutura.
 */
char* set_to_str(Conjunto s) {
   IterSet i = iter_set(s);
   uint16_t maior_str = 0;
   for (Elemento e = next_set(i); e != NULL; e = next_set(i)) {
      size_t t = strlen(e);
      if (t > maior_str)
         maior_str = t;
   }
   printf("tamanho: %d\n", maior_str);
   // depois reecriando iterador para outra rodada.
   i = iter_set(s);

   /* Computando o mínimo de caractéres para efetuar tal operação. 
    * Usamos o iterador criador para pegar a maior string que 
    * é representada aqui. Então, multiplicamos o comprimento dela
    * pelo total de itens no conjunto, e também pelo o espaço e 
    * o separador(a vírgula) usado para cada item, e finalmente, 
    * somamos como 2, estes represetando os delimitadores da 
    * representação(os colchetes), e também contabilizando possíveis
    * dois recuos para excluir espaço separadores extras.
    */
   uint64_t qtd = tamanho_set(s) * (maior_str + 2) + 2 + 12;
   char* resultado_fmt = (char*)calloc(qtd, sizeof(char));
   bool houve_iteracao = false;

   resultado_fmt[0] = '{';
   for (Elemento e = next_set(i); e != NULL; e = next_set(i)) {
      strcat(resultado_fmt, e);
      strcat(resultado_fmt, ", ");
      houve_iteracao = true;
   }

   if (houve_iteracao)
      strcat(resultado_fmt, "\b\b}");
   else
      strcat(resultado_fmt, "}");

   destroi_iter_ht(i);
   return resultado_fmt; 
}

/* O algoritimo para fazer a união é, clonar o primeiro argumento, e 
 * inserir indiscriminadamente os elementos do segundo neste clone,
 * que no fim será retornado, já que ele é o resultado da operação
 * de união entre os conjuntos. Digo inserir indiscriminadamente, pois
 * a tabela, que é o verdadeiro núcleo desta estrutura, não insere
 * "entradas" repetidas, ou seja, se tiverem inicialmente itens em
 * comum, a inserção não ocorrerar. */
Conjunto uniao(Conjunto a, Conjunto b) {
   IterSet i = iter_set(b);
   Conjunto juncao = clona_set(a);

   for (Elemento e = next_set(i); e != NULL; e = next_set(i)) 
      { insere_set(juncao, e); }
   destroi_iter_set(i);

   return juncao;
}

/* se dá, apenas verificando se todos elementos de um estão no 
 * outro, vice-versa. */
bool eq_set(Conjunto a, Conjunto b) {
   IterSet iA = iter_set(a); 
   IterSet iB = iter_set(b); 

   // verificando pertecimento do primeiro argumento.
   for (Elemento e = next_set(iA); e != NULL; e = next_set(iA)) {
      // se um apenas não pertencer, a suposta igualdade é quebrada.    
      if (pertence_set(b, e))
         return false;
   }

   // testando pertencimento de elementos no outro.
   for (Elemento e = next_set(iB); e != NULL; e = next_set(iB)) {
      if (pertence_set(a, e))
         return false;
   }

   /* se passou em ambos verificações de pertencimento acima, então
    * a igualdade só pode ser verdadeira. */
   return true;
}

/* está operação é apenas a negação da feita acima. */
bool ne_set(Conjunto b, Conjunto a)
   { return !eq_set(a, b); }
