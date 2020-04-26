log_parser.out: main.c str_int_map.o extractors.o
	mpicc main.c str_int_map.o extractors.o -o log_parser.out
	
str_int_map.o: str_int_map.c str_int_map.h
	mpicc str_int_map.c -c

extractors.o: extractors.h extractors.c
	mpicc extractors.c -c
clean:
	rm *.o *.out
