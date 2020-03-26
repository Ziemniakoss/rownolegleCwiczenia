from multiprocessing.managers import BaseManager


class QueueManager(BaseManager):
	pass


if __name__ == '__main__':
	QueueManager.register('out_queue')
	m = QueueManager(address=('localhost', 5000), authkey=b'hek')
	m.connect()
	queue = m.out_queue()
	while True:
		queue.put(input())
