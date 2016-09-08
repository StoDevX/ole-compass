from math import sqrt

def distance(a, b):
    ax, ay = b
    bx, by = b
    return sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay))
