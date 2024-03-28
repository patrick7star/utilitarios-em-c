
/* Cabeçalho com os tipos de dados, métodos deles ou funções relacionadas.
 * 
 */


#ifndef HASHTABLE_H

 // todos os tipos de dados e seus apelidos:
 

 // alocação e destruição da estrutura.
 hashtable_t* cria_com_capacidade_ht(size_t q); 
 hashtable_t* cria_ht()
 bool destroi_ht(hashtable_t* m);


 bool insere_ht(HashTable m, Chave ch, Valor v); 
 bool contem_ht(HashTable m, Chave ch); 

/* acha uma chave e troca o valor dela, o retorno é bem sucedido se 
 * a atualização foi definitivamente feita, e falho caso o contrário,
 * que seria se por exemplo, se não houvesse a chave demanda para
 * atualização. */
 bool atualiza_ht(HashTable m, Chave ch, Valor v); 

/* A operação de remoção é apenas usar o método de atualização, para
 * inserir uma chave-em-branco no lugar da 'chave' dada, o resto é 
 * apenas realinhar das 'entradas' na array, tipo chaves-em-branco
 * no fim dela, 'entradas' não vázia no começo, não importando a ordem.
 */
 bool deleta_ht(HashTable m, Chave ch);

/* Operações, principalmente de encapsulamento, extremamente triviais
 * mais necessárias para esconder, futurumente quando este arquivo
 * fazer parte da 'lib' acessar tais valores da estrutura. */
 bool vazia_ht(HashTable m);

 size_t tamanho_ht(HashTable m);

/* Obtém o endereço do valor da dada 'chave' na tabela, se a tal não 
 * pertencer a 'tabela', então simplesmente retorna 'null'. Talvez
 * o nome 'obtem' não seja adequado, no futuro pode ser alterado ou
 * ter outros métodos que 'embrulham' este.
 */
 Valor* obtem_ht(HashTable m, Chave ch); 

#endif // FIM_DO_HASHTABLE_H
