import sys
from utils.queues import TwoWayQueue


if len(sys.argv) != 3:
	print(f'{sys.argv[0]} [nr portu] [klucz')
	exit(1)

port = int(sys.argv[1])
print("Startuje serwer na porcie ", port)
TwoWayQueue("localhost", port, bytes(sys.argv[2], encoding="utf8"))
