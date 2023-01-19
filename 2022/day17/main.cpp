
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "day17_helpers.h"

#define TESTING false

using Column = std::vector<char>;
using Chamber = std::vector<Column>;
using Jets = std::deque<char>;

#if TESTING
#define FILE "../test.txt"
#else
#define FILE "../input.txt"
#endif

inline char rotate_jets(Jets& jets)
{
    char jet = jets.front();
    jets.pop_front();
    jets.push_back(jet);
    return jet;
}

/// @brief clear all superfluous voids above the top of the uppermost rock
/// @param chamber the chimney under consideration
void clear_top(Chamber& chamber)
{

    while (true) {
        for (auto& col : chamber) {
            if (col.back() != ' ')
                return;
        }

        for (auto& col : chamber)
            col.pop_back();
    }
}

inline void add_gap(Chamber& chamber)
{
    clear_top(chamber);
    for (auto& col : chamber) {
        col.insert(col.end(), { ' ', ' ', ' ' });
    }
}

/// @brief given a chamber and a row check if the moving item has come to rest
/// @param chamber
/// @param row
/// @return false if no further movement is possible
bool check_drop(const Chamber& chamber, int row)
{

    for (auto col : chamber) {
        // require final check for second row of cross shape
        if (col[row] == '@' && col[row - 1] != ' ' && col[row - 1] != '@')
            return false;
    }
    return true;
}

/// @brief scans the tower from the current line and either drops the item or marks it as at rest
/// @param chamber
/// @param drop_or_mark true if the items is to be dropped one row.
/// @param current_line
void drop_item(Chamber& chamber, bool drop_or_mark, int current_line)
{
    for (auto& col : chamber) {
        for (int line_to_check { current_line }; line_to_check < current_line + 4; ++line_to_check)
            if (col[line_to_check] == '@') {
                if (drop_or_mark) {
                    col[line_to_check] = ' ';
                    col[line_to_check - 1] = '@';
                } else {
                    col[line_to_check] = '#';
                }
            }
    }
}

/// @brief adds a @@@@ line to the top of the chamber starting at column 2
/// @param chamber the chimney which gets the new line
/// @return the row in the chimney where the new line is inserted;
int add_line(Chamber& chamber)
{
    add_gap(chamber);
    for (auto index { 0 }; auto& col : chamber) {
        if (index > 1 && index < 6)
            col.push_back('@');
        else
            col.push_back(' ');
        index++;
    }
    // dsw_aoc_day17::draw_chimney(chamber);
    return chamber[0].size() - 1;
}

/// @brief moves the line left or right if possible then either drops the line by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the current row the line is at.
bool drop_line(Chamber& chamber, char jet, int& current_line)
{
    Chamber::iterator col_current = chamber.begin();

    while ((*col_current)[current_line] != '@') {
        col_current++;
    }

    if (jet == '<' && col_current != chamber.begin()) {

        Chamber::iterator col_previous = col_current - 1;
        if ((*col_previous)[current_line] == ' ') {
            for (int index { 0 }; index < 4; ++index) {
                (*col_previous++)[current_line] = '@';
                (*col_current++)[current_line] = ' ';
            }
        }
    }
    if (jet == '>' && col_current < chamber.begin() + 3) {
        col_current += 3;
        Chamber::iterator col_next = col_current + 1;
        if ((*col_next)[current_line] == ' ') {
            for (int index { 0 }; index < 4; ++index) {
                (*col_next--)[current_line] = '@';
                (*col_current--)[current_line] = ' ';
            }
        }
    }

    bool can_drop { check_drop(chamber, current_line) };

    drop_item(chamber, can_drop, current_line);

    if (can_drop)
        current_line--;

    // dsw_aoc_day17::draw_chimney(chamber);
    return can_drop;
}

/// @brief adds the cross shape to the top of the chamber starting at column 2
/// @param chamber
/// @return the row that the top of the cross is on
int add_cross(Chamber& chamber)
{
    add_gap(chamber);
    for (auto index { 0 }; auto& col : chamber) {
        if (index > 1 && index < 5)
            if (index == 3)
                col.insert(col.end(), { '@', '@', '@' });
            else
                col.insert(col.end(), { ' ', '@', ' ' });
        else
            col.insert(col.end(), { ' ', ' ', ' ' });
        index++;
    }
    // dsw_aoc_day17::draw_chimney(chamber);
    return chamber[0].size() - 1;
}

/// @brief moves the cross left or right if possible then either drops the cross by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_cross(Chamber& chamber, char jet, int& current_line)
{
    current_line -= 2; // we want the base, not the top.

    Chamber::iterator col_current = chamber.begin();

    // get the center of the bottom of the cross this time.
    while ((*col_current)[current_line] != '@') {
        col_current++;
    }

    col_current += (jet == '<') ? -1 : 1;

    if (jet == '<' && col_current != chamber.begin()) {

        Chamber::iterator col_previous = col_current - 1;
        if ((*col_previous)[current_line + 1] == ' '
            && (*col_current)[current_line] == ' '
            && (*col_current)[current_line + 2] == ' ') {
            (*col_previous++)[current_line + 1] = '@';
            (*col_previous).erase((*col_previous).begin() + current_line, (*col_previous).begin() + current_line + 3);
            (*col_previous).insert((*col_previous++).begin() + current_line, { '@', '@', '@' });
            (*col_previous).erase((*col_previous).begin() + current_line, (*col_previous).begin() + current_line + 3);
            (*col_previous).insert((*col_previous++).begin() + current_line, { ' ', '@', ' ' });
            (*col_previous)[current_line + 1] = ' ';
        }
    }
    if (jet == '>' && col_current < chamber.end() - 1) {
        Chamber::iterator col_next = col_current + 1;
        if ((*col_next)[current_line + 1] == ' '
            && (*col_current)[current_line] == ' '
            && (*col_current)[current_line + 2] == ' ') {
            (*col_next--)[current_line + 1] = '@';
            (*col_next).erase((*col_next).begin() + current_line, (*col_next).begin() + current_line + 3);
            (*col_next).insert((*col_next--).begin() + current_line, { '@', '@', '@' });
            (*col_next).erase((*col_next).begin() + current_line, (*col_next).begin() + current_line + 3);
            (*col_next).insert((*col_next--).begin() + current_line, { ' ', '@', ' ' });
            (*col_next)[current_line + 1] = ' ';
        }
    }

    bool can_drop { check_drop(chamber, current_line) };
    if (can_drop)
        can_drop = check_drop(chamber, current_line + 1);

    drop_item(chamber, can_drop, current_line);

    // dsw_aoc_day17::draw_chimney(chamber);
    // reset the current line to the top of the cross.
    current_line += (can_drop) ? 1 : 2;

    return can_drop;
}

/// @brief adds the _| shape to the top of the chamber starting at column 2
/// @param chamber
/// @return the row that the top of the cross is on
int add_backward_l(Chamber& chamber)
{
    add_gap(chamber);
    for (auto index { 0 }; auto& col : chamber) {
        if (index > 1 && index < 5)
            if (index == 4)
                col.insert(col.end(), { '@', '@', '@' });
            else
                col.insert(col.end(), { '@', ' ', ' ' });
        else
            col.insert(col.end(), { ' ', ' ', ' ' });
        index++;
    }
    // dsw_aoc_day17::draw_chimney(chamber);
    return chamber[0].size() - 1;
}

/// @brief moves the _l left or right if possible then either drops the _l by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_backward_l(Chamber& chamber, char jet, int& current_line)
{
    current_line -= 2; // we want the base, not the top.

    Chamber::iterator col_current = chamber.begin();

    // get the left hand side of the _l.
    while ((*col_current)[current_line] != '@') {
        col_current++;
    }

    col_current += (jet == '<') ? 0 : 2;

    if (jet == '<' && col_current != chamber.begin()) {
        Chamber::iterator col_previous = col_current - 1;
        if ((*col_previous)[current_line] == ' ' && (*(col_previous + 2))[current_line + 1] == ' '
            && (*(col_previous + 2))[current_line + 2] == ' ') {
            (*col_previous++)[current_line] = '@';
            (*col_previous++)[current_line] = '@';
            (*col_previous).erase((*col_previous).begin() + current_line, (*col_previous).begin() + current_line + 3);
            (*col_previous).insert((*col_previous++).begin() + current_line, { '@', '@', '@' });
            (*col_previous).erase((*col_previous).begin() + current_line, (*col_previous).begin() + current_line + 3);
            (*col_previous).insert((*col_previous).begin() + current_line, { ' ', ' ', ' ' });
        }
    }
    if (jet == '>' && col_current < chamber.end() - 1) {
        Chamber::iterator col_next = col_current + 1;
        if ((*col_next)[current_line] == ' '
            && (*col_next)[current_line + 1] == ' '
            && (*col_next)[current_line + 2] == ' ') {
            (*col_next).erase((*col_next).begin() + current_line, (*col_next).begin() + current_line + 3);
            (*col_next).insert((*col_next--).begin() + current_line, { '@', '@', '@' });
            (*(col_next))[current_line + 1] = ' ';
            (*(col_next))[current_line + 2] = ' ';
            (*(col_next - 2))[current_line] = ' ';
        }
    }

    bool can_drop { check_drop(chamber, current_line) };

    drop_item(chamber, can_drop, current_line);

    // dsw_aoc_day17::draw_chimney(chamber);
    // reset the current line to the top of the _l.
    current_line += (can_drop) ? 1 : 2;

    return can_drop;
}

/// @brief adds the 4 x 1  shape to the top of the chamber starting at column 2
/// @param chamber
/// @return the row that the top of the cross is on
int add_tower(Chamber& chamber)
{
    add_gap(chamber);
    for (auto index { 0 }; auto& col : chamber) {
        if (index == 2)
            col.insert(col.end(), { '@', '@', '@', '@' });
        else
            col.insert(col.end(), { ' ', ' ', ' ', ' ' });
        index++;
    }
    // dsw_aoc_day17::draw_chimney(chamber);
    return chamber[0].size() - 1;
}

/// @brief moves the tower left or right if possible then either drops the tower by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_tower(Chamber& chamber, char jet, int& current_line)
{
    current_line -= 3; // we want the base, not the top.

    Chamber::iterator col_current = chamber.begin();

    // get the left hand side of the _l.
    while ((*col_current)[current_line] != '@') {
        col_current++;
    }

    if (jet == '<' && col_current != chamber.begin()) {
        Chamber::iterator col_previous = col_current - 1;
        if ((*col_previous)[current_line] == ' '
            && (*(col_previous))[current_line + 1] == ' '
            && (*(col_previous))[current_line + 2] == ' '
            && (*(col_previous))[current_line + 3] == ' ') {
            for (int offset { 0 }; offset < 4; ++offset) {
                (*col_previous)[current_line + offset] = '@';
                (*col_current)[current_line + offset] = ' ';
            }
        }
    }
    if (jet == '>' && col_current < chamber.end() - 1) {
        Chamber::iterator col_next = col_current + 1;
        if ((*col_next)[current_line] == ' '
            && (*col_next)[current_line + 1] == ' '
            && (*col_next)[current_line + 2] == ' '
            && (*col_next)[current_line + 3] == ' ') {
            for (int offset { 0 }; offset < 4; ++offset) {
                (*col_next)[current_line + offset] = '@';
                (*col_current)[current_line + offset] = ' ';
            }
        }
    }

    bool can_drop { check_drop(chamber, current_line) };

    drop_item(chamber, can_drop, current_line);

    // dsw_aoc_day17::draw_chimney(chamber);
    // reset the current line to the top of the cross.
    current_line += (can_drop) ? 2 : 3;

    return can_drop;
}

/// @brief adds the 2x2 shape to the top of the chamber starting at column 2
/// @param chamber
/// @return the row that the top of the cross is on
int add_square(Chamber& chamber)
{
    add_gap(chamber);
    for (auto index { 0 }; auto& col : chamber) {
        if (index > 1 && index < 4)
            col.insert(col.end(), { '@', '@' });
        else
            col.insert(col.end(), { ' ', ' ' });
        index++;
    }
    // dsw_aoc_day17::draw_chimney(chamber);
    return chamber[0].size() - 1;
}

/// @brief moves the tower left or right if possible then either drops the tower by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_square(Chamber& chamber, char jet, int& current_line)
{
    current_line -= 1; // we want the base, not the top.

    Chamber::iterator col_current = chamber.begin();

    // get the left hand side of the _l.
    while ((*col_current)[current_line] != '@') {
        col_current++;
    }

    col_current += (jet == '<') ? 0 : 1;

    if (jet == '<' && col_current != chamber.begin()) {
        Chamber::iterator col_previous = col_current - 1;
        if ((*col_previous)[current_line] == ' ' && (*(col_previous))[current_line + 1] == ' ') {
            (*col_previous).erase((*col_previous).begin() + current_line, (*col_previous).begin() + current_line + 2);
            (*col_previous).insert((*col_previous++).begin() + current_line, { '@', '@' });
            col_previous++; // skip a column
            (*col_previous).erase((*col_previous).begin() + current_line, (*col_previous).begin() + current_line + 2);
            (*col_previous).insert((*col_previous).begin() + current_line, { ' ', ' ' });
        }
    }
    if (jet == '>' && col_current < chamber.end() - 1) {
        Chamber::iterator col_next = col_current + 1;
        if ((*col_next)[current_line] == ' ' && (*col_next)[current_line + 1] == ' ') {
            (*col_next).erase((*col_next).begin() + current_line, (*col_next).begin() + current_line + 2);
            (*col_next).insert((*col_next--).begin() + current_line, { '@', '@' });
            col_next--;
            (*col_next).erase((*col_next).begin() + current_line, (*col_next).begin() + current_line + 2);
            (*col_next).insert((*col_next).begin() + current_line, { ' ', ' ' });
        }
    }

    bool can_drop { check_drop(chamber, current_line) };

    drop_item(chamber, can_drop, current_line);

    // dsw_aoc_day17::draw_chimney(chamber);
    // reset the current line to the top of the cross.
    current_line += (can_drop) ? 0 : 1;

    return can_drop;
}

int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data { FILE };
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);

    Jets jets {};

    if (!data) {
        std::cerr << FILE << " could not be opened for reading\n ";
        return 1;
    }

    while (data && !data.eof()) {
        char jet = data.get();
        if (jet == -1)
            break;
        jets.push_back(jet);
    }

    Chamber chamber {
        { '-' },
        { '-' },
        { '-' },
        { '-' },
        { '-' },
        { '-' },
        { '-' }

    };

    // dont slow it down by constant resizing of the vectors (probably overkill)
    for (auto col : chamber)
        col.reserve(10000);

    int rocks_fallen { 0 };
    bool rock_can_fall = false;
    int rock { 0 };
    int chamber_row { 0 };
    int rocks_required { 40022 };
    int jets_fired(0);
    int repeat_length { 0 };
    int discarded_length { 0 };
    

    while (rocks_fallen < rocks_required) {
        if (!rock_can_fall) {
            clear_top(chamber);
            switch (rock) {
            case 0:
                chamber_row = add_line(chamber);
                break;
            case 1:
                chamber_row = add_cross(chamber);
                break;
            case 2:
                chamber_row = add_backward_l(chamber);
                break;
            case 3:
                chamber_row = add_tower(chamber);
                break;
            case 4:
                chamber_row = add_square(chamber);
                break;
            }
            rocks_fallen++;
            rock_can_fall = true;
        }
        while (rock_can_fall) {
            char jet { rotate_jets(jets) };
            jets_fired++;
            if (rock_can_fall) {
                switch (rock) {
                case 0:
                    rock_can_fall = drop_line(chamber, jet, chamber_row);
                    break;
                case 1:
                    rock_can_fall = drop_cross(chamber, jet, chamber_row);
                    break;
                case 2:
                    rock_can_fall = drop_backward_l(chamber, jet, chamber_row);
                    break;
                case 3:
                    rock_can_fall = drop_tower(chamber, jet, chamber_row);
                    break;
                case 4:
                    rock_can_fall = drop_square(chamber, jet, chamber_row);
                    break;

                default:
                    break;
                }
                if (!rock_can_fall) {
                    // is initial rock at rest when we restart jet sequence;
                    if (!repeat_length && !rock && !(jets_fired % jets.size())) {
                        clear_top(chamber);
                        if (chamber_row == chamber[0].size()-1) // we have a repeat
                            repeat_length = chamber_row;
                    }
                    rock = ++rock % 5;
                    if (chamber_row > 9900) {
                        chamber_row -= 9800;
                        discarded_length += 9800;
                        for (auto& col : chamber) {
                            col.erase(col.begin(), col.begin() + 9800);
                        }
                    }
                }
            }
        }
    }

    clear_top(chamber);
    // std::cout << '\n';
    // dsw_aoc_day17::draw_chimney(chamber);
    // std::cout << "next rock " << rock << '\n';
    std::cout << "Repeat length " << repeat_length << '\n';
    std::cout << "Part 1; Height of the tower of rocks is " << chamber[0].size() - 1 + discarded_length << " units tall.\n";

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
