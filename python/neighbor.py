from point import distance

class Neighbor(object):
  max_weight = 5000000

  def __init__(self, target=0, weight=0):
    self.target = target
    self.weight = weight
    self.visible = False
    self.over_node = False
    self.node_is_pressed = False
    self.location = (0, 0)
    self.id = '00'


  def dijkstra_compute_paths(self, source, adjacency_list):
    n = len(adjacency_list)

    min_distance = [Neighbor.max_weight for _ in range(n)]
    min_distance[source] = 0

    previous = [-1 for _ in range(n)]

    vertex_queue = set()
    vertex_queue.add((min_distance[source], source))

    while vertex_queue:
      dist, u = vertex_queue.pop()
      vertex_queue.remove((dist, u))

      # Visit each edge exiting u
      neighbors = adjacency_list[u]
      for neighbor in neighbors:
        v = neighbor_iter.target
        weight = neighbor.weight;
        distance_through_u = dist + weight
        if distance_through_u < min_distance[v]:
          vertex_queue.discard((min_distance[v], v))

          min_distance[v] = distance_through_u
          previous[v] = u
          vertex_queue.add((min_distance[v], v))

    return min_distance, previous


  def dijkstra_get_shortest_path_to(self, vertex, previous):
    path = []
    while vertex != -1:
      path.append(vertex)
      vertex = previous[vertex]
    return reversed(path)


  def on_node(self, x, y, shift_factor_x, shift_factor_y):
    radius = 12
    dist = distance((x - shift_factor_x, y - shift_factor_y), self.location)
    return dist < radius
