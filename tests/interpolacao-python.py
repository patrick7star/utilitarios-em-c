"""
   Este é um testes da interpolação de funções escritas em C, poréḿ portadas
 para Python.
"""

import interpola
from pprint import pprint

QUANTIA = 50; COLUNA = 4

print("\nVerificando a documentação de tal:")
pprint(dir(interpola))
print("\n\nAs {} palavras sorteadas:\n".format(QUANTIA))

for numero in range(QUANTIA):
   if (numero + 1) % COLUNA == 0:
      print('')
   result = interpola.palavra_aleatoria()
   print("{:<20s}".format(result), end=' ')

else:
   print('\n')


print("Frases coloridas:")
print(interpola.colori_string("Maçã", 0x1f))
