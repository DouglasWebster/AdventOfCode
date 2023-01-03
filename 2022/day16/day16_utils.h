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