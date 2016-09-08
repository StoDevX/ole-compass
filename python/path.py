from point import distance
from map import Map
from copy import deepcopy
from helpers import draw_rectangle

class Path(Map):
  def __init__(self):
    self.distance = 0
    self.path_size = 0


  def fillPath(self, m, route):
    self.path_size = len(route)
    nodes = [Neighbor() for _ in range(self.path_size)]
    for i in range(self.path_size):
      nodes[i] = m[route[i]]
    self.distance = 0
    for i in range(self.path_size - 1):
      self.distance += distance(nodes[i].location, nodes[i + 1].location)


  def emptyPath(self):
    self.path_size = 0
    self.distance = 0


  def display(self):
    print("Distance from " , nodes[0].getID() , " to "
              , nodes[self.path_size - 1].getID() , ": ")
    print(self.distance )
    print("Route: ")
    for node in nodes:
      print(node.getID(), end=' ')


  def draw(self):
    if self.path_size is 0:
      self.path_size += 1
    for i in range(self.path_size - 1):
      one = nodes[i].location
      two = nodes[i + 1].location
      draw_rectangle(one, two, 3, 0)
