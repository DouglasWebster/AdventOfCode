#include <iostream>
#include <string>
#include <fstream>
#include <string_view>
#include <sstream>
#include <vector>

#include <chrono>

#include "directory.h"

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data{"../input.txt"};

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    Directory home("/");

    while (data)
    {
        std::string strInput;
        std::getline(data, strInput);
        if (strInput != "")
        {

            home.do_command(data);
        }
    }

    std::map<std::string, int> dir_list;
    // std::vector<std::string, int>::iterator it;

    home.get_dir_size(dir_list);

    int part_a_total{0};
    for (auto d : dir_list)
    {

        std::cout << d.first << ": " << d.second << '\n';
        if (d.second <= 100000)
            part_a_total += d.second;
    }

    std::cout << "Sum of directories whose total is at most 10000: " << part_a_total << '\n';

    int total_file_size = dir_list["/"];
    int free_space = 70000000 - total_file_size;
    int delete_required = 30000000 - free_space;

    std::cout << "current size: " << total_file_size << ", free_space: " << free_space
              << ", minimum deletion required: " << delete_required << '\n';

    int deleted_dir_size{total_file_size};
    std::string deleted_dir{""};

    for (auto d : dir_list)
    {
        if (d.second > delete_required && d.second < deleted_dir_size)
            deleted_dir_size = d.second;
        deleted_dir = d.first;
    }

    std::cout << "deleted: " << deleted_dir << " freeing: " << deleted_dir_size << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
