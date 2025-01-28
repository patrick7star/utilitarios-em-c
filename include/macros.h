/* Macros comuns e simples que faltam no C inicialmente, apesar de serem
 * bem simples de ser feitos. Podem também ter no C, mas estão de forma 
 * totalmente ilegível e não-prática. 
 */

 /* Macros importantíssimos para realizarem várias funções importantes. 
  * Talvez, no futuro, tal sejam portados para o módulo 'teste'. Acho que
  * seja mais adequado lá. 
  */
 #define stringfy(VAR) #VAR
 #define _concatena_com_nada(NADA, VAR) NADA##VAR
 #define destringfy(VAR) _concatena_com_nada(,VAR) 
 // Apelidos mais curtos:
 #define strfy(VAR) stringfy(VAR)
 #define destrfy(VAR) destringfy(VAR)

 /*   Pega o valor lógico, e transforma numa literal string que explica 
  * que tipo de valor este booleano assume.
  */
 #define bool_to_str(VL) (VL ? "true": "false")
