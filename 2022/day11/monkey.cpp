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
      ;
    break;
  }
  ss >> dummy >> dummy >> dummy >> m_operation >> dummy;
  m_operation_value = (dummy == "old") ? -1 : stoi(dummy);
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
    if (m_operation == '+')
    {
      if (m_operation_value == -1)
      {
        item *= 2;
      }
      else
      {
        item += m_operation_value;
      }
    }
    if (m_operation == '*')
    {
      if (m_operation_value == -1)
      {
        item *= item;
      }
      else
      {
        item *= m_operation_value;
      }
    }
    m_items.erase(m_items.begin());
    item /= 1;
    bool not_worried = (item % m_test_value);

    item_thrown << item << " " << ((not_worried == false) ? m_monkey_if_true : m_monkey_if_false) << '\n';
    thrown += item_thrown.str();
    ++m_inspections;
  }
  thrown += '!'; // end of output marker

  return thrown;
}

Monkey::~Monkey()
{
}