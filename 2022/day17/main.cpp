
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "day17_helpers.h"

#define TESTING false

/// @brief
using Column = std::vector<char>;
using Chamber = std::vector<Column>;

#if TESTING
#define FILE "../test.txt"
#else
#define FILE "../input.txt"
#endif

/// @brief Record the state of the top of the chamber when a
/// @brief row of @@@@@@@@ is at the top of the chamber
struct RepeatRow {
    int_fast64_t chamber_height { 0 };
    int_fast64_t total_rocks_fallen { 0 };
    int_fast64_t repeat_rocks_fallen { 0 };
    Chamber chamber { {}, {}, {}, {}, {}, {}, {} }; // record enough items so that anything falling must come to rest,
};

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
void drop_item(Chamber& chamber, bool drop_or_mark, int_fast64_t current_line, int rows)
{
    for (auto& col : chamber) {
        for (int_fast64_t line_to_check { current_line }; line_to_check < current_line + rows; ++line_to_check)
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
    return chamber[0].size() - 1;
}

/// @brief moves the line left or right if possible then either drops the line by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the current row the line is at.
bool drop_line(Chamber& chamber, char jet, int_fast64_t& current_line)
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

    drop_item(chamber, can_drop, current_line, 1);

    if (can_drop)
        current_line--;
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
    return chamber[0].size() - 1;
}

/// @brief moves the cross left or right if possible then either drops the cross by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_cross(Chamber& chamber, char jet, int_fast64_t& current_line)
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

    drop_item(chamber, can_drop, current_line, 3);
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
    return chamber[0].size() - 1;
}

/// @brief moves the _l left or right if possible then either drops the _l by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_backward_l(Chamber& chamber, char jet, int_fast64_t& current_line)
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
    drop_item(chamber, can_drop, current_line, 3);

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
    return chamber[0].size() - 1;
}

/// @brief moves the tower left or right if possible then either drops the tower by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_tower(Chamber& chamber, char jet, int_fast64_t& current_line)
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

    drop_item(chamber, can_drop, current_line, 4);

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

    return chamber[0].size() - 1;
}

/// @brief moves the tower left or right if possible then either drops the tower by one or marks it at rest
/// @param chamber
/// @param jet
/// @param current_line
/// @return the row that the top of the cross is on
bool drop_square(Chamber& chamber, char jet, int_fast64_t& current_line)
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

    drop_item(chamber, can_drop, current_line, 2);

    current_line += (can_drop) ? 0 : 1;

    return can_drop;
}

int rock_fall_height(Chamber& chamber, int_fast64_t rocks_required, int rock, const std::string& jets, int& jet_index, RepeatRow& first_repeat)
{
    int_fast64_t rocks_fallen { 0 };
    bool repeat_found { false };
    bool rock_can_fall { false };
    int_fast64_t chamber_row { 0 };
    int_fast64_t jets_fired { 0 };

    std::map<int_fast64_t, RepeatRow> repeated_rows;

    while (rocks_fallen < rocks_required && !repeat_found) {
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
            char jet { jets[jet_index++] };
            if (jet_index > jets.length() - 1)
                jet_index = 0;
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
                    clear_top(chamber);
                    if (!rock) {
                        if (chamber_row == chamber[0].size() - 1) { // we have a repeat
                            RepeatRow row_status { chamber_row, rocks_fallen, rocks_fallen };
                            int max_depth { 0 };
                            for (auto col : chamber) {
                                int depth { 1 };
                                for (auto it { col.crbegin() }; *it == ' '; ++it, ++depth)
                                    max_depth = (depth > max_depth) ? depth : max_depth;
                            }
                            int top_row_col { 0 };
                            for (auto col : chamber) {
                                for (auto it = col.end() - max_depth - 1; it != col.end(); ++it)
                                    row_status.chamber[top_row_col].push_back(*it);
                                ++top_row_col;
                            }

                            std::pair<std::map<int_fast64_t, RepeatRow>::iterator, bool> exists;

                            exists = repeated_rows.insert(std::pair<int_fast64_t, RepeatRow>(jets_fired % static_cast<int_fast64_t>(jets.size()), row_status));
                            if (!exists.second) { // we have a match on the index, is the chamber section the same?
                                if (exists.first->second.chamber[0].size() == row_status.chamber[0].size()) { // same size, check contents
                                    for (int index { 0 }; index < 7; ++index) // if contents don't match keep trying
                                        if (exists.first->second.chamber[index] != row_status.chamber[index])
                                            continue;
                                    first_repeat = exists.first->second;
                                    first_repeat.total_rocks_fallen = rocks_fallen;
                                    repeat_found = true;

                                    break;
                                };
                            }
                        }
                    }
                    rock = ++rock % 5;
                }
            }
        }
    }
    clear_top(chamber);

    return static_cast<int>(chamber[0].size());
}

/// @brief
/// @param
/// @param
/// @return
int main(int, char**)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data { FILE };

    std::string jets {};

    if (!data) {
        std::cerr << FILE << " could not be opened for reading\n ";
        return 1;
    }

    while (data && !data.eof()) {
        std::getline(data, jets);
    }

    int_fast64_t rock_to_fall[] { 2022, 1000000000000 };
    int_fast64_t solution[] {0, 0};

    for (int part { 0 }; part < 2; ++part) {

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
            col.reserve(20000);

        int jet_index { 0 };
        int rocks_fallen { 0 };
        bool rock_can_fall = false;
        int rock { 0 };
        int chamber_row { 0 };
        int_fast64_t rocks_required { rock_to_fall[part] };
        int jets_fired(0);
        int repeat_length { 0 };
        int discarded_length { 0 };
        RepeatRow first_repeat {};
        bool repeat_found { false };
        std::map<int, RepeatRow> line_at_rest;

        int chamber_height { rock_fall_height(chamber, rocks_required, rock, jets, jet_index, first_repeat) };
        clear_top(chamber);
        int_fast64_t finish_chamber_start_height { static_cast<int>(first_repeat.chamber[0].size()) };

        if (first_repeat.chamber_height) {
            int_fast64_t chamber_height_at_repeat_start { first_repeat.chamber_height };
            int_fast64_t repeated_height { chamber_height - chamber_height_at_repeat_start - 1 };
            int_fast64_t rocks_fallen_at_repeat_start { first_repeat.repeat_rocks_fallen };
            int_fast64_t rocks_fallen_in_repeat { first_repeat.total_rocks_fallen - rocks_fallen_at_repeat_start };
            int_fast64_t repeat_count { (rocks_required - rocks_fallen_at_repeat_start) / rocks_fallen_in_repeat };
            int_fast64_t rocks_left_to_fall { (rocks_required - rocks_fallen_at_repeat_start) % rocks_fallen_in_repeat };
            int_fast64_t chamber_height_at_end_of_repeat_sequence { chamber_height_at_repeat_start + repeated_height * repeat_count };
            int_fast64_t remaining_height { rock_fall_height(first_repeat.chamber, rocks_left_to_fall, rock + 1, jets, jet_index, first_repeat) - finish_chamber_start_height };
            solution[part] = chamber_height_at_end_of_repeat_sequence + remaining_height;
        } else
            solution[part] = chamber_height - 1;

        std::cout << '\n';
    }

    std::cout << "Part 1; Height of the tower of rocks is " << solution[0] << " units tall.\n";
    std::cout << "Part 2; Height of the tower of rocks is " << solution[1] << " units tall.\n";


    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        end_time - time_start);
    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
