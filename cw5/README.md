# Całkowanie z wykorzystaniem MPI

Trzy programy liczące całkę z wykorzystanniem innych rodzajów
komuikacji:
 - blokującej (send_recv.c)
 - nieblokującej (isend_irecv.c)
 - grupowej (groups.c)

## Kompilacja

Do kopilowania można użyć Makefile:
```bash
make
```

## Uruchamianie

Wszystkie programy mają taką samą listę parametrów wywołania:
 - start przedziału całkowania
 - koniec przedziału całkowania
 - liczba podprzedziałów
 - kod metody

Kod metody to jedna z poniższych liczb:
 - 1 dla metody prostokątów
 - 2 dla metody trapezów
 - 3 dla metody Simsona

### Przykłądowe wywołania

```bash
mpiexec isend_irecv.out 0 2000 2000 2
```

```bash
mpiexec send_recv.out 0 2000 2000 3
```

```bash
mpiexec groups.out 0 100 5000 1
```
