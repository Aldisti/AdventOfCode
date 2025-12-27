
import matplotlib.pyplot as plt

with open("input.txt", 'r') as f:
        lines = f.readlines()

def plot(points) -> None:
    fig = plt.figure()
    ax = plt.axes(projection='3d')

    data = [[], [], []]

    for point in points:
        for i, n in enumerate(point):
            data[i].append(n)

    ax.scatter(data[0], data[1], data[2])
    plt.show()

def distance(a: list[int], b: list[int]) -> int:
    sum = 0
    for a_n, b_n in zip(a, b):
        sum += pow(a_n - b_n, 2)
    return sum

points = [[int(num) for num in line.split(",")] for line in lines]

x = list(map(lambda x : x[1], sorted(map(lambda p : (distance(p, [0, 0, 0]), p), points), key=lambda x : x[0])))

plot(x)

