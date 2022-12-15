#ifndef DSW_MONKEY_H
#define DSW_MONKEY_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

class Monkey
{
private:
  int m_monkey_number;
  std::vector<int> m_items;
  std::string m_operation;
  int m_operation_value;
  int m_test_value;
  int m_monkey_if_true;
  int m_monkey_if_false;
  int m_inspections{};
  int m_mod_factor{};
  int m_thrown_to_false{};
  int m_thrown_to_true{};
  int m_max_item_limit {0};
  bool m_restrict_item{true};
public:
  Monkey(std::string data);
  std::string throw_items();
  void catch_item(int item) { m_items.push_back(item); }
  int inspections() { return m_inspections; }
  void set_max_limit(int limit) { m_max_item_limit = limit;}
  int get_test_value() { return m_test_value;}
  ~Monkey();
};

#endif