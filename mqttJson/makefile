src = $(wildcard ./*.c)  
obj = $(patsubst ./%.c, ./%.o, $(src)) 

myArgs= -Wall -g -lpaho-mqtt3cs
target=mqttpublish.out
CC=gcc

ALL:$(target)

$(target):$(obj)
		$(CC) $^ -o $@ $(myArgs)

$(obj):%.o:%.c
		$(CC) -c $^ -o $@ $(myArgs)

clean:
		-rm -rf $(obj) $(target)

.PHONY: clean ALL
