import random
import sys

"""
Program do generacji danych wejściwoych dla client.py
"""
if len(sys.argv) != 5:
	print(f'{sys.argv[0]} [kolumny macierz/ dlugosc wektora] [wiersze macierzy] [plik na macierz] [plik na wektor]')
	exit(1)

columns = int(sys.argv[1])
rows = int(sys.argv[2])

# Tworzymy plik z macierza
matrixFIle = open(sys.argv[3], "w")
matrixFIle.write(f'{rows}\n')
matrixFIle.write(f'{columns}\n')
for row in range(rows):
	for column in range(columns):
		matrixFIle.write("{0:.16f}\n".format(random.uniform(-500, 500)))
matrixFIle.close()

# Tworzymy plik z wektorem
vectorFile = open(sys.argv[4], "w")
vectorFile.write(f'{columns}\n')
for column in range(columns):
	vectorFile.write("{0:.16f}\n".format(random.uniform(-500, 500)))
vectorFile.close()
