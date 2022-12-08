#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

int get_pack_priority(std::string pack)
{
    std::map<char, int> check_map;
    auto half_len = pack.length() / 2;
    auto first_half = pack.substr(0, half_len);
    auto second_half = pack.substr(half_len);

    for (auto pack_item : first_half)
        check_map.insert(std::pair<char, int>(pack_item, 1));

    for (auto pack_item : second_half)
    {
        std::map<char, int>::iterator it = check_map.find(pack_item);
        if (it != check_map.end())
            return pack_item < 'a' ? pack_item - 'A' + 27 : pack_item - 'a' + 1;
    }
    return 0;
}

int get_badge_priority(std::vector<std::string> group)
{
    std::map<char, int> badge_map;
    std::pair<std::map<char, int>::iterator, bool> ret;
    
    for (auto pack : group)
    {
    std:
        sort(pack.begin(), pack.end());
        auto last = std::unique(pack.begin(), pack.end());
        pack.erase(last, pack.end());

        for (auto c : pack)
        {
            ret = badge_map.insert(std::pair<char, int>(c, 1));
            if (ret.second == false)
                ++ret.first->second;
            if (ret.first->second == 3)
                return ret.first->first < 'a' ? ret.first->first - 'A' + 27 : ret.first->first - 'a' + 1;
        }
    }

    return 0;
}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data{"../input.txt"};
    std::vector<std::string> packs;
    packs.reserve(500);

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    int priority_total{0};
    int badge_total(0);
    while (data)
    {
        std::string strInput;
        std::getline(data, strInput);
        if (strInput != "")
        {
            packs.push_back(strInput);
            priority_total += get_pack_priority(strInput);
            if ((packs.size() % 3) == 0)
                badge_total += get_badge_priority({packs.end() - 3, packs.end()});
        }
    }

    std::cout << "Sum of priorities: " << priority_total << "\n";
    std::cout << "Sum of badge priorities: " << badge_total << "\n";

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);
    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
