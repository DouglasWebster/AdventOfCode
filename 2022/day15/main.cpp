#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

#include <deque>

#include <chrono>

using Point = std::pair<int, int>;
using Sensor = Point;
using Beacon = Point;
using Beacons = std::vector<Beacon>;
using Sensors = std::vector<Sensor>;
using MapItem = std::pair<Sensor, int>;
using RangeMap = std::map<Sensor, int>;
using LineRanges = std::vector<std::pair<int, int>>;

#define TESTING true

bool compare_ranges(std::pair<int, int> first, std::pair<int, int> second) {
    return (first.first < second.first) ? true : false;
} 


bool parse_positions(std::string input, Sensors &sensors, Beacons &beacons, RangeMap &sensor_map)
{
    std::stringstream ss(input);
    std::string dummy;
    Sensor sensor;
    Beacon beacon;

    std::getline(ss, dummy, '=');
    ss >> sensor.first;
    std::getline(ss, dummy, '=');
    ss >> sensor.second;
    std::getline(ss, dummy, '=');
    ss >> beacon.first;
    std::getline(ss, dummy, '=');
    ss >> beacon.second;

    int manhattan_distance = abs(sensor.first - beacon.first) + abs(sensor.second - beacon.second);

    std::pair<RangeMap::iterator, bool> it = sensor_map.insert(MapItem(sensor, manhattan_distance));
    if (it.second = false)
        return true; // duplicate sensors!

    sensors.push_back(sensor);
    beacons.push_back(beacon);
    return false;
}

LineRanges row_coverage(const RangeMap &ranges, int row)
{
    int min_x;
    int max_x;

    LineRanges coverage;

    for (const auto &sensor : ranges)
    {
        int y_distance = abs(sensor.first.second - row);
        int x_offset = sensor.second - y_distance;

        int sensor_min_x = sensor.first.first - x_offset;
        int sensor_max_x = sensor.first.first + x_offset;
        coverage.push_back(std::pair(sensor_min_x, sensor_max_x));
    }

    // sort coverage
    std::sort(coverage.begin(), coverage.end(), compare_ranges);

    // merge overlaps
    bool did_merge{true};

    while(did_merge) {
        did_merge = false;
        for (auto index{0}; index < coverage.size() - 1; ++index)
            if(coverage[index+1 < compare_ranges[index])
    }


    return coverage;

}

int main(int, char **)
{
    auto time_start = std::chrono::high_resolution_clock::now();
    std::string file_name = (TESTING) ? "../test.txt" : "../input.txt";
    std::ifstream data{file_name};

    Sensors sensors;
    Beacons beacons;
    RangeMap sensor_ranges;

    bool duplicate_sensors{false};
    while (data)
    {
        std::string input;
        std::getline(data, input);
        if (input == "")
            break;
        duplicate_sensors = parse_positions(input, sensors, beacons, sensor_ranges);
        if (duplicate_sensors)
        {
            std::cout << "Something went wrong - we have duplicate sensors!\n";
            return 1;
        }
    }

    for (const auto &range : sensor_ranges)
        std::cout << range.first.first << "," << range.first.second << " distance: " << range.second << '\n';

    // set the limits of the sensor to the location of the first sensor
    std::pair<int, int> x_range{};
    std::pair<int, int> y_range{sensors[0].second, sensors[0].second};

    for (auto sensor : sensors)
    {
        if (sensor.first < x_range.first)
            x_range.first = sensor.first;
        if (sensor.first > x_range.second)
            x_range.second = sensor.first;
        if (sensor.second < y_range.first)
            y_range.first = sensor.second;
        if (sensor.second > y_range.second)
            y_range.second = sensor.second;
    }

    for (auto beacon : beacons)
    {
        if (beacon.first < x_range.first)
            x_range.first = beacon.first;
        if (beacon.first > x_range.second)
            x_range.second = beacon.first;
        if (beacon.second < y_range.first)
            y_range.first = beacon.second;
        if (beacon.second > y_range.second)
            y_range.second = beacon.second;
    }

    LineRanges line_ranges = row_coverage(sensor_ranges, 10);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - time_start);

    std::cout << "Time taken by program: " << duration.count() << " microseconds"
              << "\n";
    return 0;
}
