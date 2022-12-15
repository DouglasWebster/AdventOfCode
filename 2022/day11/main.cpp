#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "monkey.h"

#include <chrono>

#define TESTING true

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

    std::vector<Monkey *> monkeys;

    while (data)
    {
        std::string dataLine;
        std::getline(data, dataLine);
        std::string monkeyData;
        while (data && (dataLine != ""))
        {
            monkeyData += dataLine;
            monkeyData += '\n';
            std::getline(data, dataLine);
        }
        Monkey *monkey = new Monkey(monkeyData);
        monkeys.push_back(monkey);
    }

    int max_limit = 1;
    for (auto monkey : monkeys) max_limit *= monkey->get_test_value();
    for (auto monkey : monkeys) monkey->set_max_limit(max_limit);

    const int no_of_monkeys{static_cast<int>(monkeys.size())};

    int inspection_table[no_of_monkeys];
    

    for (int round{1}; round < 10001; ++round)
    {
        for (Monkey *monkey : monkeys)
        {
            std::stringstream thrown{monkey->throw_items()};
            int item{};
            int monkey_index{};
            std::string dummy;
            while (thrown)
            {
                if (thrown.peek() == '!')
                    break;
                thrown >> item >> monkey_index;
                monkeys[monkey_index]->catch_item(item);
                thrown >> std::ws;
            }
        }
        for (int i{}; auto *monkey : monkeys)
        {
            inspection_table[i] = monkey->inspections();
            ++i;
        }
        switch (round)
        {
        case 1:
        case 20:
        case 1000:
        case 2000:
        case 3000:
        case 4000:
        case 5000:
        case 6000:
        case 7000:
        case 8000:
        case 9000:
        case 10000:
            std::cout << std::setw(8)
                      << "after round " << round
                      << " 0: " << inspection_table[0] << ","
                      << " 1: " << inspection_table[1] << ","
                      << " 2: " << inspection_table[2] << ","
                      << " 3: " << inspection_table[3] << '\n';
            break;
        default:
            break;
        }
        if (round == 20)
        {
            int intermediate_inspection_table[no_of_monkeys];
            for (auto index{0}; index < no_of_monkeys; ++index)
                intermediate_inspection_table[index] = inspection_table[index];
            std::sort(intermediate_inspection_table, intermediate_inspection_table + no_of_monkeys);
            std::cout
                << "Level of monkey business is: "
                << static_cast<std::int64_t>(intermediate_inspection_table[2] * intermediate_inspection_table[3]) << '\n';
        }
    }

    std::sort(inspection_table, inspection_table + no_of_monkeys);
    std::int_fast64_t active1{inspection_table[no_of_monkeys - 1]};
    std::int_fast64_t active2{inspection_table[no_of_monkeys - 2]};

    std::int64_t monkey_business{active1 * active2};

    std::cout << "Level of monkey business is: "
              << monkey_business << '\n';
    // clean up after oneself.
    for (auto monkey : monkeys)
        delete monkey;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
