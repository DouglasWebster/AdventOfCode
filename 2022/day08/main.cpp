#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <iomanip>

#include <chrono>

#define MATRIX_SIZE 99
// #define DEBUG

using IntVector = std::vector<int>;
using BoolVector = std::vector<bool>;
using ForrestMatrix = std::vector<std::vector<int>>;
using VisibilityMatrix = std::vector<std::vector<bool>>;
using TreePosition = std::pair<int, int>;

#ifdef DEBUG

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
#endif

int calculate_scenic_score(ForrestMatrix &forest, TreePosition position)
{
    int row{position.first};
    int column{position.second};
    int tree_score{1};
    int tree_height{forest[row][column]};

    // look north
    int last_tree{row}; 
    for (int forest_row{row -1}; forest_row >= 0; forest_row--)
    {
        last_tree = forest_row;
        if (forest[forest_row][column] >= tree_height)
            break;
    }

    tree_score *= (row - last_tree);
    
    // look south
    last_tree = row; 
    for (int forest_row{row +1 }; forest_row < MATRIX_SIZE; forest_row++)
    {
        last_tree = forest_row;
        if (forest[forest_row][column] >= tree_height)
            break;
    }

    tree_score *= (last_tree - row);
   

    // look west and assume we can't see north at all
    last_tree = column; 
    for (int forest_column{column - 1}; forest_column >= 0; forest_column--)
    {
        last_tree = forest_column;
        if (forest[row][forest_column] >= tree_height)
            break;
    }

    tree_score *= (column - last_tree);
    
    // look east and assume we can't see north at all
    last_tree = column;
    for (int forest_column{column +1}; forest_column < MATRIX_SIZE; forest_column++)
    {
        last_tree = forest_column;
        if (forest[row][forest_column] >= tree_height)
            break;
    }

    return tree_score *= (last_tree - column);

}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (MATRIX_SIZE == 99) ? "../input.txt" : "../test.txt";
    std::ifstream data{file_name};
    ForrestMatrix forest(MATRIX_SIZE, IntVector(MATRIX_SIZE, 0));
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
                forest[row_no][column] = value;

                visibility_matrix[row_no][column] =
                    (column == 0 || column == MATRIX_SIZE - 1 || row_no == 0 || row_no == MATRIX_SIZE - 1)
                        ? true
                        : false;
            }
        }
        row_no++;
    }

#ifdef DEBUG
    draw_matrix(forest);
    draw_matrix(visibility_matrix);
#endif

    // scan the rows L-R
    for (int row{0}; auto tree_row : forest)
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

#ifdef DEBUG
    draw_matrix(visibility_matrix);
#endif

    // scan the rows R-L
    for (int row{0}; auto tree_row : forest)
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

#ifdef DEBUG
    draw_matrix(visibility_matrix);
#endif

    // scan the rows T-B
    for (int column{0}; column < forest[0].size(); column++)
    {
        int max_height{0};
        for (int row{0}; auto tree_row : forest)
        {
            if (tree_row[column] > max_height)
            {
                max_height = tree_row[column];
                visibility_matrix[row][column] = true;
            }
            ++row;
        }
    }

#ifdef DEBUG
    draw_matrix(visibility_matrix);
#endif

    // scan the rows B-T
    for (int column{0}; column < forest[0].size(); column++)
    {
        int max_height{0};
        for (int row{forest.size() - 1}; row >= 0; row--)
        {
            if (forest[row][column] > max_height)
            {
                max_height = forest[row][column];
                visibility_matrix[row][column] = true;
            }
        }
    }

#ifdef DEBUG
    draw_matrix(visibility_matrix);
#endif

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

    // outside rows have one viewing direction where trees seen == 0.
    // result is multiplicative so don't bother checking them.
    for (auto row{1}; row < MATRIX_SIZE -1; row++)
    {
        for (auto column{1}; column < MATRIX_SIZE -1; column++)
        {
            TreePosition position{std::make_pair(row, column)};
            int tree_score = calculate_scenic_score(forest, position);
            if (tree_score > max_scenic_score)
                max_scenic_score = tree_score;
            scenic_matrix[row][column] = tree_score;
        }
    }

    std::cout << "There are " << total_visible << " trees visible.\n";
    std::cout << "Highest scenic score: " << max_scenic_score << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
