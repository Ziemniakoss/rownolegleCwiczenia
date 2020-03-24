from multiprocessing.managers import BaseManager


class QueueManager(BaseManager):
	pass


if __name__ == '__main__':
	QueueManager.register('in_queue')
	m = QueueManager(address=('localhost', 5000), authkey=b'blah')
	m.connect()
	queue = m.in_queue()
	while True:
		queue.put(input())
