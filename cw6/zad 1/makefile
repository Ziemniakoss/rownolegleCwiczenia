nwd.o: nwd.c nwd.h
	mpicc -c nwd.c

main: main.c nwd.c
	mpicc main.c nwd.c -o main -lm
	$(info Przykladowe wywolanie: mpirun -n 8 ./main 128 64 32 256 4096 1024 2048 3200)
clean:
	rm *.o 
