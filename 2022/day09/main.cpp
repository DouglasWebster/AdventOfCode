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
using MovesVector = std::vector<Position>;
using Move = std::pair<char, int>;
using Rope = std::vector<Position>;

// helper functions

// stringify returns a unique hashed value of a Position
auto stringify = [](const Position &p, std::string sep = ",") -> std::string
{
    return std::to_string(p.first) + sep + std::to_string(p.second);
};

int get_file_size(std::string file)
{
    const auto fsize = fs::file_size(file);
    return fsize;
}

// movement functions
void move_up_or_down(
    VisitedSet &tail_movement_set,
    Move head_movement,
    Rope &rope)
{
    int move_offset = (head_movement.first == 'U') ? 1 : -1;
    for (auto move{head_movement.second}; move > 0; move--)
    {

        rope.front().second += move_offset;
        for (Rope::iterator leading_knot = rope.begin(), following_knot = ++rope.begin();
             following_knot != rope.end();
             ++leading_knot, ++following_knot)
        {
            // if the distance between two knots after moving the first is less than  2
            // then no further movement need be considered
            if (std::hypot((*leading_knot).first - (*following_knot).first, (*leading_knot).second - (*following_knot).second) < 2.0)
                break;

            int x_offset = (*leading_knot).first - (*following_knot).first;
            int y_offset = (*leading_knot).second - (*following_knot).second;
            if (x_offset)
                (*following_knot).first += (x_offset > 0) ? 1 : -1;
            if (y_offset)
                (*following_knot).second += (y_offset > 0) ? 1 : -1;
        }
        tail_movement_set.insert(stringify(rope.back()));
    }
}

void move_left_or_right(
    VisitedSet &tail_movement_set,
    Move head_movement,
    Rope &rope)
{
    int move_offset = (head_movement.first == 'R') ? 1 : -1;
    for (auto move{head_movement.second}; move > 0; move--)
    {

        rope.front().first += move_offset;
        for (Rope::iterator leading_knot = rope.begin(), following_knot = ++rope.begin();
             following_knot != rope.end();
             ++leading_knot, ++following_knot)
        {
            // if the distance between two knots after moving the first is less than root 2
            // then no further movement need be considered
            if (std::hypot((*leading_knot).first - (*following_knot).first, (*leading_knot).second - (*following_knot).second) < 2.0)
                break;

            int x_offset = (*leading_knot).first - (*following_knot).first;
            int y_offset = (*leading_knot).second - (*following_knot).second;
            if (x_offset)
                (*following_knot).first += (x_offset > 0) ? 1 : -1;
            if (y_offset)
                (*following_knot).second += (y_offset > 0) ? 1 : -1;
        }
        // last_head_position = head_position;
        tail_movement_set.insert(stringify(rope.back()));
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

    VisitedSet rope1_unique_tail_position;
    rope1_unique_tail_position.reserve(no_of_moves);
    VisitedSet rope2_unique_tail_position;
    rope2_unique_tail_position.reserve(no_of_moves);

    Rope part1_rope(2, Position(0, 0));
    Rope part2_rope(10, Position(0, 0));

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
            {
                move_up_or_down(rope1_unique_tail_position, movement, part1_rope);
                move_up_or_down(rope2_unique_tail_position, movement, part2_rope);
            }
            if (movement.first == 'L' || movement.first == 'R')
            {
                move_left_or_right(rope1_unique_tail_position, movement, part1_rope);
                move_left_or_right(rope2_unique_tail_position, movement, part2_rope);
            }
        }
    }

    std::cout << "No of unique positions visited by the tail for part 1: " << rope1_unique_tail_position.size() << '\n';
    std::cout << "No of unique positions visited by the tail for part 2: " << rope2_unique_tail_position.size() << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
