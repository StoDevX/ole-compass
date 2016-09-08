#include "Neighbor.h"

const double max_weight = 5000000;

Neighbor::Neighbor(int arg_target, double arg_weight)
    : target(arg_target), weight(arg_weight) {
  visible = false;
  loc.x = loc.y = 0;
  id = "00";
}

Neighbor::Neighbor() {
  target = 0;
  weight = 0;
  id = "00";
  loc.x = loc.y = 0;
  visible = false;
}

void Neighbor::setLocation(int x, int y) {
  loc.x = x;
  loc.y = y;
}

void Neighbor::setID(std::string a) { id = a; }

void Neighbor::setVisibility(bool vis) { visible = vis; }

bool Neighbor::getVisibility() { return visible; }

std::string Neighbor::getID() { return id; }

Point Neighbor::getLocation() { return loc; }

void Neighbor::DijkstraComputePaths(int source,
                                    const adjacency_list_t &adjacency_list,
                                    vector<double> &min_distance,
                                    vector<int> &previous) {
  int n = adjacency_list.size();
  min_distance.clear();
  min_distance.resize(n, max_weight);
  min_distance[source] = 0;
  previous.clear();
  previous.resize(n, -1);
  set<pair<double, int>> vertex_queue;
  vertex_queue.insert(make_pair(min_distance[source], source));

  while (!vertex_queue.empty()) {
    double dist = vertex_queue.begin()->first;
    int u = vertex_queue.begin()->second;
    vertex_queue.erase(vertex_queue.begin());

    // Visit each edge exiting u
    const vector<Neighbor> &neighbors = adjacency_list[u];
    for (vector<Neighbor>::const_iterator neighbor_iter = neighbors.begin();
         neighbor_iter != neighbors.end(); neighbor_iter++) {
      int v = neighbor_iter->target;
      double weight = neighbor_iter->weight;
      double distance_through_u = dist + weight;
      if (distance_through_u < min_distance[v]) {
        vertex_queue.erase(make_pair(min_distance[v], v));

        min_distance[v] = distance_through_u;
        previous[v] = u;
        vertex_queue.insert(make_pair(min_distance[v], v));
      }
    }
  }
}

list<int> Neighbor::DijkstraGetShortestPathTo(int vertex,
                                              const vector<int> &previous) {
  list<int> path;
  for (; vertex != -1; vertex = previous[vertex])
    path.push_front(vertex);
  return path;
}

bool Neighbor::onNode(int x, int y, int ShiftFactorX, int ShiftFactorY) {
  Point a;
  a.x = x - ShiftFactorX;
  a.y = y - ShiftFactorY;
  double radius = 12;
  double dist = a.distance(loc);
  return dist < radius;
}

void Neighbor::setNodeIsPressed(bool newVal) { NodeIsPressed = newVal; }

void Neighbor::setoverNode(bool newVal) { overNode = newVal; }

bool Neighbor::getNodeIsPressed() { return NodeIsPressed; }
