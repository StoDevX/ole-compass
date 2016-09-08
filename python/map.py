from neighbor import Neighbor
from collections import defaultdict
from point import distance
from helpers import draw_text, draw_rectangle
from OpenGL.GL import glColor3f, glBegin, glEnd, glVertex2d, GL_TRIANGLE_FAN
from math import cos, sin, pi

NUM_DIVS = 50

class Map(object):
  def __init__(self):
    self.size = 0
    self.nodes = []#defaultdict(Neighbor)
    self.dist_matrix = defaultdict(list)


  def __getitem__(self, key):
    return self.nodes[i]


  def load(self, filename):
    with open(filename, 'r', encoding='utf-8') as infile:
      size = int(infile.readline())
      self.nodes = [Neighbor() for _ in range(size)]

      for i in range(size):
        splits = infile.readline().split()

        buffer = splits[0]
        self.nodes[i].id = buffer
        x = int(splits[1])
        y = int(splits[2])
        self.nodes[i].location = (x, y)

        connections = splits[4:]
        vertices = connections[0::2]
        weights = connections[1::2]
        for vertex, weight in zip(vertices, weights):
          self.dist_matrix[i].append(Neighbor(vertex, weight))


  def load_map_calc(self, filename):
    with open(filename, 'r', encoding='utf-8') as infile:
      size = int(infile.readline())
      self.nodes = [Neighbor() for _ in range(size)]

      for i in range(size):
        splits = infile.readline().split()
        buffer = splits[0]
        self.nodes[i].id = buffer
        x = int(splits[1])
        y = int(splits[2])
        self.nodes[i].location = (x, y)
        visib = bool(splits[3])
        self.nodes[i].visible = visib

      infile.readline()

      for i in range(size):
        splits = infile.readline().split()
        buffer = splits[0]
        a = self.nodes[i].location
        connections = splits[2:]
        vertices = [int(v) for v in connections[0::2]]
        _ = connections[1::2]
        for vertex in vertices:
          b = self.nodes[vertex].location
          dist = distance(a, b)
          self.dist_matrix[i].append(Neighbor(vertex, dist))


  def load_map_calc_weighted(self, filename, outdoor, stairs, blacktop, paths):
    with open(filename, 'r', encoding='utf-8') as infile:
      size = int(infile.readline())
      self.nodes = [Neighbor() for _ in range(size)]

      for i in range(size):
        splits = infile.readline().split()
        buffer = splits[0]
        self.nodes[i].id = buffer
        x = int(splits[1])
        y = int(splits[2])
        self.nodes[i].location = (x, y)
        visib = bool(splits[3])
        self.nodes[i].visible = visib

      infile.readline()

      for i in range(size):
        splits = infile.readline().split()
        buffer = splits[0]
        a = self.nodes[i].location
        connections = splits[2:]
        vertices = [int(v) for v in connections[0::2]]
        connectionTypes = [int(t) for t in connections[1::2]]
        for vertex, connect_type in zip(vertices, connectionTypes):
          b = self.nodes[vertex].location
          dist = distance(a, b)

          if outdoor and connect_type is 0:
            dist *= 2
          if stairs and connect_type is 2:
            dist *= 100
          if blacktop and connect_type is 3:
            dist *= 20
          if paths and connect_type is 4:
            dist *= 30

          self.dist_matrix[i].append(Neighbor(vertex, dist))


  def draw_nodes(self, drawAll, begin, end):
    glColor3f(1, 1, 1)
    radius = 12
    for i, node in enumerate(self.nodes):
      if node.visible or drawAll:
        glColor3f(.4, .4, .8);
        if i is begin or i is end:
          glColor3f(0.2, 0.95, 0.7)

        x, y = node.location
        glBegin(GL_TRIANGLE_FAN)
        glVertex2d(x, y)
        for i in range(NUM_DIVS):
          glVertex2d(x + radius * cos(i * 2 * pi / (NUM_DIVS - 1)),
                     y + radius * sin(i * 2 * pi / (NUM_DIVS - 1)))
        glEnd()

        glColor3f(0, 0, 0)
        draw_text(x - 8, y + 6, node.id)


  def draw_connections(self, filename):
    m = Map()
    with open(filename, 'r', encoding='utf-8') as infile:
      glColor3f(1., 0., 0.)
      size = int(infile.readline())
      m.nodes = [Neighbor() for _ in range(size)]

      for i in range(size):
        splits = infile.readline().split()
        buffer = splits[0]
        m.nodes[i].id = buffer
        x = int(splits[1])
        y = int(splits[2])
        m.nodes[i].location = (x, y)
        visib = bool(splits[3])
        m.nodes[i].visible = visib

      infile.readline()

      for i in range(size):
        splits = infile.readline().split()
        buffer = splits[0]
        one = m.nodes[i].location
        connections = splits[2:]
        vertices = [int(v) for v in connections[0::2]]
        _ = connections[1::2]
        for vertex in vertices:
          two = m.nodes[vertex].location
          draw_rectangle(one, two, 1, connectType)


  def equate_nodes(self, other):
    for i in range(self.size):
      other[i] = nodes[i]
    return other
