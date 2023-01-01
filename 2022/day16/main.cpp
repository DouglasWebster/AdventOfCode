#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/graphviz.hpp>
// #include <boost/graph/properties.hpp>
// #include <boost/property_map/property_map.hpp>
// #include <boost/graph/named_function_params.hpp>


#define TESTING false

using Nodes = std::map<int, int>;
using Edges = std::map<int, std::vector<int>>;
using ValveTags = std::map<std::string, int>;

struct ValveInfo {
  std::string name{};
  int flow{};
  std::string neighbours;
};

void parse_input(Nodes& nodes, Edges& edges, ValveTags& tags) {
  std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
  std::ifstream input{file_name};

  std::string initial_valve{""};
  std::vector<ValveInfo> valves{};

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
      valve.neighbours = {match[3]};
      valves.push_back(valve);
    }
  }

  for (int index{0}; auto valve : valves) {
    tags.insert(std::make_pair(valve.name, index++));
  }

  for (int index{0}; auto valve : valves) {
    nodes.insert(std::make_pair(index, valve.flow));
    std::vector<int> links;
    std::regex alpha_links{R"([A-Z]{2})"};
    std::smatch match;
    std::string neighbours = valve.neighbours;
    while (std::regex_search(neighbours, match, alpha_links)) {
      links.push_back(tags[match.str()]);
      neighbours = match.suffix();
    }
    nodes.insert(std::make_pair(index, valve.flow));
    edges.insert(std::make_pair(index++, links));
  }
}

int main(int, char**) {
  auto time_start = std::chrono::high_resolution_clock::now();
  std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
  std::ifstream data{file_name};

  Nodes valves;
  Edges valve_neighbours;
  ValveTags tags;

  parse_input(valves, valve_neighbours, tags);

  // std::vector<std::string> operational_valves;
  // for (auto valve : valves) {
  //   if (valve.second != 0) operational_valves.push_back(valve.first);
  // }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - time_start);

  std::cout << "Time taken by program: " << duration.count() << " microseconds"
            << "\n";
  return 0;
}
