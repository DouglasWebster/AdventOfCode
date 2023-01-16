#ifndef DSW_AOC_DAY17_HELPERS_H
#define DSW_AOC_DAY17_HELPERS_H

#include <vector>
#include <iostream>

namespace dsw_aoc_day17
{

using Column = std::vector<char>;
using Chamber = std::vector<Column>;

  void draw_chimney(Chamber & chimney);
}

#endif // DSW_AOC_DAY17_HELPERS_H