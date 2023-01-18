#include "day17_helpers.h"

void dsw_aoc_day17::draw_chimney(Chamber& chamber, int amount)
{
    int last_row = (amount) ? chamber[0].size() - amount : amount;
    int row { static_cast<int>(chamber[0].size() - 1) };
    for (; row >= last_row; --row) {
        for (auto col : chamber) {
            char c = (col[row] == ' ') ? '.' : col[row];
            std::cout << c;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}
