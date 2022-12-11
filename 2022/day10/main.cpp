#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <chrono>

#define TESTING false

using Cycle = std::pair<int, int>;
using Cycles = std::vector<Cycle>;

void draw_screen(char screen[][40], const Cycles &cycles)
{
    int cycle_index{1};
    for (int row{0}; row < 6; row++)
    {
        for (int column{0}; column < 40; column++)
        {
            std::pair<int, int> sprite_range(
                (cycles[cycle_index].second) - 2,
                (cycles[cycle_index].second) + 2
            );
        screen[row][column] = (column > sprite_range.first && column < sprite_range.second ) 
            ? '#' 
            : ' ';

        std::cout << screen[row][column];
        ++cycle_index;
        }
        std::cout << '\n';

    }
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

    // align the vector so that updates occur after the cycle ends.
    Cycles cycles{Cycle(0, 0), Cycle(1, 1)};
    cycles.reserve(250);

    int cycle_no{2};

    while (data)
    {
        std::string instructionInput;
        std::getline(data, instructionInput);
        if (instructionInput != "")
        {
            std::stringstream instruction(instructionInput);
            std::string operation{};
            int operand{};

            instruction >> operation >> operand;
            if (operation == "noop")
                cycles.push_back(Cycle(cycle_no, cycles.back().second));
            if (operation == "addx")
            {
                cycles.push_back(Cycle(cycle_no, cycles.back().second)), ++cycle_no;
                cycles.push_back(Cycle(cycle_no, cycles.back().second + operand));
            }
        }
        ++cycle_no;
    }

    int signal_strength{0};
    for (auto cycle_index{20}; cycle_index < 221; cycle_index += 40)
        signal_strength += (cycle_index)*cycles[cycle_index].second;

    std::cout << "Signal strength is: " << signal_strength << '\n';

    char screen[6][40]{};

    draw_screen(screen,cycles );

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
