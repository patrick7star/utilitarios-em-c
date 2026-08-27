#include <errno.h>

// todos apelidos dados:
typedef struct nodulo_do_hash nodulo_t, *Node; 

struct nodulo_do_hash { 
   // Valores genéricos tanto da chave como do valor:
   GenT chave; 
   GenT valor;

   // referência para próximo item.
   Node seta;
};

static Node cria_nodulo (GenT key, GenT value) {
/* Retorna uma instância inválida ou não. Dependendo se a alocação foi bem
 * sucedidad. */
   nodulo_t* instancia = malloc (sizeof (nodulo_t));

   if (instancia != INVALIDA) {
      instancia->chave = key;
      instancia->valor = value;
      instancia->seta = NULL;
   }
   return instancia;
}

// Agora que tal implementação tem seu próprio submódulo, fica possível 
// adicionar até métodos extras que acrescentam semântica e contexto ao 
// código, porém no escopo principal apenas trariam o problema de rolar
// e buscar trechos durante a codificação.
static void destroi_nodulo(Node self)
   { free(self); }

static Node* array_nodulo(const size_t N)
{
/* Muitas funções alocão arrays de nódulos, concentrarei o código aqui, 
 * então elas apenas usam tal função. É uma boa refatoração do código. 
 * Também traz o complexo tratamento de erro prá cá, fazer localmente lá
 * apenas deixa o código terceiro mais ilegível e com aparência de 
 * trabalhoso. 
 */
   const size_t size = sizeof (Node);
   Node* array = calloc(N, size);
   size_t k = 0;

   if (array == NULL)
      { perror(strerror(errno)); abort(); }

   for (k = 1; k <= N; k++)
      array[k - 1] = NULL;

   return array;
}

static void destroi_array_nodulo(Node* obj)
   { free(obj); }

