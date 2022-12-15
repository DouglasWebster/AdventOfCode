#ifndef DSW_MONKEY_H
#define DSW_MONKEY_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class Monkey
{
private:
  int m_monkey_number;
  std::vector<int> m_items;
  char m_operation;
  int m_operation_value;
  int m_test_value;
  int m_monkey_if_true;
  int m_monkey_if_false;
  int m_inspections{};

public:
  Monkey(std::string data);
  std::string throw_items();
  void catch_item(int item) { m_items.push_back(item); }
  int inspections() { return m_inspections; }
  ~Monkey();
};

#endif