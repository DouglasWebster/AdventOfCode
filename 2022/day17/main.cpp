
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define TESTING true

using Column = std::vector<char>;
using Chamber = std::vector<Column>;

#if TESTING
#define FILE "../test.txt"
#else
#define FILE "../input.txt"
#endif

void add_line(Chamber& chamber)
{
    for (auto index { 0 }; auto& col : chamber) {
        if (index > 1 && index < 6)
            col.push_back('@');
        else
            col.push_back(' ');
        index++;
    }
}

void drop_line(Chamber& chamber)
{
}

static std::ofstream report(FILE);

int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data { FILE };
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);

    std::deque<char> jets {};

    if (!data) {
        std::cerr << FILE << " could not be opened for reading\n ";
        return 1;
    }

    while (data) {
        char jet = data.get();
        jets.push_back(jet);
    }

    Chamber chamber {
        { '-', ' ', ' ', ' ' },
        { '-', ' ', ' ', ' ' },
        { '-', ' ', ' ', ' ' },
        { '-', ' ', ' ', ' ' },
        { '-', ' ', ' ', ' ' },
        { '-', ' ', ' ', ' ' },
        { '-', ' ', ' ', ' ' }

    };

    add_line(chamber);

    std::vector<int> low_points(7, 0);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
