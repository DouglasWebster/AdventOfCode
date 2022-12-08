#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

bool reverse_sort(int i, int j) { return (i > j); }

int main()
{
    std::ifstream data{"../input.txt"};
    std::stringstream os;
    std::vector<int> elves{};
    elves.reserve(3000);


    if (!data)
    {
        std::cerr << "input.txt could not be opened for reading\n";
        return 1;
    }
    int max_value{0};
    int current_value{0};

    while (data)
    {
        int nvalue;
        std::string strInput;
        std::getline(data, strInput);
        if (strInput == "")
        {
            elves.push_back(current_value);
            current_value = 0;
        }
        else
        {
            os << strInput;
            os >> nvalue;
            current_value += nvalue;
            os.str("");
            os.clear();
        }
    }

    std::sort(elves.begin(), elves.end(), reverse_sort);

    std::cout << "max_value: " << elves[0] << " calories\n";
    std::cout << "2nd most: " << elves[1] << " calories\n";
    std ::cout << "3rd most: " << elves[2] << " calories\n";

    std::cout << "Top 3 elves are carrying " << elves[0] + elves[1] + elves[2] << " calories\n";

    return 0;

    // When inf goes out of scope, the ifstream
    // destructor will close the file
}
