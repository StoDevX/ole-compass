#include "Neighbor.h"

const double max_weight = 5000000;

Neighbor::Neighbor(int arg_target, double arg_weight)
  : target(arg_target), weight(arg_weight) {
  visible=false;
  loc.x=loc.y=0;
  id[0]=id[1]='0';
  id[2]=0;
}

Neighbor::Neighbor(){
  target = 0;
  weight = 0;
  id[0]=id[1]='0';
  id[2]=0;
  loc.x = loc.y = 0;
  visible = false;
}


void Neighbor::setLocation(int x, int y) {
  loc.x = x;
  loc.y = y;
}

void Neighbor::setID(const char *a){
  id[0]=a[0];
  id[1]=a[1];
  id[2]=a[2];
  id[3]=0;
}

void Neighbor::setVisibility(bool vis){
  visible = vis;
}

bool Neighbor::getVisibility(){
  return visible;
}

char * Neighbor::getID(){
  return id;
  }

Point Neighbor::getLocation(){
  return loc;
 }


void Neighbor::DijkstraComputePaths(int source,const adjacency_list_t &adjacency_list,vector<double> &min_distance,vector<int> &previous){
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    set<pair<double, int> > vertex_queue;
    vertex_queue.insert(make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        double dist = vertex_queue.begin()->first;
        int u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
	const vector<Neighbor> &neighbors = adjacency_list[u];
        for (vector<Neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
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


list<int> Neighbor::DijkstraGetShortestPathTo(
    int vertex, const vector<int> &previous)
{
    list<int> path;
    for ( ; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}


bool Neighbor::onNode(int x, int y, int ShiftFactorX, int ShiftFactorY)
{

  Point a;
  a.x = x-ShiftFactorX;
  a.y = y-ShiftFactorY;
  double radius = 12;
  double dist = a.distance(loc);
  return dist < radius;

  /*
	return x >= loc.x+ShiftFactorX  &&
	  y >= loc.y+ShiftFactorY && x <= loc.x+ShiftFactorX + 10 &&
	  y <= loc.y+ShiftFactorY + 10;*/
}

void Neighbor::setNodeIsPressed(bool newVal)
{
	NodeIsPressed = newVal;
}

void Neighbor::setoverNode(bool newVal)
{
	overNode = newVal;
}

bool Neighbor::getNodeIsPressed()
{
	return NodeIsPressed;
}







/*

void Neighbor::display()
{
  glColor3f(1., 0., 0.);
  const int NUM_DIVS = 50;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(loc.x, loc.y);
  for ( int i=0; i< NUM_DIVS; ++i)
  {
    glVertex2d(loc.x+5*cos(i*2*M_PI/(NUM_DIVS-1)),
	       loc.y+5*sin(i*2*M_PI/(NUM_DIVS-1)));
  }
  glEnd();
}

*/

