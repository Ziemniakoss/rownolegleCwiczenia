#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "range.h"
#include "vector.h"

void * create_shared_memory(size_t size){
	int protection = PROT_READ | PROT_WRITE; //można zapisywać i odczytywać
	int visiblity = MAP_SHARED | MAP_ANONYMOUS; //tylko procesy potomne będą wiedzieć o pamięci
	return mmap(NULL, size, protection, visiblity, -1, 0);
}
