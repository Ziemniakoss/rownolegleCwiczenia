from multiprocessing.managers import BaseManager
import multiprocessing
from multiprocessing.queues import Queue


class TwoWayQueue:
	def __init__(self, host: str, port: int, authkey: bytes):
		self.__inQueue = BlockedQueue()
		self.__outQueue = BlockedQueue()
		in_queue = BlockedQueue()
		out_queue = BlockedQueue()
		manager = BaseManager(address=(host, port), authkey=authkey)
		manager.register('in_queue', callable=lambda: in_queue)
		manager.register('out_queue', callable=lambda: out_queue)
		self.__manager = manager
		manager.get_server().serve_forever()

	def getInputQueue(self):
		return self.__inQueue

	def getOutputQueue(self):
		return self.__outQueue

	def stop(self):
		self.__manager.shutdown()


class BlockedQueue(Queue):
	def __init__(self, maxsize=-1, block=True, timeout=None):
		self.block = block
		self.timeout = timeout
		super().__init__(maxsize, ctx=multiprocessing.get_context())
