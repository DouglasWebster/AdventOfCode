#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

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


    for (int round{}; round <10000; ++round)
    {
        for (Monkey *monkey : monkeys)
        {
            std::stringstream thrown{monkey->throw_items()};
            int item{};
            int monkey_index{};
            std::string dummy;
            while (thrown)
            {
                if(thrown.peek() == '!') break;
                thrown >> item >> monkey_index;
                monkeys[monkey_index]->catch_item(item);
                thrown >> std::ws;
            }
        }
    }

    int no_of_monkeys{monkeys.size()};

    int inspection_table[no_of_monkeys];

    for(int i{}; auto *monkey : monkeys) {
        inspection_table[i] = monkey->inspections();
        ++ i;
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
