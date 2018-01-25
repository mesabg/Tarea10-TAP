from sys import stdin

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    # getters && setters
    def getX(self):
        return self.x

    def getY(self):
        return self.y

    def setX(self, x):
        self.x = x

    def setY(self, y):
        self.y = y

    def __str__(self):
        return "({}, {})".format(self.x, self.y)




# Main
[nConjunto1] = list(map(int,stdin.readline().strip().split()))
conjunto1 = []
for i in range(nConjunto1):
    [x, y] = list(map(int,stdin.readline().strip().split()))
    conjunto1.append(Point(x, y))

[nConjunto2] = list(map(int,stdin.readline().strip().split()))
conjunto2 = []
for i in range(nConjunto2):
    [x, y] = list(map(int,stdin.readline().strip().split()))
    conjunto2.append(Point(x, y))

# Do some stuff