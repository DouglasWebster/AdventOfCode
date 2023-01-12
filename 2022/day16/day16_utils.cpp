#include "day16_utils.h"

void build_edges(std::set<Edge>& edges, const Valves& valves)
{
    std::map<std::string, int> edge_map;

    for (int index { 0 }; auto valve : valves)
        edge_map.insert(std::make_pair(valve.name, index++));

    for (auto valve : valves) {
        int source { (edge_map[valve.name]) };
        for (auto neighbour : valve.neighbours) {
            int destination { edge_map[neighbour] };
            if (source < destination) // prevent duplicate paths
                edges.insert(Edge(source, destination));
            else
                edges.insert(Edge(destination, source));
        }
    }
}

void parse_input(Valves& valves)
{
    std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
    std::ifstream input { file_name };

    std::string initial_valve { "" };

    std::regex full_line {
        R"(Valve ([A-Z]{2}) has flow rate=(\d+); tunnels? leads? to valves? ([A-Z]{2}(?:, [A-Z]{2})*))"
    };

    while (input && !input.eof()) {
        std::string line {};
        std::getline(input, line);
        std::smatch match;

        if (std::regex_match(line, match, full_line)) {
            ValveInfo valve {};
            valve.name = match[1];
            valve.flow = stoi(match[2]);
            std::string neighbours = { match[3] };
            std::smatch submatch;
            std::regex alpha_links { R"([A-Z]{2})" };
            while (std::regex_search(neighbours, match, alpha_links)) {
                valve.neighbours.push_back(match.str());
                neighbours = match.suffix();
            }
            valves.push_back(valve);
        }
    }
}

void build_vertices(std::vector<VertexData>& vertices, Valves& valves)
{
    for (auto valve : valves) {
        VertexData vertex { valve.name, valve.flow };
        vertices.push_back(vertex);
    }
}

void draw_graph(const CavernGraph& graph, const Valves& valves)
{
    std::string file = (TESTING) ? "../figs/test_graph.dot" : "../figs/problem_graph.dot";
    std::ofstream fout(file);

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
        fout << "\"" << boost::source(*ei, graph) << ": "
             << valves[boost::source(*ei, graph)].name << "\n"
             << valves[boost::source(*ei, graph)].flow
             << "\" -> \""
             << boost::target(*ei, graph) << ": "
             << valves[boost::target(*ei, graph)].name << "\n"
             << valves[boost::target(*ei, graph)].flow << "\""
             << "[label=" << get(boost::edge_weight, graph)[*ei] << "]\n";

    fout << "}\n";
}

void print_distance_matrix(const int D[][V])
{
    std::cout << "       ";
    for (int k = 0; k < V; ++k)
        std::cout << std::setw(5) << k;
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
    const std::vector<int>& working_valves, const Valves& valves)
{
    std::string file = (TESTING) ? "../figs/test_flows.txt" : "../figs/problem_flow.txt";
    std::ofstream fout(file);

    if (!fout) {
        // Print an error and exit
        std::cerr << "Uh oh, " << file << " could not be opened for writing!\n";
        return;
    }

    fout << "       ";
    for (int k = 0; k < V; ++k)
        if (working_valves[k] || valves[k].name == "AA")
            fout << std::setw(5) << valves[k].name;
    fout << std::endl;
    for (int i = 0; i < V; ++i) {
        if (!working_valves[i] && valves[i].name != "AA")
            continue;
        fout << std::setw(3) << valves[i].name << "(" << std::setw(2) << valves[i].flow << ") -> ";
        for (int j = 0; j < V; ++j) {
            if (!working_valves[j] && valves[j].name != "AA")
                continue;
            if (D[i][j] == (std::numeric_limits<int>::max)())
                fout << std::setw(5) << "inf";
            else
                fout << std::setw(5) << D[i][j];
        }
        fout << std::endl;
    }
}

void path_combinations(
    std::vector<int> valves,
    int reqLen,
    int s,
    int currLen,
    bool check[],
    int l,
    CavernPaths& paths)
{
    if (currLen > reqLen)
        return;
    else if (currLen == reqLen) {
        std::vector<int> path;
        path.reserve(l);
        for (int i = 0; i < l; i++) {
            if (check[i] == true)
                path.push_back(valves[i]);
        }
        paths.insert(std::make_pair(path, 0));
        return;
    }
    if (s == l) {
        return;
    }
    check[s] = true;
    path_combinations(valves, reqLen, s + 1, currLen + 1, check, l, paths);
    check[s] = false;
    path_combinations(valves, reqLen, s + 1, currLen, check, l, paths);
}

Path get_alternate_path(const Path& full_path, const Path& chosen_path)
{
    Path alternate_path(full_path.size());
    Path::iterator it;

    it = std::set_difference(
            full_path.begin(), 
            full_path.end(), 
            chosen_path.begin(), 
            chosen_path.end(), alternate_path.begin());

    alternate_path.resize(it-alternate_path.begin());

    return alternate_path;
}
