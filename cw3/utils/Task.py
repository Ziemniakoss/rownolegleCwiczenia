from typing import List


class Task:
	"""
	Reprezentuje pelne zadnaie do obliczenia.
	Zawiera pełną macierz, wektor przez którą trzeba ją pomnożyć
	oraz zakres indeksów podzadań w postaci słownika:
		- klucz to numer zadania
		- wartość to para indeksu początkowego i końcowego.
	Obydwa indeksy są włączne. Oznacza to że słownik
	{ 0: (0,10), 1(11,20)}
	zawiera dwa podzadania, gdzie pierwsze to policzenie iloczynu podmacierzy A
	od rzędu 0 włącznie do 10 włącznie przez wektor, a drugie to policzenie iloczynu podmacierzy A
	od rzędu 11 do 20 przez wektor
	"""

	def __init__(self, ranges: dict, a: List[List[float]], vector: List[float]):
		self.a = a
		self.ranges = ranges
		self.vector = vector
