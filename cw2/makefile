matrix.o: matrix.c matrix.h
	cc -c matrix.c

matrix_calc.o: matrix_calc.c matrix_calc.h
	cc -c matrix_calc.c

main: main.c matrix.c matrix_calc.c
	cc main.c matrix.c matrix_calc.c -lpthread -o main -lm

clean:
	rm *.o 
