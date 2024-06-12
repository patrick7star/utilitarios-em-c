
  Existe dois tipos de implementações, aquelas que captura referências, e
as que copiam os dados. Estas que copiam são limitados aos tipos primitivos,
e alguns derivados deles, se for preciso algo mais complexo envolvendo eles,
será necessário uma implementação própria -- apesar de que, copiar tal 
código e refazer baseado nos objetos que você necessita não é algo tão 
complexo, se quer, muito trabalhoso. Já no caso das referências, podem
absover qualquer coisa dentro delas, inclusive dados primitivos embrulhados
na 'heap'. Estes últimos podem segura dados mais genéricos.

  Os módulos com implementaçoes tem sufixos que definem suas propriedades, 
os de copia são 'cpy'(que é 'copy' em inglês), já os de referência são 
'ref', isso deve-se muito por motivação de colocar todos seus 'header files'
num mesmo lugar, e não ramificar-lôs em subdiretórios, para cada tipo. Um 
problema com tais implementações é que ambos tipos conflitam, você não 
podem, ainda, chamar uma mesma estrutura ambas com propriedades de 
referências, quanto de cópia, porque os nomes que definem cada uma são 
exatamente iguais, já suas estruturas internas, totalmente diferentes.

  O foco de desenvolvimento inicialmente, será na das referências, por serem
mais amplos, servem para mais casos, e isso implicando numa utilização mais
imediata. Os demais, com estes já bem maduros, serão uma cópia destes, 
porém modificados para abordarem propriedades de cópia. Isso é muito simples
de se fazer, já que, os métodos e operações relacionados a tais estruturas
são o mesmo, independente da propriedade ser cópia/ou referência delas.

  A ideia de tal coleção em C é por causa da limitação da própria linguagem,
que não tem tais coleções por padrão iguais as demais linguagens -- que pode
ser algo até divertido para quem está aprendendo a linguagem, fazer estes
problemas(estrutura) simples; entretanto extramente trabalhoso para quem
quer ser prático. Já refiz vários destas espalhadas em vários programas, ao
invés de fazer apenas uma vez, e copiar -- estavam muito atrelado os códigos
de tais projetos, o que impossibilita uma cópia direta deles -- somente.

