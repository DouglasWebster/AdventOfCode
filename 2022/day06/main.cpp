#include <iostream>
#include <string>
#include <fstream>

#include <chrono>


int get_marker(std::string data, int marker_length)
{
    int data_length = data.length();
    if (data_length < marker_length)
        return 0; // string isn't long enough to check;

    for (auto count{0}; count < (data_length - marker_length); ++count)
    {
        bool match = false;
        char *data_start{&data[count]};
        for (int i{0}; i < marker_length; ++i)
        {
            for (int j{i + 1}; j < marker_length; ++j)
            {
                if (*(data_start + i) == *(data_start + j))
                {
                    match = true;
                    break;
                }
            }
            if (match)
                break;
        }
        if (match)
            continue;
        

        return count + marker_length;
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

    while (data)
    {
        std::string strInput;
        std::getline(data, strInput);
        int packet_pos = get_marker(strInput, 4);
        if (packet_pos > 0)
            std::cout << "Position of first packet is: " << packet_pos << "\n";
        int start_of_message = get_marker(strInput, 14);
        if (start_of_message > 0)
            std::cout << "Position of message start is: " << start_of_message << "\n";
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
