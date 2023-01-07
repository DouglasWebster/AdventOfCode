#ifndef DSW_DAY16_UTILS_H
#define DSW_DAY16_UTILS_H

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>

#define TESTING false
#if TESTING
#define V 10
#else
#define V 58
#endif  // TESTING

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

using CavernGraph = boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, VertexData,
    boost::property<boost::edge_weight_t, int,
                    boost::property<boost::edge_weight2_t, int>>>;

void build_edges(std::set<Edge>& edges, const Valves& valves);
void parse_input(Valves& valves);
void build_vertices(std::vector<VertexData>& vertices, Valves& valves);
void draw_graph(const CavernGraph& graph, const Valves& valves);
void print_distance_matrix(const int D[][V]);
void print_flowing_distance_matrix(const int D[][V],
                                   const std::vector<int>& working_valves, const Valves& valves);

#endif //DSW_DAY16_UTILS_H