import math
import pickle
import sys
import time
from multiprocessing.managers import BaseManager
from multiprocessing.queues import Queue
from typing import List

from utils.Result import Result
from utils.Task import Task


def readMatrix(filename: str) -> List[List[float]]:
	file = open(filename)
	rows = int(file.readline())
	columns = int(file.readline())
	if rows <= 0 or columns <= 0:
		raise Exception("Macierz musi mieć przynajmniej jeden rząd i kolumnę")
	matrix: List[List[float]] = []

	for i in range(rows):
		matrix.append([])
		for j in range(columns):
			matrix[i].append(float(file.readline()))
	return matrix


def readVector(filename: str) -> List[float]:
	file = open(filename)
	length = int(file.readline())
	r = []
	for i in range(length):
		r.append(float(file.readline()))
	return r


if __name__ == '__main__':
	if len(sys.argv) != 7:
		print("[macierz plik] [wektor plik] [serverIp] [port] [Na ile podzielic] [haslo]")
		exit(1)

	timers = {}
	start = time.time()
	print("Wczytujemy macierze")
	matrix = readMatrix(sys.argv[1])
	vector = readVector(sys.argv[2])
	# czy można pomnożyć?
	if len(vector) != len(matrix[0]):
		print(len(matrix), len(matrix[0]))
		raise Exception("Niepoprawne rozmiary macierzy")

	jobs = max(int(sys.argv[5]), 1)
	elementsOfVector = math.ceil(1.0 * len(matrix) / jobs)
	print(f'dzielimy wektor na {jobs} części po max {elementsOfVector} elementów')

	ranges = {}
	i = 0
	index = 0
	while index < len(matrix):
		ranges[i] = (index, index + elementsOfVector)
		i += 1
		index += elementsOfVector
	timers["Wczytywanie macierzy i jej dzielenie"] = time.time() - start

	# Wysyłamy
	start = time.time()
	print("Wysyłam dane")
	task = Task(ranges, matrix, vector)
	m = BaseManager(address=(sys.argv[3], int(sys.argv[4])), authkey=bytes(sys.argv[6], encoding="utf8"))
	m.register("in_queue")
	m.register("out_queue")
	m.connect()
	q: Queue = m.in_queue()
	q.put(pickle.dumps(task))
	timers["Wysyłanie"] = time.time() - start


	# Czekamy na wyniki
	print("Wysłano, czekam na wyniki")
	start = time.time()
	result = [0.0] * len(matrix)
	q = m.out_queue()
	while i > 0:
		r: Result = pickle.loads(q.get())
		rr = task.ranges[r.i]
		result[rr[0]:rr[1]] = r.result
		i -= 1
	timers["Zbieranie wyników"] = time.time() - start
	print(result)
	for key in timers:
		print(f'{key}: {timers[key]} s')
