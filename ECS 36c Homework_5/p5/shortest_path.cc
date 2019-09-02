#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <cmath>

#include "index_min_pq.h"

class Edge {
 public:
  Edge(unsigned int dest, double weight);
  unsigned int GetEdgeDest(void);
  double GetWeight(void);

  friend std::ostream& operator <<(std::ostream &os, Edge &e);
 private:
  unsigned int edgeDestVertex;
  double weight;
};

class Vertex {
 public:
  void AddEdge(unsigned int edgeDestVertex, double edgeWeight);
  std::vector<Edge> vertex;
  friend std::ostream& operator <<(std::ostream &os, Vertex &v);
};

class Graph {
 public:
  int ExtractFile(const std::string &fileName);
  int GetNumVertices(void);
  std::vector<Vertex>& GetAdjList(void);
  void Print(void);
 private:
  int numVertices = 0;
  int numEdges = 0;

  std::vector<Vertex> AdjList;
};

class ShortestPath {
 public:
  ShortestPath(int sourceVertex, int destVertex);
  void Dijkstra(Graph &g);
  void Print(void);

 private:
  int sourceVertex, destVertex;

  std::vector<int> shortestPath;
  double shortestDistance;
};

Edge::Edge(unsigned int dest, double weight)
    : edgeDestVertex(dest), weight(weight) {}

unsigned int Edge::GetEdgeDest(void) {
  return edgeDestVertex;
}

double Edge::GetWeight(void) {
  return weight;
}

std::ostream& operator <<(std::ostream &os, Edge &e) {
  os << "edgeDestVertex: " << e.edgeDestVertex << " ";
  os << "edgeWeight: " << e.weight;

  return os;
}

std::ostream& operator <<(std::ostream &os, Vertex &v) {
  for (auto edge : v.vertex)
    std::cout << edge << std::endl;

  return os;
}

void Vertex::AddEdge(unsigned int edgeDestVertex, double edgeWeight) {
  Edge edge(edgeDestVertex, edgeWeight);
  vertex.push_back(edge);
}

// Extracts file contents to construct graph
// Returns -1 if file cannot be open or there are input errors
// Returns 0 if file successfully read
int Graph::ExtractFile(const std::string &fileName) {
  std::ifstream myfile(fileName);

  if (myfile.fail()) {
    std::cerr << "Error: cannot open file " << fileName << std::endl;
    return -1;
  }

  unsigned int edgeSourceVertex, edgeDestVertex;
  double edgeWeight;

  if (myfile >> numVertices) {
    Vertex v;
    AdjList.resize(numVertices, v);
  } else {  // There are no vertices in the graph
    std::cerr << "Error: invalid graph size" << std::endl;
    return -1;
  }

  while (myfile >> edgeSourceVertex >> edgeDestVertex >> edgeWeight) {
    if (edgeSourceVertex >= AdjList.size()) {
      std::cerr << "Invalid source vertex number ";
      std::cerr << edgeSourceVertex << std::endl;
      return -1;
    }

    if (edgeDestVertex >= AdjList.size()) {
      std::cerr << "Invalid dest vertex number " << edgeDestVertex << std::endl;
      return -1;
    }

    if (edgeWeight < 0) {
      std::cerr << "Invalid weight " << edgeWeight << std::endl;
      return -1;
    }

    AdjList[edgeSourceVertex].AddEdge(edgeDestVertex, edgeWeight);
    numEdges++;
  }

  myfile.close();

  return 0;
}

int Graph::GetNumVertices(void) {
  return numVertices;
}

std::vector<Vertex>& Graph::GetAdjList(void) {
  return AdjList;
}

void Graph::Print(void) {
  int index = 0;

  for (auto v : AdjList) {
    std::cout << "At vertex " << index << ", adjacent edges are:" << std::endl;
    std::cout << v << std::endl;
    index++;
  }
}

ShortestPath::ShortestPath(int sourceVertex, int destVertex) :
    sourceVertex(sourceVertex),  destVertex(destVertex) {}

// Dijkstra's algorithm for finding shortest path in a graph
void ShortestPath::Dijkstra(Graph &g) {
  IndexMinPQ<double> Q(g.GetNumVertices());

  std::vector<double> dist(g.GetNumVertices(),
    std::numeric_limits<double>::max());

  std::vector<int> prev(g.GetNumVertices(), -1);

  dist[sourceVertex] = 0;

  Q.Push(dist[sourceVertex], sourceVertex);

  while (Q.Size()) {
    int u = Q.Top();
    Q.Pop();

    if (u == destVertex) {
      break;
    }
    for (Edge e : g.GetAdjList()[u].vertex) {
      double alt = dist[u] + e.GetWeight();
      if (alt < dist[e.GetEdgeDest()]) {
        dist[e.GetEdgeDest()] = alt;
        prev[e.GetEdgeDest()] = u;

        if (Q.Contains(e.GetEdgeDest()))
          Q.ChangeKey(dist[e.GetEdgeDest()], e.GetEdgeDest());
        else
          Q.Push(dist[e.GetEdgeDest()], e.GetEdgeDest());
      }
    }
  }

  int u = destVertex;
  if (dist[u] != std::numeric_limits<double>::max()) {
    while (u != -1) {
      shortestPath.push_back(u);
      u = prev[u];
    }
  }
  shortestDistance = dist[destVertex];
}

void ShortestPath::Print(void) {
  std::cout << sourceVertex << " to " << destVertex << ": ";
  if (shortestPath.empty()) {
    std::cout << "no path" << std::endl;
    return;
  }

  for (auto i = shortestPath.size() - 1; i >= 1; i--) {
    std::cout << shortestPath.at(i) << " => ";
  }
  std::cout << shortestPath.front() << " (";
  std::cout << shortestDistance << ')' << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <graph.dat> src dst" << std::endl;
    return 1;
  }

  Graph graph;

  if (graph.ExtractFile(argv[1]) == -1)
    return 1;
  if (static_cast<int>(graph.GetAdjList().size()) <= std::stoi(argv[2])
      || std::stoi(argv[2]) < 0) {
    std::cerr << "Error: invalid source vertex number ";
    std::cerr << std::stoi(argv[2]) << std::endl;
    return 1;
  }
  if (static_cast<int>(graph.GetAdjList().size()) <= std::stoi(argv[3])
      || std::stoi(argv[3]) < 0) {
    std::cerr << "Error: invalid dest vertex number ";
    std::cerr << std::stoi(argv[3]) << std::endl;
    return 1;
  }

  ShortestPath s(std::stoi(argv[2]), std::stoi(argv[3]));

  s.Dijkstra(graph);

  s.Print();

  return 0;
}
