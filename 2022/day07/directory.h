#ifndef AOC2022_DIRECTORY_H
#define AOC2022_DIRECTORY_H

#include <vector>
#include <map>
#include <sstream>
#include <fstream>

class Directory
{
private:
  std::string m_name{};
  std::map<std::string, int> m_files;
  std::map<std::string, Directory *> m_directories;
  std::string m_path;
  int m_size{0};

public:
  Directory(std::string name, std::string path = "") : m_name{name}
  {
    m_files.clear();
    m_directories.clear();
    m_path = (name == "/") ? "/" : path + name;
  };

  void do_command(std::ifstream &commands)
  {
    while (commands)
    {
      std::string cmd;
      std::getline(commands, cmd, ' '); // eat white space before command
      std::string next_cmd{"  "};
      commands.get(next_cmd[0]);
      commands.get(next_cmd[1]);

      if (next_cmd == "ls")
      {
        std::getline(commands, cmd); // eat any remnants on the line
        do_ls(commands);
      }
      if (next_cmd == "cd")
      {
        bool finished = do_cd(commands);
        if (finished)
          return;
      }
    }
  }

  int get_dir_size(std::map<std::string, int> &dir_list)
  {
    int sub_dir_size{0};
    for (auto d : m_directories)
    {
      sub_dir_size += d.second->get_dir_size(dir_list);
    }

    int dir_size{0};
    for (auto f : m_files)
    {
      dir_size += f.second;
    }

    m_size += sub_dir_size + dir_size;
    dir_list.insert(dir_list.begin(), std::pair<std::string, int>(m_path, m_size));

    return m_size;

  }

private:
  void do_ls(std::ifstream &commands)
  {
    while (commands && (commands.peek() != '$'))
    {
      char cmd_start = commands.peek();
      if (cmd_start == 'd')
      {
        add_dir(commands);
      }

      if (isdigit(cmd_start))
      {
        add_file(commands);
      }

      std::string eat;
      std::getline(commands, eat);
    }
  }

  bool do_cd(std::ifstream &commands)
  {
    std::string dir_name;
    commands >> dir_name;
    if (dir_name == "..")
      return true;
    Directory *d = m_directories[dir_name];
    d->do_command(commands);
    return false;
  }

  void add_dir(std::ifstream &commands)
  {
    std::string name;
    std::string dummy;
    std::getline(commands, dummy, ' ');
    commands >> name;

    std::string path = (m_path == "/") ? m_path : m_path + "/";

    Directory *d = new Directory(name, path);
    m_directories.insert(std::pair<std::string, Directory *>(name, d));
  }

  void add_file(std::ifstream &commands)
  {
    int size;
    std::string name;
    commands >> size >> name;
    m_files.insert(std::pair<std::string, int>(name, size));
  }
};

#endif