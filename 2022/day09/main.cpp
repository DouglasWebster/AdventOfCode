#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <utility>
#include <cmath>
#include <filesystem>

#include <chrono>

#define TESTING false

// #define DEBUG

namespace fs = std::filesystem;

using Position = std::pair<int, int>;
using VisitedSet = std::unordered_set<std::string>;
using MovesVector= std::vector<Position>;
using Move = std::pair<char, int>;

auto stringify = [](const Position &p, std::string sep = "-") -> std::string
{
    return std::to_string(p.first) + sep + std::to_string(p.second);
};

int get_file_size(std::string file) {
    const auto fsize = fs::file_size(file);
    return fsize;
}


void move_up_or_down(
    VisitedSet &tail_movement_set,
    Move head_movement,
    Position &head_position,
    Position &tail_position)
{
    int move_offset = (head_movement.first == 'U') ? 1 : -1;
    Position last_head_position{head_position};
    for (auto move{head_movement.second}; move > 0; move--)
    {
        head_position.second += move_offset;
        if (std::hypot(head_position.first - tail_position.first, head_position.second - tail_position.second) > 1.5)
        {
            tail_position = last_head_position;
            tail_movement_set.insert(stringify(tail_position));
        }
        last_head_position = head_position;
    }
}

void move_left_or_right(
    VisitedSet &tail_movement_set,
    Move head_movement,
    Position &head_position,
    Position &tail_position)
{
    int move_offset = (head_movement.first == 'R') ? 1 : -1;
    Position last_head_position{head_position};
    for (auto move{head_movement.second}; move > 0; move--)
    {
        head_position.first += move_offset;
        if (std::hypot(head_position.first - tail_position.first, head_position.second - tail_position.second) > 1.5)
        {
            tail_position = last_head_position;
            tail_movement_set.insert(stringify(tail_position));
        }
        last_head_position = head_position;
    }
}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
    // get an overestimate of the number of moves so that we can reserve enough space in vectors;
    int no_of_moves = get_file_size(file_name) / 3;  
    std::ifstream data{file_name};

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    VisitedSet unique_tail_position;
    unique_tail_position.reserve(no_of_moves);
    MovesVector tail_positions;
    tail_positions.reserve(no_of_moves);
    Position head_postion{0, 0};
    Position tail_position{0, 0};
    tail_positions.push_back(tail_position);
    unique_tail_position.insert(stringify(tail_position));

    while (data)
    {
        std::string moveInput;
        std::getline(data, moveInput);
        if (moveInput != "")
        {
            std::stringstream ss(moveInput);
            char direction;
            int steps;
            ss >> direction >> steps;
            Move movement{std::make_pair(direction, steps)};
            if (movement.first == 'U' || movement.first == 'D')
                move_up_or_down(unique_tail_position, movement, head_postion, tail_position);
            if (movement.first == 'L' || movement.first == 'R')
                move_left_or_right(unique_tail_position, movement, head_postion, tail_position);
        }
    }

    std::cout << "No of unique positions visited by the tail: " << unique_tail_position.size() << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
