#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>

#include <chrono>

#define TESTING false

using PacketPair = std::pair<std::string, std::string>;
using PacketQueue = std::deque<std::string>;

void parse_packet(PacketQueue &packet, std::string input)
{
    std::stringstream ss(input);
    int digit_holder;
    char char_holder;
    while (ss.peek() != EOF)
    {
        if (isdigit(ss.peek()))
        {
            ss >> digit_holder;
            packet.emplace_back(std::to_string(digit_holder));
        }
        else
        {
            ss >> char_holder;
            packet.emplace_back(std::string(1, char_holder));
        }
    }
}

bool check_order(std::string upper_packet, std::string lower_packet)
{
    PacketQueue packet_1, packet_2;

    parse_packet(packet_1, upper_packet);
    parse_packet(packet_2, lower_packet);

    while (packet_1.size() && packet_2.size())
    {
        std::string packet_1_next = packet_1.front();
        std::string packet_2_next = packet_2.front();

        bool same_char = (packet_1_next == packet_2_next);
        if (same_char)
        {
            packet_1.pop_front();
            packet_2.pop_front();
            continue;
        }

        if (isdigit(packet_1_next[0]) && isdigit(packet_2_next[0]))
            return (std::stoi(packet_1_next) < std::stoi(packet_2_next));
        if (packet_1_next == "]" || packet_2_next == "]")
            return (packet_1_next == "]") ? true : false;

        if (isdigit(packet_1_next[0]))
        {
            packet_1.pop_front();
            packet_1.push_front("]");
            packet_1.push_front(packet_1_next);
            packet_1.push_front("[");
            continue;
        }
        if (isdigit(packet_2_next[0]))
        {
            packet_2.pop_front();
            packet_2.push_front("]");
            packet_2.push_front(packet_2_next);
            packet_2.push_front("[");
            continue;
        }
    }
    return false;
}

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

    std::vector<std::string> inputs;
    while (data)
    {
        std::string packet;
        std::getline(data, packet);
        if (packet == "")
            continue;
        inputs.push_back(packet);
    }

    int indices_sum{0};

    for (auto index{0}, indices{1}; index < inputs.size(); index += 2, ++indices)
    {
        bool in_order = check_order(inputs[index], inputs[index + 1]);
        if (in_order)
            indices_sum += indices;
        // std::cout << "Packet pair " << indices << " returned " << in_order << std::endl;
    }

    std::string decoders_key_1 = "[[2]]";
    std::string decoders_key_2 = "[[6]]";

    inputs.push_back(decoders_key_1);
    inputs.push_back(decoders_key_2);
    std::sort(inputs.begin(), inputs.end(), check_order);

    int decoder_key{1};
    for(auto index{1}; auto input : inputs) {
        if (input == decoders_key_1 || input == decoders_key_2) decoder_key *= index;
        ++index;
    }

    std::cout << "Sum of the indices of the pairs is " << indices_sum << '\n';
    std::cout << "The decoder key for the distress signal is " << decoder_key << '\n';




    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
