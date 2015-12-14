all: main.o  common.o
	gcc main.o common.o

main.o: main.c
	gcc -c main.c

common.o: common.c
	gcc -c common.c

clean:
	rm common.o main.o a.out