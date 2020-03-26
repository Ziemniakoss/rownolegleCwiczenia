import pickle
import sys
from multiprocessing.managers import BaseManager
from multiprocessing.queues import Queue
import multiprocessing
from typing import List, Tuple

from utils.Result import Result
from utils.Task import Task


def solve(subtask: Tuple[int, List[List[float]], List[float]]) -> Result:
	"""
	Rozwiązuje podzadanie(mnoży macierz przez wektor)
	:param subtask: jest to tuple zawierający:
		- indeks podzadania. Odpowiada indeksowi zasięgu w Task.ranges
		- macierz do przemnożenia(już przycięta)
		- wektor przez któ©y należy tą macierz przemnożyć
	:return: rozwiązanie podzadania
	"""
	index = subtask[0]
	a = subtask[1]
	v = subtask[2]
	result = [0.0] * len(a)

	for row in range(len(a)):
		for i in range(len(v)):
			result[row] = a[row][i] * v[i]
	return Result(index, result)


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
		subtasks = []
		for key in task.ranges:
			r = task.ranges[key]
			subMatrix = task.a[r[0]:r[1]]
			subTask = (key, subMatrix, task.vector)
			subtasks.append(subTask)
		print(f'Rozwiazuje {len(subtasks)} podzadan')
		with multiprocessing.Pool() as pool:
			r = pool.map(solve,subtasks)
		for x in r:
			outq.put(pickle.dumps(x))
		print("Skonczone")
