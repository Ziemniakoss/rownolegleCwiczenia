log_parser.out: main.c str_long_map.o 
	mpicc main.c str_long_map.o -o log_parser.out
	
str_long_map.o: str_long_map.c str_long_map.h
	mpicc str_long_map.c -c

clean:
	rm *.o *.out
