import pickle
import sys
from multiprocessing.managers import BaseManager
from multiprocessing.queues import Queue

from utils.Result import Result
from utils.Task import Task

def solve(task:Task) -> Result:
	a = task.A
	b = task.B
	result = []
	for row in range(len(a)):
		r = []
		for column in range(len(b[0])):
			for i in range(len(b)):
				r.append(a[])



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
	print(task.ranges)
