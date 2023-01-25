
#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
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

/// @brief Cantor pairing function to generate a hash of two integers
/// @param a first integer of the pair
/// @param b second integer of the pair
/// @return the hash of the two integers;
inline int cantor_pairing(int a, int b)
{
    return ((a + b) * (a + b + 1)) / 2 + b;
}

/// @brief
/// @param
/// @param
/// @return
int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data { FILE };
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);

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

    int common_face_count { 0 };

    // check x & y axis for co-planer blocks
    for (int x_index { 0 }; x_index < static_cast<int>(blocks[0].size()); ++x_index) {
        Point base_point { blocks[0][x_index], blocks[1][x_index] };
        int base_z_ordinate { blocks[2][x_index] };
        for (int y_index {0}; y_index < static_cast<int>(blocks[1].size()); ++y_index) {
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

    std::cout << "Total number of faces by counting: " << static_cast<int>(blocks[0].size()) * 6 - common_face_count << '\n';

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}

