all: main.o  common.o cache.o
	gcc main.o common.o cache.o -o cache

main.o: main.c
	gcc -c main.c

common.o: common.c
	gcc -c common.c

cache.o: cache.c
	gcc -c cache.c

clean:
	rm cache main.o common.o cache.o
