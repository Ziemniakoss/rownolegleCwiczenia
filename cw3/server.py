import multiprocessing
import sys
from multiprocessing.managers import BaseManager
from multiprocessing.queues import Queue


class BlockedQueue(Queue):
	def __init__(self, maxsize=-1, block=True, timeout=None):
		self.block = block
		self.timeout = timeout
		super().__init__(maxsize, ctx=multiprocessing.get_context())


if __name__ == '__main__':
	if len(sys.argv) != 3:
		print(f'{sys.argv[0]} [nr portu] [klucz')
		exit(1)
	in_queue = BlockedQueue()
	out_queue = BlockedQueue()
	port = int(sys.argv[1])
	manager = BaseManager(address=("localhost", port), authkey=bytes(sys.argv[2], encoding="utf8"))
	manager.register('in_queue', callable=lambda: in_queue)
	manager.register('out_queue', callable=lambda: out_queue)
	print("Startuje serwer na porcie ", port)
	manager.get_server().serve_forever()
