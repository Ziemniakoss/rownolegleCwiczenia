# MapReduce w MPI

Program do robienia statystyk plików z logami.

## Obsługiwane tryby

Program tworzy statystyki na 3 sposoby:
- addr: który adres ip wysłał ile zapytań
- stat: Ile razy został zwrócony dany status http
- time: Ile zapytań było wysyłane w danym czasie(z dokładnością do sekundy)

## Budowanie i uruchamianie

Do zbudowania programu potrzebna jest zainstalowane biblioteka mpi. Program można łatwo zbudować z użyciem make
```bash
make
```

Następnie można go uruchomić za pomocą komendy
```bash
mpiexec log_parser.out [opcja] [log]
```


### Przykładowe wywołania
Zliczanie adresów ip:
```bash
mpiexec log_parser.out -addr a2.log
```

Zliczanie statusów http:
```bash
mpiexec log_parser.out -stat a2.log
```

Sprawdzanie ilości zdarzeń w czasie z dokładnością do sekundy
```bash
mpiexec log_parser.out -time a2.log
```
## Wprowadzanie zmian

W celu wprowadzenia nowego rodzaju statystyk należy:
- dodać metodę do wydobywania potrzebnych danych do pliku extractors.h
- zaimplementować tą metodę w extracotrs.c
- zdefiniować zmienną MODE_NAZWA_TRYBU w extractors.h. 
- w main.c przy sprawdzaniu parametru przypisać ciąg znaków odpowiadający tej zmiennej(np dla MODE_ADDR jest to "addr"). Dzięki temu będzie można łatwo wywoływać program z nowym trybem.

W celu zmiany formatu pliku z logami należy w extractors.c zreimplementować metody do extrakcji danych. 
