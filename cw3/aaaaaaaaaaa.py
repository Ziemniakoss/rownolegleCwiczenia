# import math
#
# matrix = [i for i in range(200)]
# parts = 20
# elemtnsInPart = math.ceil(1.0 * len(matrix) / parts)
# index = 0
# while index < len(matrix):
# 	print(matrix[index:index + elemtnsInPart])
# 	index += elemtnsInPart
import pickle

from cw3.client import Task

out = open("a.p", "wb")
pickle.dump(Task(1, [[1], [2], [3]], [[1], [4], [5]]), out)
out.close()
a = pickle.load(open("a.p", "rb"))
print(a.A)