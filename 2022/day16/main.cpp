
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
    report << "At valve " << current_valve
           << " venting " << valves[current_valve] << " for " << remaining_time
           << "s, vented: " << vented << std::endl;
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

        report << "returned to valve "
               << current_valve;

        if (additional_flow > max_additional_flow) {
            max_additional_flow
                = additional_flow;

            report << " additional venting "
                   << additional_flow
                   << " new total "
                   << max_additional_flow + vented
                   << std::endl;
        } else {
            report << " " << additional_flow << " lower than "
                   << max_additional_flow << ". Total still "
                   << max_additional_flow + vented
                   << std::endl;
        }
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

    std::cout << "max flow: " << max_flow << '\n';

    // for (auto level : level_max_s) {
    //     if (level.second)
    //         std::cout << "valve " << valves[level.first].name << " vented " << level.second << " gas.\n";
    // }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
