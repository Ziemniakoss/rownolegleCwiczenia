all: send_recv.out isend_irecv.out groups.out
	 
utils.o: utils.c utils.h
	mpicc utils.c -c

integrals.o: integrals.h integrals.c
	mpicc integrals.c -c

send_recv.out: send_recv.c integrals.o utils.o
	mpicc send_recv.c integrals.o utils.o -o send_recv.out

isend_irecv.out: isend_irecv.c integrals.o utils.o
	mpicc isend_irecv.c integrals.o utils.o -o isend_irecv.out

groups.out: groups.c integrals.o utils.o
	mpicc groups.c integrals.o utils.o -o groups.out

clean:
	rm *.out
	rm *.o
