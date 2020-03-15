memory.o: memory.c memory.h
	cc memory.c -c

vector.o: vector.c vector.h memory.o
	cc -c vector.c

main: main.c vector.o memory.o
	cc main.c vector.o memory.o -o main

clean:
	rm *.o 
