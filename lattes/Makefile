CFLAGS = -Wall -g  # gerar "warnings" detalhados e infos de depuração
 
objs = index.o  
# regra default (primeira regra)
all: index
 
# regras de ligacao
index: $(objs)
 
# regras de compilação
index.o: index.c   
# remove arquivos temporários
clean:
	-rm -f $(objs) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f index