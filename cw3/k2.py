from multiprocessing.managers import BaseManager
import pickle


class QueueManager(BaseManager):
	pass


QueueManager.register('in_queue')
m = QueueManager(address=('localhost', 5000), authkey=b'hek')
m.connect()
queue = m.in_queue()

print(pickle.loads(queue.get())[4].id)
print(type(queue))
