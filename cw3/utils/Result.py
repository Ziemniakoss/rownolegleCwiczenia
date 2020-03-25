from typing import List


class Result:
	"""
	Zawiera rozwiązanie podzadania zdefiniowanego przez Task.
	i to odpowidnik indeksu w słowniku task.ranges
	a result to wektor wynikowy
	"""

	def __init__(self, i: int, result: List[float]):
		self.i = i
		self.result = result
