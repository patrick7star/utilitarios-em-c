"""
   Troca qualquer nome que foi colocada num arquivo, que alguma parte do seu
 código importou. Este script, troca o nome dele, e subsequente cada arquivo
 que o usa.
   Por enquanto, é exclusivo para este código, e para C. Entretanto, pode
 ser modificado futuramente.
"""
import glob, pprint
from pathlib import (Path)
from collections.abc import (Generator)
from queue import (SimpleQueue)

# Antigo nome(corresponde) e o novo a ser alterado.
CORRESPONDE = "dados_testes.h"
NOVO_NOME   = "dados-testes.h"
END_OF_FILE = ''
INVALIDO    = -1
# Aonde serão colada as amostras para testes.
DESPEJO  = Path("./script/data/test")


def laudo_do_encontrado(linha: int, caminho: Path, trecho: str) -> None:
   nome = caminho.name
   trecho = trecho.rstrip('\n')

   print("linha[{:>04d}] | {}:\t'{}'".format(linha, nome, trecho))

def detector(caminho: Path) -> bool:
   linha = 1
   nome = caminho.name
   LEITURA = "rt"

   try:
      with open(caminho, LEITURA) as stream:
         conteudo = stream.readline()

         while conteudo != END_OF_FILE:
            if ("#include" in conteudo) and (CORRESPONDE in conteudo):
               laudo_do_encontrado(linha, caminho, conteudo)
               return linha
            linha += 1
            conteudo = stream.readline()

   except UnicodeDecodeError:
      print("Não foi possível com o arquivo %s" % nome)
      return INVALIDO
   else:
      # Caso não tenha achado nada, também retorna "inválido".
      return INVALIDO

def todos_possiveis_arquivos_com_problema_no_projeto() -> Generator[Path]:
   return map(lambda item: Path(item),
      # Vasculha em todos os diretórios abaixo. As opções são várias diretórios e 
      # subdiretórios com algum possível arquivo com tal trecho nele. Alguns a profundidade
      # vai mais do que a médio dentro de algum nó. Se este arquivo é usado em outro código,
      # com outras linguagens de programação, isso aqui talvez tenha que ser alterado, tanto
      # a extensão, como a adição de novas profundidades. O que é feito aqui agora, 
      # aborda só o que é útil para este projeto.
         glob.glob("src/*.c")    +
         glob.glob("src/*/*.c")  +
         glob.glob("tests/*.c")  +
         glob.glob("src/*/*.c")
   )

def filtra_o_que_sera_substituido(In: Generator[Path]) -> list[(Path, int)]:
   conteudo_filtrado = In
   output = []

   for caminho in conteudo_filtrado:
      linha = detector(caminho)

      if linha != INVALIDO:
         output.append((caminho, linha))
   return output

def cria_ramo_de_diretorios() -> None:
   try:
      makedirs(DESPEJO)
   except FileExistsError:
      print("Ramos de diretórios já existem!")
   else:
      print("'{}' criado com sucesso.".format(DESPEJO))
   finally:
      try:
         mkdir(DESPEJO.joinpath("output"))
      except FileExistsError:
         print("Já existe tal 'output' dentro de 'data/test'.")
      else:
         print("Subdiretório 'output' criado em 'data'.")
      finally:
         pass

def altera_para_novo_nome(In: (Path, int)) -> None:
   fila = SimpleQueue()
   pathname = In[0]; LINHA = In[1]
   cursor = 1
   INJECAO = "#include \"{}\"\n".format(NOVO_NOME)

   # Capturando linha por linha, colocando numa fila para futuramente remover nesta ordem,
   # e claro, com exceção da linha que será modificada.
   with open(pathname, "rt") as streaming:
      for (p, linha) in enumerate(streaming):
         if (p + 1) == LINHA:
            print(
               "\nAlterando linha({} | '{}') ..."
               .format(LINHA, linha.rstrip())
            )
            fila.put(INJECAO)
         else:
            fila.put(linha)

   # Reescrevendo modificações no próprio arquivo. Abre o arquivo novamente, e despeja as 
   # linhas iteradas para dentro de uma fila.
   with open(pathname, "wt") as streaming:
      while (not fila.empty()):
         remocao = fila.get()

         streaming.write(remocao)
         cursor += 1

if __name__ == "__main__":
   OLD_NAME = CORRESPONDE
   print("Altera NOME_ATUAL[{}] por NOVO_NOME[{}].".format(OLD_NAME, NOVO_NOME))

   In = todos_possiveis_arquivos_com_problema_no_projeto()
   Out = filtra_o_que_sera_substituido(In)
   quantia = len(Out)

   print("Foram computadas {} possíveis alterações.".format(quantia))

   if len(Out) == 0:
      print("Não há nada a ser alterado para o padrão '{}'!".format(OLD_NAME))
   else:
      for alteracao in Out:
         altera_para_novo_nome(alteracao)

from unittest import (TestCase, skip)
from os import (makedirs, system, mkdir)
from shutil import (copy as CopyFile, rmtree)

@skip("Algum parâmetro errado(evite o otimizado), e altera todas amostras tomadas.")
class AlteraTrechoDoArquivo(TestCase):
   def altera_para_novo_nome(self, In: (Path, int)) -> None:
      "Copia local da função com algumas diferenças."
      fila = SimpleQueue()
      pathname = In[0]
      LINHA = In[1]
      cursor = 1

      with open(pathname, "rt") as streaming:
         for (p, linha) in enumerate(streaming):
            if (p + 1) == LINHA:
               print(
                  "\nAlterando linha({}) ===> '{}'"
                  .format(LINHA, linha.rstrip())
               )
               fila.put("#include \"{}\"\n".format(NOVO_NOME))
            else:
               fila.put(linha)

      destino = pathname.parent.joinpath("output").joinpath(pathname.name)

      with open(destino, "wt") as streaming:
         while (not fila.empty()):
            remocao = fila.get()
            fmt = "{}ª \t{}".format(cursor, remocao)
            cursor += 1

            streaming.write(fmt)

   def realiza_copias_de_amostras(self):
      In    = todos_possiveis_arquivos_com_problema_no_projeto()
      In_a  = filtra_o_que_sera_substituido(In)
      Out   = map(lambda tupla: tupla[0], In_a)

      for (num, source) in enumerate(Out):
         nome = "amostra({}).c".format(num + 1)
         destino = DESPEJO.joinpath(nome)

         if (not destino.exists()):
            CopyFile(source, destino)
            print("'{}' copiado para 'data/'.".format(nome))
         else:
            print("Arquivo '{}' já existe!".format(nome))

   def cria_raiz_de_diretorios_se_preciso(self):
      if not DESPEJO.exists():
         cria_ramo_de_diretorios()
      else:
         print("Não é preciso criar nada!")

   def nova_amostra(self) -> Generator[Path]:
      regex = "./script/data/test/amostra*.c"
      arquivos = glob.glob(regex)

      return map(lambda item: Path(item), arquivos)

   def setUp(self):
      self.cria_raiz_de_diretorios_se_preciso()
      self.realiza_copias_de_amostras()

   def tearDown(self):
      for item in DESPEJO.joinpath("output").iterdir():
         item.unlink()
         print('\t- ', item, "removido.")

   def runTest(self):
      In  = self.nova_amostra()
      Out = filtra_o_que_sera_substituido(In)

      for item in Out:
         self.altera_para_novo_nome(item)

@skip("Pode excluir estrutura já criada, esta com backups importantes!")
class CriaTodoRamoDeDiretorios(TestCase):
   def tearDown(self):
      rmtree("./script/data")

   def runTest(self):
      cria_ramo_de_diretorios()
      system("tree -n ./script")

@skip("Pode não funcionar em testes futuramente!")
class FiltraAlvosDoProblema(TestCase):
   def runTest(self):
      populacao = todos_possiveis_arquivos_com_problema_no_projeto()
      amostra = filtra_o_que_sera_substituido(populacao)

      pprint.pprint(amostra)

@skip("Pode não funcionar em testes futuramente!")
class ArquivosComPossiveisProblemas(TestCase):
   def runTest(self):
      input = todos_possiveis_arquivos_com_problema_no_projeto()
      
      pprint.pprint(list(input))

