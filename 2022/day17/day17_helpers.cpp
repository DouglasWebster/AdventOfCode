#include "day17_helpers.h"

void dsw_aoc_day17::draw_chimney(Chamber& chamber, int amount)
{
    int last_row = (amount) ? chamber[0].size() - amount : amount;
    int row { static_cast<int>(chamber[0].size() - 1) };
    for (; row >= last_row; --row) {
        std::cout << ' ';
        for (auto col : chamber) {
            char c = (col[row] == ' ') ? '.' : col[row];
            std::cout << c;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void dsw_aoc_day17::draw_chimney(Chamber& chamber, std::string file_name, char jet, int amount)
{
    std::ofstream out_file { file_name};
    int last_row = (amount) ? chamber[0].size() - amount : amount;
    int row { static_cast<int>(chamber[0].size() - 1) };
    out_file << "Next jet to fire is: " << jet << '\n';
    for (; row >= last_row; --row) {
        out_file << '\t';
        for (auto col : chamber) {
            char c = (col[row] == ' ') ? '.' : col[row];
            out_file << c;
        }
        out_file << '\n';
    }
    std::cout << '\n';


}
