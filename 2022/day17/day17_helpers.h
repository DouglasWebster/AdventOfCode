#ifndef DSW_AOC_DAY17_HELPERS_H
#define DSW_AOC_DAY17_HELPERS_H

#include <vector>
#include <iostream>
#include <deque>

namespace dsw_aoc_day17
{

using Column = std::vector<char>;
using Chamber = std::vector<Column>;
using Jets = std::deque<char>;


  void draw_chimney(Chamber & chimney, int from = 0);

 
}

#endif // DSW_AOC_DAY17_HELPERS_H