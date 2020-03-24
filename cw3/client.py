import math
import pickle
import sys
from multiprocessing.managers import BaseManager
from typing import List

from utils.Result import Result
from utils.Task import Task


def readMatrix(filename: str) -> List[List[float]]:
	file = open(filename)
	rows = int(file.readline())
	columns = int(file.readline())
	if rows <= 0 or columns <= 0:
		raise Exception("matrix must have at least 1 row and column")
	matrix: List[List[float]] = []

	for i in range(rows):
		matrix.append([])
		for j in range(columns):
			matrix[i].append(float(file.readline()))
	return matrix


if __name__ == '__main__':
	if len(sys.argv) != 5:
		print("[macierz plik] [wektor plik] [serverIp] [Na ile podzielic]")
		exit(1)
	print("Wczytujemy macierze")
	matrix = readMatrix(sys.argv[1])
	vector = readMatrix(sys.argv[2])
	# can multiply?
	if len(vector) != len(matrix[0]):
		print(len(matrix), len(matrix[0]))
		raise Exception("Niepoprawne rozmiary macierzy")

	jobs = max(int(sys.argv[4]), 1)
	elementsOfVector = math.ceil(1.0 * len(matrix) / jobs)
	print(f'dzielimy wektor na {jobs} części po max {elementsOfVector} elementów')

	tasks = []
	index = 0
	i = 0
	while index < len(vector):
		task = Task(i, matrix[index:index + elementsOfVector], vector)
		index += elementsOfVector
		i += 1
		tasks.append(task)

	print("Wysyłam dane")
	serialized = pickle.dumps(tasks)

	m = BaseManager(address=(sys.argv[3], 5000), authkey=b'blah')
	m.register("in_queue")
	m.register("out_queue")
	m.connect()
	q = m.in_queue()
	q.put(serialized)
	print("Wysłano, czekam na wyniki")
	i = len(tasks)
	results = {}
	q = m.out_queue()
	while i > 0:
		r: Result = pickle.loads(q.get)
		results[r.id] = r.result
	print(results)
