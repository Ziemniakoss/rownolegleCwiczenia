import multiprocessing.pool

with multiprocessing.Pool(16) as pool:
	print(pool.map(print, [1, 2, 3]))
