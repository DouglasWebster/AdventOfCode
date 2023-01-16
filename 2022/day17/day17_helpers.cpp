#include "day17_helpers.h"

  void dsw_aoc_day17::draw_chimney(Chamber & chamber){
    int row{static_cast<int>(chamber[0].size() -1)};
    for(; row >= 0; --row ) {
      for (auto col: chamber) {
        char c = (col[row] == ' ') ? '.' : col[row];
        std::cout << c;
      }
      std::cout << '\n';
    }
    std::cout << '\n';

  }
