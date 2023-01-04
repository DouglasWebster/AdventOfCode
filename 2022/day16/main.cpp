
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

#include "day16_utils.h"

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
    std::vector<int> distance;
    for (int j = 0; j < V; ++j) 
      distance.push_back(D[i][j]);
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
