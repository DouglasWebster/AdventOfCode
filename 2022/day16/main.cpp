
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

#include "day16_utils.h"

using ValveDistances = std::vector<std::vector<int>>;
using Flows = std::vector<int>;
using WorkingValves = std::map<int, int>;
using LevelMaxFlow = std::pair<int, int>;
using LevelMaximums = std::vector<LevelMaxFlow>;

static int level {};

#if TESTING
#define FILE "../figs/test_report.txt"
#else
#define FILE "../figs/problem_report.txt"
#endif

static std::ofstream report(FILE);

int calculate_max_flow(WorkingValves& valves,
    const ValveDistances& distances,
    int current_valve = 0,
    int remaining_time = 30)
{
    int vented { valves[current_valve] * remaining_time };

    WorkingValves unvisited_valves = valves;
    unvisited_valves.erase(current_valve);
    int max_additional_flow { 0 };
    if (remaining_time < 2)
        return vented;
    for (auto valve : unvisited_valves) {
        int remaining_vent_time = remaining_time - distances[current_valve][valve.first] - 1;
        if (remaining_vent_time <= 0)
            continue; // not enough time to open valve
        int additional_flow {
            calculate_max_flow(
                unvisited_valves, distances, valve.first, remaining_vent_time)
        };

        if (additional_flow > max_additional_flow)
            max_additional_flow
                = additional_flow;
    }
    return vented + max_additional_flow;
}

int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
    std::ifstream data { file_name };

    Valves valves;
    parse_input(valves);

    std::set<Edge> edges;
    build_edges(edges, valves);

    std::vector<VertexData> vertices;
    build_vertices(vertices, valves);

    Edge edge_array[edges.size()];
    for (int index { 0 }; auto edge : edges)
        edge_array[index++] = edge;

    const std::size_t E = sizeof(edge_array) / sizeof(Edge);

    CavernGraph cavern(edge_array, edge_array + E, V);

    boost::property_map<CavernGraph, boost::edge_weight_t>::type w = get(boost::edge_weight, cavern);

    boost::graph_traits<CavernGraph>::edge_iterator e, e_end;
    for (boost::tie(e, e_end) = boost::edges(cavern); e != e_end; ++e)
        w[*e] = 1;

    std::vector<int> d(V, (std::numeric_limits<int>::max)());
    int D[V][V];
    boost::johnson_all_pairs_shortest_paths(
        cavern, D, boost::distance_map(&d[0]));

    Flows flow_vertices;
    for (auto valve : valves)
        flow_vertices.push_back(valve.flow);

    int starting_valve { 0 };

    WorkingValves working_valves;
    for (int index { 0 }; auto valve : valves) {
        if (valve.flow > 0 || valve.name == "AA")
            working_valves.insert(std::make_pair(index, valve.flow));
        if (valve.name == "AA")
            starting_valve = index;
        ++index;
    }

    ValveDistances dist_between_working_valves;

    for (int i = 0; i < V; ++i) {
        std::vector<int> distance;
        for (int j = 0; j < V; ++j)
            distance.push_back(D[i][j]);
        dist_between_working_valves.push_back(distance);
    }

    // print_distance_matrix(D);
    // std::cout << std::endl;
    // print_flowing_distance_matrix(D, flow_vertices, valves);
    // draw_graph(cavern, valves);

    // LevelMaximums level_max_s(15);

    int max_flow = calculate_max_flow(working_valves, dist_between_working_valves, starting_valve);

    CavernPaths all_paths {};

    std::vector<int> valves_for_combi {};
    for (const auto& [key, value] : working_valves) {
        if (key == starting_valve)
            continue;
        valves_for_combi.push_back(key);
    }

    int no_of_valves { static_cast<int>(valves_for_combi.size()) };
    bool check[no_of_valves];
    for (int index { 1 }; index <= no_of_valves; index++)
        path_combinations(valves_for_combi, index, 0, 0, check, no_of_valves, all_paths);

    int max_combined_flow { 0 };

    std::cout << "There are " << all_paths.size() << " combination of caverns: \n";

    for (int index { 0 }; auto& [key, value] : all_paths) {
        
        if (value)
            continue;
        WorkingValves path1 {};
        for (auto cavern : key)
            path1.insert(std::make_pair(cavern, working_valves[cavern]));
        path1.insert(std::make_pair(starting_valve, 0));
        int path1_flow { calculate_max_flow(path1, dist_between_working_valves, starting_valve, 26) };
        all_paths[key] = path1_flow;

        Path path2_keys = get_alternate_path(valves_for_combi, key);
        path2_keys.push_back(0);
        WorkingValves path2;
        for (auto cavern : path2_keys)
            path2.insert(std::make_pair(cavern, working_valves[cavern]));
        int path2_flow { calculate_max_flow(path2, dist_between_working_valves, starting_valve, 26) };
        all_paths[path2_keys] = path2_flow;

        if (path1_flow + path2_flow > max_combined_flow)
            max_combined_flow = path1_flow + path2_flow;
    }

    std::cout << "max flow part 1: " << max_flow << '\n';
    std::cout << "max flow part 2: " << max_combined_flow << '\n';
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
