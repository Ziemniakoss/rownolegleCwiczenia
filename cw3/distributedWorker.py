import pickle
import sys
import time
from multiprocessing.managers import BaseManager
from multiprocessing.pool import Pool
from multiprocessing.queues import Queue
from typing import List, Tuple

from utils.Result import Result
from utils.Task import Task

"""
Program przystosowany do obliczeń rozproszonych. Wykorzystuje 
wiele procesów do obliczeń
"""


def solve(index: int, a: List[List[float]], v: List[float]) -> Result:
	"""
	Rozwiązuje podzadanie(mnoży macierz przez wektor)
	:param index: unikalny indeks zadania
	:param v: wektor
	:param a: macierz
	:return: rozwiązanie podzadania
	"""
	with Pool() as pool:
		r = pool.map(dot, [(a[i], v) for i in range(len(a))])
		return Result(index, r)


def dot(parameters: Tuple[List[float], List[float]]) -> float:
	a = parameters[0]
	b = parameters[0]
	r = 0
	for i in range(len(a)):
		r += a[i] * b[i]
	return r


if __name__ == '__main__':
	if len(sys.argv) != 4:
		print(f'{sys.argv[0]} [ip serwera] [port serwera] [haslo]')
		exit(1)

	m = BaseManager(address=(sys.argv[1], int(sys.argv[2])), authkey=bytes(sys.argv[3], encoding="utf8"))
	m.register("in_queue")
	m.register("out_queue")
	m.connect()
	inq: Queue = m.in_queue()
	outq: Queue = m.out_queue()

	while True:
		task: Task = pickle.loads(inq.get())
		start = time.time()
		result = solve(task.index, task.a, task.vector)
		outq.put(pickle.dumps(result))
		print(f'Obliczenia skonczone w {time.time() - start} sekund, wysyłam')
