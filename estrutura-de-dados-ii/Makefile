
all: hashtable-testes pilha-array-testes

TESTE_DEPEDENCIA = ../teste.c ../tempo.o ../legivel.o ../terminal.o
FLAGS_HT =  -I "../" -D_UT_HASHTABLE 

hashtable-testes: $(TESTE_DEPEDENCIA)
	echo "compilando 'hashtable.c' ..."
	gcc -o ut_hashtable hashtable.c $(FLAGS_HT) \
	$(TESTE_DEPEDENCIA) -lm
	echo "feito."

FLAGS_PA =  -I "../" -D_UT_PILHA_ESTATICA 
pilha-array-testes: $(TESTE_DEPEDENCIA)
	echo "compilando 'pilha_array.c' ..."
	gcc -o ut_pilha_array pilha_array.c $(FLAGS_PA) \
	$(TESTE_DEPEDENCIA) -lm
	echo "feito."


clean:
	rm -fv ut*
