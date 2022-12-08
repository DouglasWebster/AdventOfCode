#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>

// std::vector<int> convert_to_vector(std::string pairs)
// {
//     std::vector<int> sections(4);
//     std::smatch m;
//     std::regex e("\\d+");
//     int index{0};
//     while (std::regex_search(pairs, m, e))
//     {
//         for (std::string x : m)
//             sections[index++] = stoi(x);
//         pairs = m.suffix().str();
//     }

//     return sections;
// }

std::vector<int> convert_to_vector(std::string pairs)
{
    std::vector<int> sections(4);
    std::stringstream ss;
    char dummy;
    ss << pairs;
    ss >> sections[0] >> dummy >> sections[1] >> dummy >> sections[2] >> dummy >> sections[3];
    return sections;
}
inline bool check_pair(std::vector<int> sections)
{
    if ((sections[0] <= sections[2]) && (sections[1] >= sections[3]))
        return true;
    if ((sections[2] <= sections[0]) && (sections[3] >= sections[1]))
        return true;

    return false;
}

inline bool no_overlap(std::vector<int> sections)
{
    return sections[0] < sections[2] ? sections[1] < sections[2] : sections[0] > sections[3];
}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::ifstream data{"../input.txt"};

    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n ";
        return 1;
    }

    int areas[6][4];

    std::vector<std::vector<int>> pairs(1000);
    int total_fully_contained{0};
    int overlapped_pairs{0};
    int area{0};
    while (data)
    {
        std::string strInput;
        std::getline(data, strInput);
        if (strInput != "")
        {
            pairs[area++] = convert_to_vector(strInput);

            // convert_to_array(strInput, areas[area]);
        }
    }

    for (auto p : pairs)
    {
        if (check_pair(p))
            ++total_fully_contained;
        if (!no_overlap(p))
            ++overlapped_pairs;
    }

    std::cout << "No of fully contained pairs is " << total_fully_contained << '\n';
    std::cout << "No of overlapped pairs is " << overlapped_pairs << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);
    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
