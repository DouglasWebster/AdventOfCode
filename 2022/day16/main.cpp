
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



int calculate_max_flow(WorkingValves& valves,
    const ValveDistances& distances,
    LevelMaximums& maximums,
    int starting_valve = 0,
    int initial_flow = 0,
    int remaining_time = 30)
{
    int max_flow { initial_flow };
    int this_flow { initial_flow };

    static int level{0};

    WorkingValves::reverse_iterator rit;
    bool valve_chosen{};
    for (rit = valves.rbegin(); rit != valves.rend(); ++rit) {
        int next_valve = rit->second;
        int travel_time = distances[starting_valve][next_valve];
        if (!(travel_time < remaining_time))
            continue; // can't open valve in time, try another
        int time_for_next_valve = remaining_time - travel_time - 1;
        int next_flow { time_for_next_valve * rit->first };
        // remove the next valve from the list to check
        WorkingValves remaining_valves = valves;
        remaining_valves.erase(rit->first);
        level++;
        int flow {
            calculate_max_flow(
                remaining_valves, distances, maximums, next_valve, next_flow, time_for_next_valve)
        };
        if(flow + this_flow > max_flow) 
            maximums[level] = LevelMaxFlow(next_valve, flow);
        flow += this_flow;
        if (flow > max_flow) {
            // if (!valve_chosen) {
            //     visited_valves.push_back(next_valve);
            //     valve_chosen = true;
            
            // } else {
            //     visited_valves.pop_back();
            //     visited_valves.push_back(next_valve);
            // }
            max_flow = flow;
        }
        level--;
    }
    // std::cout << "using valve " << starting_valve << ", flow at " << max_flow << " " << std::endl;

    // visited_valves.insert(std::end(visited_valves), std::begin(visiting_valves), std::end(visiting_valves));
    return max_flow;
}

int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
    std::ifstream data { file_name };

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
    for (int index { 0 }; auto edge : edges)
        edge_array[index++] = edge;

    const std::size_t E = sizeof(edge_array) / sizeof(Edge);
    // const int V = 10;

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

    // for (int index { 0 }; auto flow : flow_vertices) {
    //     if (flow > 0)
    //         std::cout << "Valve " << index << " flow: " << flow << '\n';
    //     ++index;
    // }

    WorkingValves working_valves;
    for (int index { 0 }; auto valve : valves) {
        if (valve.flow > 0 || index == 0)
            working_valves.insert(std::make_pair(valve.flow, index));
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

    LevelMaximums level_max_s(15);

    int max_flow = calculate_max_flow(working_valves, dist_between_working_valves, level_max_s);

    std::cout << "max flow: " << max_flow << '\n';

    for (auto level: level_max_s) {
        if (level.second) 
            std::cout << "valve " << valves[level.first].name << " vented " << level.second << " gas.\n";
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
