#ifndef _STR_INT_MAP_H_
#define _STR_INT_MAP_H_

/*
 * Mapa z kluczami typu string i wartościami long
 */
typedef struct map_t{
	int size;
	int maxIndex;
	char ** strings;
	int * values;
} map_t;

/*
 * Zwraca index danego słowa w mapie. Gdy takiego słowa
 * nie ma zwraca -1
 */
int getIndex(struct map_t * map, char * word);

/*
 * Dodaje słowo do mapy ale tylko, gdy jeszcze takiego 
 * słowa nie ma. Gdy takie słowo już istnieje, zwalnia pamięć
 * pod word
 *
 * Zwraca indeks nowego(lub istniejacego) słowa
 * lub -1 gdy nie udało się dodać nowego słowa(Np problemy 
 * z alokacją pamięci)
 */
int add(struct map_t * map, char * word);

/*
 * Zwalnia miejsce zajmowane przez mapę
 */
void free_map(struct map_t * map);

/*
 * Dokonuje redukcji map. Druga mapa jest opisana przez dwie tablice 
 * i jednego integera:
 * - indexes to indeksy słów w pierwszej mapie
 * - values to wartości przypisane tym słowom
 * - n to wielkość mapy
 *
 * Podczas redukcji dodawane są wartości dla danego indeksu w mapie 1.
 */
void reduce(struct map_t * map, int * indexes, int * values, int n);

/*
 * Drukuje dane o mapie na konsolę
 */
void print(struct map_t * map);

/*
 * Inicjalizuje mapę o podanym rozmiarze.
 * Zwraca liczbę ujemną w przypadku błędu.
 */
int init_map(struct map_t * map, int size);
#endif
