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
             << valves[boost::source(*ei, graph)].name << "\" -> \""
             << boost::target(*ei, graph) << ": "
             << valves[boost::target(*ei, graph)].name << "\""
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
        if (working_valves[k] || k == 0)
            fout << std::setw(5) << valves[k].name;
    fout << std::endl;
    for (int i = 0; i < V; ++i) {
        if (!working_valves[i] && i)
            continue;
        fout << std::setw(3) << valves[i].name << "(" << std::setw(2) << valves[i].flow << ") -> ";
        for (int j = 0; j < V; ++j) {
            if (!working_valves[j] && j)
                continue;
            if (D[i][j] == (std::numeric_limits<int>::max)())
                fout << std::setw(5) << "inf";
            else
                fout << std::setw(5) << D[i][j];
        }
        fout << std::endl;
    }
}
