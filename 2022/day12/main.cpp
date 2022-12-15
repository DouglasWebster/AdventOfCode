#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#

#include <chrono>

#define TESTING false

using Point = std::pair<int, int>;

using LineVector = std::vector<char>;
using GraphVector = std::vector<LineVector>;
using PointSet = std::set<Point>;

void draw_missing(GraphVector &g)
{
    for (int r{0}; auto row : g)
    {
        for (int c{0}; auto col : row)
        {
            if (col > 'h' && col < 'o')
                std::cout << g[r][c];
            else
                std::cout << ' ';
            ++c;
        }
        std::cout << '\n';
        ++r;
    }
    std::cout << '\n';
}
void draw_visited(std::vector<std::vector<int>> &visited, GraphVector &g)
{
    for (int r{0}; auto row : visited)
    {
        for (int c{0}; auto col : row)
        {
            if (col)
                std::cout << g[r][c];
            else
                std::cout << ' ';
            ++c;
        }
        std::cout << '\n';
        ++r;
    }
    std::cout << '\n';
}

int get_shortest_path(GraphVector &graph, Point start_pos, Point end_pos, bool record_intermediates, PointSet &intermediates)
{
    int rows{graph.size()};
    int columns{graph[0].size()};
    std::queue<int> row_queue, column_queue;

    int move_count{0};
    int nodes_left_in_layer{1};
    int nodes_in_next_layer{0};
    bool reached_end{false};

    int direction_vector_row[]{-1, +1, 0, 0};
    int direction_vector_col[]{0, 0, +1, -1};

    std::vector<std::vector<int>>
        visited(rows, std::vector<int>(columns, 0));
    row_queue.push(start_pos.first);
    column_queue.push(start_pos.second);

    std::set<int> b_moves{};

    while (row_queue.size() > 0 && !reached_end)
    {

        int current_row = row_queue.front();
        row_queue.pop();
        int current_column = column_queue.front();
        column_queue.pop();
        if (current_row == end_pos.first && current_column == end_pos.second)
        // if (graph[current_row][current_column] == 'z' + 1)
        {
            reached_end = true;
            break;
        }
        // explore_neighbours(r, c);
        for (auto index{0}; index < 4; index++)
        {
            int check_row{current_row + direction_vector_row[index]};
            int check_column{current_column + direction_vector_col[index]};
            if (check_row < 0 || check_column < 0)
                continue;
            if (check_row > rows - 1 || check_column > columns - 1)
                continue;
            if (visited[check_row][check_column])
                continue;
            char step_from = graph[current_row][current_column];
            char step_to = graph[check_row][check_column];
            if (step_to > (step_from + 1))
                continue;

            if (step_from == 'a' && step_to == 'b')
                b_moves.insert(move_count);

            row_queue.push(check_row);
            column_queue.push(check_column);

            visited[check_row][check_column] = move_count;

            // draw_visited(visited, graph);

            nodes_in_next_layer++;
        }

        nodes_left_in_layer--;
        if (nodes_left_in_layer == 0)
        {
            nodes_left_in_layer = nodes_in_next_layer;
            nodes_in_next_layer = 0;
            move_count++;

            // std::cout << "move: " << move_count << " "
            //           << "\n\n";
        }
    }
    if (reached_end)
        return move_count;
    return -1;
}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
    std::ifstream data{file_name};

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    std::vector<std::string> graphLines;

    while (data)
    {
        std::string graphLine;
        std::getline(data, graphLine);
        if (graphLine != "")
            graphLines.push_back(graphLine);
    }

    const int rows = graphLines.size();
    const int columns = graphLines[0].size();
    Point start_pos;
    Point end_pos;

    // std::queue<int> row_queue, column_queue;

    // int move_count{0};
    // int nodes_left_in_layer{1};
    // int nodes_in_next_layer{0};
    // bool reached_end{false};

    // int direction_vector_row[]{-1, +1, 0, 0};
    // int direction_vector_col[]{0, 0, +1, -1};

    GraphVector graph;
    graph.resize(graphLines.size());

    for (auto row{0}; auto line : graphLines)
    {
        for (auto column{0}; auto height : line)
        {
            if (height == 'S')
            {
                start_pos.first = row;
                start_pos.second = column;
                graph[row].push_back('a' - 1);
                ++column;
                continue;
            }
            if (height == 'E')
            {
                end_pos.first = row;
                end_pos.second = column;
                graph[row].push_back('z' + 1);
                ++column;
                continue;
            }
            graph[row].push_back(height);
            ++column;
        }
        ++row;
    }

    // draw_missing(graph);

    // std::vector<std::vector<int>>
    //     visited(rows, std::vector<int>(columns, 0));
    // row_queue.push(start_pos.first);
    // column_queue.push(start_pos.second);

    PointSet b_moves{};

    // while (row_queue.size() > 0 && !reached_end)
    // {

    //     int current_row = row_queue.front();
    //     row_queue.pop();
    //     int current_column = column_queue.front();
    //     column_queue.pop();
    //     if (current_row == end_pos.first && current_column == end_pos.second)
    //     // if (graph[current_row][current_column] == 'z' + 1)
    //     {
    //         reached_end = true;
    //         break;
    //     }
    //     // explore_neighbours(r, c);
    //     for (auto index{0}; index < 4; index++)
    //     {
    //         int check_row{current_row + direction_vector_row[index]};
    //         int check_column{current_column + direction_vector_col[index]};
    //         if (check_row < 0 || check_column < 0)
    //             continue;
    //         if (check_row > rows - 1 || check_column > columns - 1)
    //             continue;
    //         if (visited[check_row][check_column])
    //             continue;
    //         char step_from = graph[current_row][current_column];
    //         char step_to = graph[check_row][check_column];
    //         if (step_to > (step_from + 1))
    //             continue;

    //         if (step_from == 'a' && step_to == 'b')
    //             b_moves.insert(move_count);

    //         row_queue.push(check_row);
    //         column_queue.push(check_column);

    //         visited[check_row][check_column] = move_count;

    //         // draw_visited(visited, graph);

    //         nodes_in_next_layer++;
    //     }

    //     nodes_left_in_layer--;
    //     if (nodes_left_in_layer == 0)
    //     {
    //         nodes_left_in_layer = nodes_in_next_layer;
    //         nodes_in_next_layer = 0;
    //         move_count++;

    //         // std::cout << "move: " << move_count << " "
    //         //           << "\n\n";
    //     }
    // }

    // draw_visited(visited, graph);

    int moves_taken = get_shortest_path(graph, start_pos, end_pos, false, b_moves);

    if (moves_taken != -1)
    {
        std::cout << "Moves required from starting position = " << moves_taken << '\n';
        //     std::cout << "Moves required from closets ground position = " << move_count - *b_moves.end() << '\n';
    }
    else
        std::cout << "No solution found.\n";

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
