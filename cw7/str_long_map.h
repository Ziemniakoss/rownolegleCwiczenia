#ifndef _STR_LONG_MAP_H_
#define _STR_LONG_MAP_H_

/*
 * Mapa z kluczami typu string i wartościami long
 */
typedef struct sl_map_t{
	int size;
	int maxIndex;
	char ** strings;
	long * values;
} sl_map;

/*
 * Zwraca index danego słowa w mapie. Gdy takiego słowa
 * nie ma zwraca -1
 */
int getIndex(struct sl_map_t * map, char * word);

/*
 * Dodaje słowo do mapy ale tylko, gdy jeszcze takiego 
 * słowa nie ma. Zwraca indeks nowego(lub istniejacego) słowa
 * lub -1 gdy nie udało się dodać nowego słowa(Np problemy 
 * z alokacją pamięci)
 */
int add(struct sl_map_t * map, char * word);

/*
 * Zwalnia miejsce zajmowane przez mapę
 */
void free_map(struct sl_map_t * map);

/*
 * Tworzy mapę o określonym rozmairze. Zwraca
 * NULL gdy nie udało się zarezerwować pamieci lub podany 
 * rozmiar był mniejszy lub równy 0
 */
struct sl_map_t * create_map(int size);

/*
 * Dokonuje redukcji map. Druga mapa jest opisana przez dwie tablice 
 * i jednego integera:
 * - indexes to indeksy słów w pierwszej mapie
 * - values to wartości przypisane tym słowom
 * - n to wielkość mapy
 *
 * Podczas redukcji dodawane są wartości dla danego indeksu w mapie 1.
 */
void reduce(struct sl_map_t * map, int * indexes, long * values, int n);

/*
 * Drukuje dane o mapie na konsolę
 */
void print(struct sl_map_t * map);
#endif
