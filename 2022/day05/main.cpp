#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <map>
#include <sstream>
#include <deque>

void set_stacks(std::string line, std::map<int, std::deque<char>> &stacks)
{
    int numberOfStacks = stacks.size();
    // int lineLength = line.length();
    for (int i{0}; i < numberOfStacks; i++)
    {
        char &crate = line.at(i * 4 + 1);
        if (crate != ' ')
            stacks.at(i).push_front(crate);
    }
}

void decode_move(std::string move, int &n, int &s, int &d)
{
    std::stringstream ss(move);
    std::string dummyString;
    char dummyChar;
    std::getline(ss, dummyString, ' ');
    ss >> n >> dummyChar;
    std::getline(ss, dummyString, ' ');
    ss >> s >> dummyChar;
    s--;
    std::getline(ss, dummyString, ' ');
    ss >> d;
    d--;
}

void move_single_crates(std::string d, std::map<int, std::deque<char>> &stacks)
{
    int noToMove{0};
    int source{0};
    int destination{0};
    decode_move(d, noToMove, source, destination);
    std::deque<char> &sourceStack = stacks.at(source);
    std::deque<char> &destStack = stacks.at(destination);

    for (auto i{0}; i < noToMove; i++)
    {
        char crate{sourceStack.back()};
        sourceStack.pop_back();
        destStack.push_back(crate);
    }
}

void move_multiple_crates(std::string d, std::map<int, std::deque<char>> &stacks)
{
    int noToMove{0};
    int source{0};
    int destination{0};
    decode_move(d, noToMove, source, destination);
    std::deque<char> &sourceStack = stacks.at(source);
    std::deque<char> &destStack = stacks.at(destination);
    std::deque<char>::iterator it = destStack.end();
    std::string crates{};
    for(auto i{0}; i < noToMove; i++) {
        crates += sourceStack.back();
        sourceStack.pop_back();
    }

    for (auto i{crates.length()}; i > 0; --i) {
        destStack.push_back(crates[i-1]);
    }
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

    std::string strInput;
    std::getline(data, strInput);
    std::map<int, std::deque<char>> stacks_single;
    int numberOfStacks = (strInput.length() + 1) / 4;

    for (auto i{0}; i < numberOfStacks; i++)
    {
        stacks_single.insert(std::pair<int, std::deque<char>>(i, std::deque<char>()));
    }

    do
    {
        if (strInput == "")
            break;
        if (isdigit(strInput[1]))
        {
            std::getline(data, strInput);
            continue;
        }
        set_stacks(strInput, stacks_single);
        std::getline(data, strInput);
    } while (data);

    std::map<int, std::deque<char>> stacks_multiple = stacks_single;

    while (data)
    {
        std::getline(data, strInput);
        if (strInput == "")
            break;
        move_single_crates(strInput, stacks_single);
        move_multiple_crates(strInput, stacks_multiple);
    }
    std::string singleTopCrates;
    for (auto &[key, value] : stacks_single)
    {
        singleTopCrates += value.back();
    }

    std::string multipleTopCrates;
    for (auto &[key, value] : stacks_multiple)
    {
        multipleTopCrates += value.back();
    }

    std::cout << "Single move top crates: " << singleTopCrates << '\n';
    std::cout << "Multiple move top crates: " << multipleTopCrates << '\n';

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);
    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
