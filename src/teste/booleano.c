
static bool string_esta_minuscula(char* string) {
/* Verifica se uma string está minúscula, ou seja, todos seus caractéres
 * são minúsculos. 
 */
   char* atual = string;
   while ((bool)*atual) {
      char _char = atual[0];
      if (isupper(_char))
         return false;
      atual++;
   } 
   return true;
}

static char* string_minuscula(char* original) {
   size_t i = 0, t = strlen(original);
   char* lowercase = malloc((t + 1) * sizeof(char));

   while ((bool)*original) {
      char _char = original[0];
      lowercase[i] = tolower(_char);
      original++;
      i++;
   }
   lowercase[t] = '\0';
   return lowercase;
}

bool str_to_bool(char* s) {
   char* nova_str;

   if (!string_esta_minuscula(s)) {
      nova_str = string_minuscula(s);
   } else 
      nova_str = s;

   bool e_verdadeiro = {
      !strcmp(nova_str, "v") || 
      !strcmp(nova_str, "true") || 
      !strcmp(nova_str, "t") || 
      !strcmp(nova_str, "verdadeiro")
   };
   bool e_falso = {
      !strcmp(nova_str, "f") || 
      !strcmp(nova_str, "false") || 
      !strcmp(nova_str, "falso")
   };

   if (e_verdadeiro)
      return true;
   else if (e_falso)
      return false;
   else
      { perror("a string não é válida!"); abort(); }
}

/* Nota: o inverso desta função, 'booleano para string', é definida, via 
 *			macro, no cabeçalho da função. 
 */

