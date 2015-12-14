all: main.o  common.o cache.o
	gcc main.o common.o cache.o

main.o: main.c
	gcc -c main.c

common.o: common.c
	gcc -c common.c

cache.o: cache.c
	gcc -c cache.c

clean:
	rm a.out main.o common.o cache.o
