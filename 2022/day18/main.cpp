
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
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

    // TODO: load each row of the input data as an x,y,z, coordinate 
    // and store them in the blocks variable.
    while (data && !data.eof()) {
    }

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
