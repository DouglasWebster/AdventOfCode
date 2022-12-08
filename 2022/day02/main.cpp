#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

int total_round_score(char opponent, char yours)
{

    switch (yours)
    {
    case 'X':
    {
        if (opponent == 'A')
            return 4;
        return (opponent == 'B') ? 1 : 7;
    }
    case 'Y':
    {
        if (opponent == 'B')
            return 5;
        return (opponent == 'C') ? 2 : 8;
    }
    case 'Z':
    {
        if (opponent == 'C')
            return 6;
        return (opponent == 'A') ? 3 : 9;
    }
    }

    return 0;
}

int correct_round_score(char opponent, char yours)
{
    switch (yours)
    {
    case 'X':
        return (opponent == 'A') ? 3 : (opponent == 'B') ? 1
                                                         : 2;
    case 'Y':
        return (opponent == 'A') ? 4 : (opponent == 'B') ? 5
                                                         : 6;
    case 'Z':
        return (opponent == 'A') ? 8 : (opponent == 'B') ? 9
                                                         : 7;
    }

    return 0;
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

    int total_score{0};
    int correct_score{0};

    while (data)
    {
        std::string strInput;
        std::getline(data, strInput);
        if (strInput != "")
        {
            total_score += total_round_score(strInput[0], strInput[2]);
            correct_score += correct_round_score(strInput[0], strInput[2]);
        }
    }

    std::cout << "Total score: " << total_score << "\n";
    std::cout << "Correct score: " << correct_score << "\n";
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);
    std::cout << "Time taken by program: " << duration.count() << " microseconds" << "\n";
    return 0;
}
