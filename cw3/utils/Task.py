class Task:
	"""
	Zawiera macierz którą trzeba pomnożyć przez wektor.
	id to unikalny numer przypisany do zadania pamagający połączyć podwyniki z
	onnych procesów
	"""

	def __init__(self, id: int, A: list[list[float]], B: list[list[float]]):
		"""
		:param id: unikalne id zadania
		:param A: macierz do przemnożenia
		:param B: wektor do przemnożenia
		"""
		self.id = id
		self.A = A
		self.B = B
