import sys
import random

print(sys.argv)
if len(sys.argv) != 3:
    print("program [nazwa_pliku] [dlugosc wektora]")

numbers = int(sys.argv[2])
f = open(sys.argv[1], "w")
f.write(str(numbers))

hek = '{0:.2f}'.format(20.2)
for i in range(numbers):
    f.write(" {0:.16f}".format(random.uniform(-500, 500)))
f.close()
