#ifndef vector_h_included
#define vector_h_included
void print_vector(double * vector, int n);

double * read_vector_from_file(char * file, int * n);

int get_index(pid_t pid, pid_t * array, int n);
#endif
