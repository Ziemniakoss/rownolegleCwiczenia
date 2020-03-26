from typing import List


class Task:
	"""
	Reprezentuje pojedyncze zadanie do wykonania. Zawiera część pełnej macierzy i wektor do rpzemnożenia
	oraz unikalne id zadania
	"""
	def __init__(self, index: int, a: List[List[float]], vector: List[float]):
		self.index = index
		self.a = a
		self.vector = vector
