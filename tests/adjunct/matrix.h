/*   Submódulo próprio para a matriz e a string que são usadas com buffer
 * dos demais submódulos.
 *   Aqui posso referenciar mais esta ferramenta, que não é compatível com
 * multithreading.
 */
#ifndef _MATRIX_BUFFER_H_
#define _MATRIX_BUFFER_H_
// O tamanho do buffer da matriz quadrada na stack é definida aqui:
#define MATRIZ_DIMENSAO_QUADRADA 30

typedef char (*MatrizRef)[MATRIZ_DIMENSAO_QUADRADA];
typedef MatrizRef MRef;

 void    set_string_buffer    (char*);
 char*   get_string_buffer    (void);
 int     string_buffer_size   (void);
 void    clear_string_buffer  (void);
 char    recuo_usado          (void);
 void    set_dimensao_matriz  (int l, int c);
 void    get_dimensao_matrix  (int*);
 int     get_dimensao_linha   (void);
 int     get_dimensao_coluna  (void);
 MRef    get_matrix_buffer    (void);
 void    clear_matriz_buffer  (void);
 void    matriz_print_debug   (void);
 void    string_to_matriz     (void);
 void    matriz_to_string     (void);

#endif
