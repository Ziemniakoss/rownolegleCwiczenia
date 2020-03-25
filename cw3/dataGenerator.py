import random
import sys

if len(sys.argv) != 5:
	print(f'{sys.argv[0]} [kolumny macierz/ dlugosc wektora] [wiersze macierzy] [plik na macierz] [plik na wektor]')

columns = int(sys.argv[1])
rows = int(sys.argv[2])
matrixFIle = open(sys.argv[3], "w")
vectorFile = open(sys.argv[4], "w")
matrixFIle.write(f'{rows}\n')
matrixFIle.write(f'{columns}\n')
vectorFile.write(f'{columns}\n')

for row in range(rows):
	for column in range(columns):
		matrixFIle.write("{0:.16f}\n".format(random.uniform(-500, 500)))
matrixFIle.close()
for column in range(columns):
	vectorFile.write("{0:.16f}\n".format(random.uniform(-500, 500)))
vectorFile.close()
