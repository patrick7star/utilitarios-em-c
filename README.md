## Utilitários em C
O mesmo que o Python e Rust, porém em C. Algumas irão variar, haverá features neste, mais não nos outros e vice-versa, porém, é um bucado de código útil em C, tijolos, 
para construir qualquer outra coisa. Este tem uma diretório só para estrutura de dados, que são irrelevantes nas outras linguagens.

> Algumas *funções* e *estrutura* importantes:
>> `Temporizador`, `Cronometro`, `Progresso`, `tamanho_legivel`, `executa_funcoes`

#### amostras diversas do que ela pode fazer:
```C
assert (tempo_legivel (86400.0) == "1dia");
assert (logico() == true || logico() == false);
assert (str_to_bool ("verdadeiro"));
assert (bool_to_str (false) == "falso");

uint8_t* dim = dimensao_terminal ();
```

### Notas
O código da biblioteca está sendo feito, inicialmente, para Linux.
