# Obliczenia na wielu rdzeniach

Testowanie biblioteki multiprocessing. Celem jest obliczenie iloczynu macierzy i wektora na
wielu maszynach z wykorzystaniem wielu procesów
 

## Serwer

Serwer jedynie udostęþnia dwie kolejki za pomocą, 
których klient komunikuje się z progreamem obliczającym iloczyn macierzy i wektora.

## Klient

Klient wczytuje z dysku dane macierzy i wektora, dzieli je na określoną liczbę zadań i wysyłą poprzez kolejkę
udostępnioną przez serwer do programu obliczającego iloczyn. Po otrzymaniu wyników, scala je w jedno rozwiązanie
i wypisuje statystyki działąnia programu

## Worker

Program, który łączy się z serwerem i czeka na dane o problemie do rozwiązania. 
Gdy dane otrzyma, za pomocą wielu procesów oblicza rozwiązanie i wysyła je do klienta poprzez
kolejkę udostępnioną przez serwer