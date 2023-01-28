#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <vector>

#include "day18_helpers.h"

#define TESTING false

#if TESTING
#define FILE "../test.txt"
#else
#define FILE "../input.txt"
#endif

using BlockAxis = std::vector<int>;
using Blocks = std::vector<BlockAxis>;
using Point = std::pair<int, int>;
using Location3D = std::tuple<int, int, int>;
using Matrix3D = std::vector<std::vector<std::vector<int>>>;
using NextVisits = std::vector<Location3D>;

bool is_valid(const Matrix3D& matrix, Location3D index)
{
    int x = std::get<0>(index);
    int y = std::get<1>(index);
    int z = std::get<2>(index);

    int x_limit { static_cast<int>(matrix.size()) };
    int y_limit { static_cast<int>(matrix[0].size()) };
    int z_limit { static_cast<int>(matrix[0][0].size()) };

    if (x < 0 || y < 0 || z < 0 || x >= x_limit || y >= y_limit || z >= z_limit)
        return false;

    return (matrix[x][y][z] == 0);
}

void get_neighbours(const Matrix3D matrix, Location3D current_pos, NextVisits& next_positions)
{
    Location3D limits({ matrix.size(), matrix[0].size(), matrix[0][0].size() });

    Location3D next_x_pos { current_pos };
    std::get<0>(next_x_pos) += 1;
    Location3D next_x_neg { current_pos };
    std::get<0>(next_x_neg) -= 1;
    Location3D next_y_pos { current_pos };
    std::get<1>(next_y_pos) += 1;
    Location3D next_y_neg { current_pos };
    std::get<1>(next_y_neg) -= 1;
    Location3D next_z_pos { current_pos };
    std::get<2>(next_z_pos) += 1;
    Location3D next_z_neg { current_pos };
    std::get<2>(next_z_neg) -= 1;

    if (is_valid(matrix, next_x_pos))
        next_positions.push_back(next_x_pos);
    if (is_valid(matrix, next_x_neg))
        next_positions.push_back(next_x_neg);
    if (is_valid(matrix, next_y_pos))
        next_positions.push_back(next_y_pos);
    if (is_valid(matrix, next_y_neg))
        next_positions.push_back(next_y_neg);
    if (is_valid(matrix, next_z_pos))
        next_positions.push_back(next_z_pos);
    if (is_valid(matrix, next_z_neg))
        next_positions.push_back(next_z_neg);
}

void flood_fill(Matrix3D& blocks, Location3D starting_point, int replacement_value = 1)
{
    int cols { std::get<0>(starting_point) };
    int rows { std::get<1>(starting_point) };
    int levels { std::get<2>(starting_point) };

    std::set<Location3D> to_visit;
    to_visit.insert({ cols, rows, levels });
    std::set<Location3D> visited;
    NextVisits next_visits;

    while (to_visit.size() > 0) {
        next_visits.clear();
        std::set<Location3D>::iterator it = to_visit.begin();
        Location3D next = *it;
        to_visit.erase(it);
        visited.insert(next);
        blocks[std::get<0>(next)][std::get<1>(next)][std::get<2>(next)] = replacement_value;
        get_neighbours(blocks, next, next_visits);
        for (auto neighbour : next_visits) {
            bool not_visited { visited.find(neighbour) == visited.end() };
            if (not_visited)
                to_visit.insert(neighbour);
        }
    }
}

/// @brief Given a block create an enveloping cube with a border of at
///  least 1 on every side.
///  1. Flood fill the outside of this enveloping
///     cube.
///  2. find any empty space left and flood fill this, keeping track
///     of any location filled.
///  3. Treat this newly flooded area as a new lava
///     block and get its outside are.
///  4. repeat 2 & 3 until there are no more void spaces.
/// @param blocks
Matrix3D convert_to_filled_map(Blocks& blocks)
{
    int x_size = *std::max_element(blocks[0].begin(), blocks[0].end()) + 2;
    int y_size = *std::max_element(blocks[1].begin(), blocks[1].end()) + 2;
    int z_size = *std::max_element(blocks[2].begin(), blocks[2].end()) + 2;

    Matrix3D encircling_cube(x_size, std::vector<std::vector<int>>(y_size, std::vector<int>(z_size, 0)));

    for (int index { 0 }; index < blocks[0].size(); ++index) {
        int x { blocks[0][index] };
        int y { blocks[1][index] };
        int z { blocks[2][index] };

        encircling_cube[x][y][z] = 1;
    }

    return encircling_cube;
}

int get_blocks_area(Blocks& blocks)
{

    int total_face_count { static_cast<int>(blocks[0].size()) * 6 };
    int common_face_count { 0 };

    // check x & y axis for co-planer blocks
    for (int x_index { 0 }; x_index < static_cast<int>(blocks[0].size()); ++x_index) {
        Point base_point { blocks[0][x_index], blocks[1][x_index] };
        int base_z_ordinate { blocks[2][x_index] };
        for (int y_index { 0 }; y_index < static_cast<int>(blocks[1].size()); ++y_index) {
            Point check_point { blocks[0][y_index], blocks[1][y_index] };
            if (base_point == check_point) {
                int check_z_ordinate { blocks[2][y_index] };
                if (abs(base_z_ordinate - check_z_ordinate) == 1) {
                    common_face_count++;
                }
            }
        }
    }

    // check x & z axis for co-planer blocks
    for (int x_index { 0 }; x_index < static_cast<int>(blocks[0].size()); ++x_index) {
        Point base_point { blocks[0][x_index], blocks[2][x_index] };
        int base_y_ordinate { blocks[1][x_index] };
        for (int z_index { 0 }; z_index < static_cast<int>(blocks[2].size()); ++z_index) {
            Point check_point { blocks[0][z_index], blocks[2][z_index] };
            if (base_point == check_point) {
                int check_y_ordinate { blocks[1][z_index] };
                if (abs(base_y_ordinate - check_y_ordinate) == 1) {
                    common_face_count++;
                }
            }
        }
    }

    // check y & z axis for co-planer blocks
    for (int y_index { 0 }; y_index < static_cast<int>(blocks[1].size()); ++y_index) {
        Point base_point { blocks[1][y_index], blocks[2][y_index] };
        int base_x_ordinate { blocks[0][y_index] };
        for (int z_index { 0 }; z_index < static_cast<int>(blocks[2].size()); ++z_index) {
            Point check_point { blocks[1][z_index], blocks[2][z_index] };
            if (base_point == check_point) {
                int check_x_ordinate { blocks[0][z_index] };
                if (abs(base_x_ordinate - check_x_ordinate) == 1) {
                    common_face_count++;
                }
            }
        }
    }

    return total_face_count - common_face_count;
}

Location3D get_void_point(const Matrix3D& blocks)
{
    Location3D no_point { -1, -1, -1 };

    for (int x { 0 }; auto row : blocks) {
        for (int y { 0 }; auto col : row) {
            for (int z { 0 }; auto level : col) {
                if (level == 0)
                    return Location3D { x, y, z };
                ++z;
            }
            ++y;
        }
        ++x;
    }

    return no_point;
}

void get_void_points(const Matrix3D matrix, Blocks& void_blocks)
{
    int x_limit { static_cast<int>(matrix.size()) - 1 };
    int y_limit { static_cast<int>(matrix[0].size()) - 1 };
    int z_limit { static_cast<int>(matrix[0][0].size()) - 1 };

    for (int x { 1 }; x < x_limit; ++x)
        for (int y { 1 }; y < y_limit; ++y)
            for (int z { 1 }; z < z_limit; ++z)
                if (matrix[x][y][z] == 0) {
                    void_blocks[0].push_back(x);
                    void_blocks[1].push_back(y);
                    void_blocks[2].push_back(z);
                }
}

/// @brief
/// @param
/// @param
/// @return
int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data { FILE };

    if (!data) {
        std::cerr << FILE << " could not be opened for reading\n ";
        return 1;
    }

    Blocks blocks(3);

    while (data && !data.eof()) {
        std::string ordinates {};
        std::getline(data, ordinates);
        std::smatch m;
        std::regex e("\\d+");
        int index { 0 };
        while (std::regex_search(ordinates, m, e)) {
            for (auto x : m)
                blocks[index].push_back(std::stoi(x));
            ordinates = m.suffix().str();
            ++index;
        }
    }

    // Part 1
    int part1_area { get_blocks_area(blocks) };

    std::cout << "Total number of faces by counting: " << part1_area << '\n';

    Matrix3D encircling_cube { convert_to_filled_map(blocks) };

    flood_fill(encircling_cube, Location3D { 0, 0, 0 });

    Blocks void_areas(3);

    get_void_points(encircling_cube, void_areas);

    int void_area {get_blocks_area(void_areas)};

    int part2_area = part1_area - void_area;

    std:: cout << "Total external area: " << part2_area << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);
    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
