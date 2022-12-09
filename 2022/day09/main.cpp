#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>

#include <chrono>

#define TESTING true

// #define DEBUG

using VisitedPositions = std::pair<int, int>;


int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (TESTING) ? "../input.txt" : "../test.txt";
    std::ifstream data{file_name};

    std::set<VisitedPositions> unique_tail_positions{};

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    while (data)
    {
        std::string rowInput;
        std::getline(data, rowInput);
        std::stringstream ss;
        if (rowInput != "")
        {

        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
