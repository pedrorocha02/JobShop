HEADERS = util.h graph.h sequencial.h threads.h

default:main

main.o: main.c	$(HEADERS)
	gcc -c main.c -lpthread -o main.o

main: main.o
	gcc main.o -lpthread -o main

clean:
	-rm -f main.o
	-rm -f main