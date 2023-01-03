#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

#define TESTING true

// using Nodes = std::map<int, int>;
// using Edges = std::map<int, std::vector<int>>;
// using ValveTags = std::map<std::string, int>;

struct ValveInfo {
  std::string name{};
  int flow{};
  std::vector<std::string> neighbours;
};

using Valves = std::vector<ValveInfo>;
using Edge = std::pair<int, int>;

struct VertexData {
  std::string node_name;
  int flow{0};
};

struct EdgeData {
  std::string edge_name;
  double dist;
};

#if TESTING
#define V 10
#else
#define V 58
#endif  // TESTING

void parse_input(Valves& valves) {
  std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
  std::ifstream input{file_name};

  std::string initial_valve{""};

  std::regex full_line{
      R"(Valve ([A-Z]{2}) has flow rate=(\d+); tunnels? leads? to valves? ([A-Z]{2}(?:, [A-Z]{2})*))"};

  while (input && !input.eof()) {
    std::string line{};
    std::getline(input, line);
    std::smatch match;

    if (std::regex_match(line, match, full_line)) {
      ValveInfo valve{};
      valve.name = match[1];
      valve.flow = stoi(match[2]);
      std::string neighbours = {match[3]};
      std::smatch submatch;
      std::regex alpha_links{R"([A-Z]{2})"};
      while (std::regex_search(neighbours, match, alpha_links)) {
        valve.neighbours.push_back(match.str());
        neighbours = match.suffix();
      }
      valves.push_back(valve);
    }
  }
}

void build_edges(std::set<Edge>& edges, const Valves& valves) {
  std::map<std::string, int> edge_map;

  for (int index{0}; auto valve : valves)
    edge_map.insert(std::make_pair(valve.name, index++));

  for (auto valve : valves) {
    int source{(edge_map[valve.name])};
    for (auto neighbour : valve.neighbours) {
      int destination{edge_map[neighbour]};
      if (source < destination)  // prevent duplicate paths
        edges.insert(Edge(source, destination));
      else
        edges.insert(Edge(destination, source));
    }
  }
}

void build_vertices(std::vector<VertexData>& vertices, Valves& valves) {
  for (auto valve : valves) {
    VertexData vertex{valve.name, valve.flow};
    vertices.push_back(vertex);
  }
}

using CavernGraph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, VertexData,
    boost::property<boost::edge_weight_t, int,
                    boost::property<boost::edge_weight2_t, int>>>;

void draw_graph(const CavernGraph& graph, const Valves& valves) {
  std::ofstream fout("../figs/johnson-eg.dot");

  if (!fout) {
    // Print an error and exit
    std::cerr << "Uh oh, Sample.txt could not be opened for writing!\n";
    return;
  }

  fout << "digraph A {\n"
       << "  rankdir=LR\n"
       << "size=\"5,3\"\n"
       << "ratio=\"fill\"\n"
       << "edge[style=\"bold\"; arrowhead=\"none\"]\n"
       << "node[shape=\"circle\"]\n";

  boost::graph_traits<CavernGraph>::edge_iterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
    fout << valves[boost::source(*ei, graph)].name << " -> "
         << valves[boost::target(*ei, graph)].name
         << "[label=" << get(boost::edge_weight, graph)[*ei] << "]\n";

  fout << "}\n";
}

void print_distance_matrix(const int D[][V]) {
  std::cout << "       ";
  for (int k = 0; k < V; ++k) std::cout << std::setw(5) << k;
  std::cout << std::endl;
  for (int i = 0; i < V; ++i) {
    std::cout << std::setw(3) << i << " -> ";
    for (int j = 0; j < V; ++j) {
      if (D[i][j] == (std::numeric_limits<int>::max)())
        std::cout << std::setw(5) << "inf";
      else
        std::cout << std::setw(5) << D[i][j];
    }
    std::cout << std::endl;
  }
}

void print_flowing_distance_matrix(const int D[][V],
                                   const std::vector<int>& working_valves) {
  std::cout << "       ";
  for (int k = 0; k < V; ++k)
    if (working_valves[k] || k == 0) std::cout << std::setw(5) << k;
  std::cout << std::endl;
  for (int i = 0; i < V; ++i) {
    if (!working_valves[i] && i) continue;
    std::cout << std::setw(3) << i << " -> ";
    for (int j = 0; j < V; ++j) {
      if (!working_valves[j] && j) continue;
      if (D[i][j] == (std::numeric_limits<int>::max)())
        std::cout << std::setw(5) << "inf";
      else
        std::cout << std::setw(5) << D[i][j];
    }
    std::cout << std::endl;
  }
}

int calculate_max_flow(std::vector<int> flows) {}

int main(int, char**) {
  auto time_start = std::chrono::high_resolution_clock::now();
  std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
  std::ifstream data{file_name};

  // Nodes valves;
  // Edges valve_neighbours;
  // ValveTags tags;

  Valves valves;
  parse_input(valves);

  std::set<Edge> edges;
  build_edges(edges, valves);

  std::vector<VertexData> vertices;
  build_vertices(vertices, valves);

  Edge edge_array[edges.size()];
  for (int index{0}; auto edge : edges) edge_array[index++] = edge;

  const std::size_t E = sizeof(edge_array) / sizeof(Edge);
  // const int V = 10;

  CavernGraph cavern(edge_array, edge_array + E, V);

  boost::property_map<CavernGraph, boost::edge_weight_t>::type w =
      get(boost::edge_weight, cavern);

  boost::graph_traits<CavernGraph>::edge_iterator e, e_end;
  for (boost::tie(e, e_end) = boost::edges(cavern); e != e_end; ++e) w[*e] = 1;

  std::vector<int> d(V, (std::numeric_limits<int>::max)());
  int D[V][V];
  boost::johnson_all_pairs_shortest_paths(cavern, D,
                                          boost::distance_map(&d[0]));

  std::vector<int> flow_vertices;
  for (auto valve : valves) flow_vertices.push_back(valve.flow);

  for (auto flow : flow_vertices) std::cout << flow << ' ';
  std::cout << std::endl;

  std::vector<std::vector<int>> dist_between_working_valves;

  for (int i = 0; i < V; ++i) {
    if (!flow_vertices[i] && i) continue;
    std::vector<int> distance;
    for (int j = 0; j < V; ++j) {
      if (!flow_vertices[j] && j) continue;
      distance.push_back(D[i][j]);
    }
    dist_between_working_valves.push_back(distance);
  }

  print_distance_matrix(D);
  std::cout << std::endl;
  print_flowing_distance_matrix(D, flow_vertices);
  draw_graph(cavern, valves);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - time_start);

  std::cout << "Time taken by program: " << duration.count() << " microseconds"
            << "\n";
  return 0;
}
