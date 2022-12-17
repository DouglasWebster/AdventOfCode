#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>

#include <chrono>

#define TESTING false

using CoOrdinate = std::pair<int, int>;
using Path = std::vector<CoOrdinate>;
using Paths = std::vector<Path>;
using CaveRow = std::deque<char>;
using Cave = std::vector<CaveRow>;

void draw_cave(Cave &cave, int min = 0, int max = 500)
{
    for (auto row : cave)
    {
        for (int column{min}; column <= max; ++column)
        {
            std::cout << row[column];
        }
        std::cout << '\n';
    }
}

// build the co-ordinates of the paths.  keep track of the maximum
// x & y co-ordinates and the min x co-ordinate;
void build_path_coords(std::ifstream &data, Paths &paths, int &x_min, int &x_max, int &y_max)
{
    while (data)
    {
        std::string path{};
        std::getline(data, path);
        if (path == "")
            continue;
        std::stringstream ss(path);
        Path new_path;
        while (ss.peek() != EOF)
        {
            char dummy;
            int x_coord{};
            int y_coord{};
            ss >> x_coord;
            if (x_coord > x_max)
                x_max = x_coord;
            if (x_coord < x_min)
                x_min = x_coord;
            ss.get(dummy);
            ss >> y_coord;
            if (y_coord > y_max)
                y_max = y_coord;
            new_path.push_back(CoOrdinate(x_coord, y_coord));
            while (!isdigit(ss.peek()) && ss.peek() != EOF)
                ss.get(dummy);
        }
        paths.push_back(new_path);
    }
}

void fill_in_rocks(Paths &paths, Cave &cave)
{

    for (Path path : paths)
    {
        for (int coord{0}; coord < path.size() - 1; ++coord)
        {
            CoOrdinate coord1 = path[coord];
            CoOrdinate coord2 = path[coord + 1];

            if (coord1.first == coord2.first) // same column
            {
                if (coord1.second > coord2.second) // rows count downwards
                {
                    int dummy = coord1.second;
                    coord1.second = coord2.second;
                    coord2.second = dummy;
                }
                for (int row{coord1.second}; row <= coord2.second; ++row)
                    cave[row][coord1.first] = '#';
            }
            if (coord1.second == coord2.second) // same row
            {
                if (coord1.first > coord2.first) // columns count left to right
                {
                    int dummy = coord1.first;
                    coord1.first = coord2.first;
                    coord2.first = dummy;
                }
                for (int column{coord1.first}; column <= coord2.first; ++column)
                    cave[coord1.second][column] = '#';
            }
        }
    }
}

void reset_cave(Cave &cave)
{
    for (int r{0}; auto row : cave)
    {
        for (int c{0}; auto pos : row)
        {
            if (pos == 'o')
                cave[r][c] = '.';
            ++c;
        }
        ++r;
    }
}

bool drop_sand(Cave &cave)
{
    static int offset{0};
    CoOrdinate sand_pos(500, 0);
    int column{sand_pos.first};
    int row{sand_pos.second};
    int cave_floor = cave.size() - 1;
    bool falling{true};
    while (falling)
    {
        if (row == cave_floor )
            return false; // we've hit the bottom so no stopping it now
        if (row < cave_floor && cave[row + 1][column] == '.')
        {
            row++;
            continue;
        }
        if (column > 0 && cave[row + 1][column - 1] == '.')
        {
            column--;
            row++;
            continue;
        }
        if (column < cave[0].size() && cave[row + 1][column + 1] == '.')
        {
            column++;
            row++;
            continue;
        }

        // stopped within space so mark position as sand.
        // if it is the start position then we've finished.
        cave[row][column] = 'o';
        return (row == 0 && column == 500) ? false : true;
    }

    return false;
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

    Paths paths;
    paths.reserve(500);

    int cave_depth{0};
    int cave_min_x{500};
    int cave_max_x{cave_min_x};

    build_path_coords(data, paths, cave_min_x, cave_max_x, cave_depth);

    int cave_width = 1000;
    Cave cave(cave_depth + 1, CaveRow(cave_width, '.'));

    fill_in_rocks(paths, cave);

    int sand_dropped{0};

    while (drop_sand(cave))
    {
        ++sand_dropped;
    }

    std::cout << "Cave depth: " << cave_depth << '\n';
    std::cout << "Cave spans " << cave_min_x << " to " << cave_max_x << '\n';
    std::cout << "Cave map:\n";
    // draw_cave(cave);
    std::cout << "Units of sand that came to rest before the sand started flowing into the abyss: " << sand_dropped << '\n';

    reset_cave(cave);
    cave.push_back(CaveRow(cave_width + 1, '.'));
    cave.push_back(CaveRow(cave_width + 1, '#'));
    //  draw_cave(cave, cave_min_x - 15, cave_max_x + 15);

    sand_dropped = 1;
    while (drop_sand(cave))
    {
        ++sand_dropped;
        // draw_cave(cave, cave_min_x - 15, cave_max_x + 15);
    }

    std::cout << "Units of sand that came to rest before entrance blocked: " << sand_dropped << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
