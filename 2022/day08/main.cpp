#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <iomanip>

#include <chrono>

#define MATRIX_SIZE 99

using IntVector = std::vector<int>;
using BoolVector = std::vector<bool>;
using ForrestMatrix = std::vector<std::vector<int>>;
using VisibilityMatrix = std::vector<std::vector<bool>>;
using TreePosition = std::pair<int, int>;

void draw_matrix(VisibilityMatrix matrix)
{
    for (auto visibility_rows : matrix)
    {
        for (auto visibility_item : visibility_rows)
        {
            std::cout << std::setw(2);
            std::cout << visibility_item;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void draw_matrix(ForrestMatrix matrix)
{
    for (auto visibility_rows : matrix)
    {
        for (auto visibility_item : visibility_rows)
        {
            std::cout << std::setw(2);
            std::cout << visibility_item;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int calculate_scenic_score(ForrestMatrix &forrest, TreePosition position)
{
}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (MATRIX_SIZE == 99) ? "../input.txt" : "../test.txt";
    std::ifstream data{file_name};
    ForrestMatrix forrest(MATRIX_SIZE, IntVector(MATRIX_SIZE, 0));
    VisibilityMatrix visibility_matrix(MATRIX_SIZE, BoolVector(MATRIX_SIZE, false));
    ForrestMatrix scenic_matrix(MATRIX_SIZE, IntVector(MATRIX_SIZE, 0));

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    int row_no{0};
    while (data)
    {
        std::string rowInput;
        std::getline(data, rowInput);
        std::stringstream ss;
        if (rowInput != "")
        {
            ss << rowInput;

            for (int column{0}; column < rowInput.length(); column++)
            {

                int value = rowInput[column] - '0';
                forrest[row_no][column] = value;

                visibility_matrix[row_no][column] =
                    (column == 0 || column == MATRIX_SIZE - 1 || row_no == 0 || row_no == MATRIX_SIZE - 1)
                        ? true
                        : false;
            }
        }
        row_no++;
    }

    draw_matrix(forrest);
    draw_matrix(visibility_matrix);

    // scan the rows L-R
    for (int row{0}; auto tree_row : forrest)
    {
        int max_height{0};
        for (int column{0}; auto tree : tree_row)
        {
            if (tree > max_height)
            {
                max_height = tree;
                visibility_matrix[row][column] = true;
            }
            ++column;
        }
        ++row;
    }

    draw_matrix(visibility_matrix);

    // scan the rows R-L
    for (int row{0}; auto tree_row : forrest)
    {
        int max_height{0};
        for (int column{tree_row.size() - 1}; column >= 0; column--)
        {
            if (tree_row[column] > max_height)
            {
                max_height = tree_row[column];
                visibility_matrix[row][column] = true;
            }
        }
        ++row;
    }

    draw_matrix(visibility_matrix);

    // scan the rows T-B
    for (int column{0}; column < forrest[0].size(); column++)
    {
        int max_height{0};
        for (int row{0}; auto tree_row : forrest)
        {
            if (tree_row[column] > max_height)
            {
                max_height = tree_row[column];
                visibility_matrix[row][column] = true;
            }
            ++row;
        }
    }

    draw_matrix(visibility_matrix);

    // scan the rows B-T
    for (int column{0}; column < forrest[0].size(); column++)
    {
        int max_height{0};
        for (int row{forrest.size() - 1}; row >= 0; row--)
        {
            if (forrest[row][column] > max_height)
            {
                max_height = forrest[row][column];
                visibility_matrix[row][column] = true;
            }
        }
    }

    draw_matrix(visibility_matrix);

    int total_visible{0};

    for (auto row : visibility_matrix)
    {
        for (auto t : row)
        {
            if (t)
                ++total_visible;
        }
    }

    int max_scenic_score{0};

    for (auto row{0}; row < MATRIX_SIZE; row++)
    {
        for (auto column{0}; column < MATRIX_SIZE; column++)
        {
            TreePosition position{std::make_pair(row, column)};
            int tree_score = calculate_scenic_score(forrest, position);
            if (tree_score > max_scenic_score)
                max_scenic_score = tree_score;
            scenic_matrix[row][column] = tree_score;
        }
    }

    std::cout << "There are " << total_visible << " trees visible.\n";

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
