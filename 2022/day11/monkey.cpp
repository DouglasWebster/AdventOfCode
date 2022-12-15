#include "monkey.h"

Monkey::Monkey(std::string data)
{
  std::cout << "monkey data: " << data << '\n';
  std::stringstream ss(data);
  std::string dummy;
  ss >> dummy >> m_monkey_number;
  ss >> dummy >> dummy >> dummy; // eat up to starting item numbers
  while (ss)
  {
    int value{};
    ss >> value;
    m_items.push_back(value);
    ss >> dummy;
    if (dummy == ",")
      continue;
    if (dummy == "Operation:")
      break;
  }
  ss >> dummy;
  ss >> dummy;
  ss >> dummy;
  ss >> m_operation;
  ss >> dummy;
  if (dummy == "old")
  {
    m_operation = "old";
    m_operation_value = 1;
  }
  else
    m_operation_value = stoi(dummy);
  ss >> dummy >> dummy >> dummy >> m_test_value;
  ss >> dummy >> dummy >> dummy >> dummy >> dummy >> m_monkey_if_true;
  ss >> dummy >> dummy >> dummy >> dummy >> dummy >> m_monkey_if_false;
}

std::string Monkey::throw_items()
{
  std::string thrown;
  while (m_items.size())
  {
    std::stringstream item_thrown;
    int item = m_items.front();
    if (m_operation == "old")
      item *= item;
    if (m_operation == "+")
      item += m_operation_value;
    if (m_operation == "*")
      item *= m_operation_value;
    // {
    //   if (m_operation_value == -1)
    //   {
    //     item *= m_operation_value;
    //   }
    //   else
    //   {
    //     item += m_operation_value;
    //   }
    // }
    // if (m_operation == '*')
    // {
    //   if (m_operation_value == -1)
    //   {
    //     item *= item;
    //   }
    //   else
    //   {
    //     item *= m_operation_value;
    //   }

    m_items.erase(m_items.begin());

    if (m_restrict_item)
      item %= m_max_item_limit;
    else
      item /= 3;

    bool not_worried = (item % m_test_value);
    int receiving_monkey{-1};

    receiving_monkey = (not_worried == false) ? m_monkey_if_true : m_monkey_if_false;
    if (not_worried)
      ++m_thrown_to_false;
    else
      ++m_thrown_to_false;

    item_thrown << item << " " << receiving_monkey << '\n';
    thrown += item_thrown.str();
    ++m_inspections;
  }
  thrown += '!'; // end of output marker

  return thrown;
}

Monkey::~Monkey()
{
}