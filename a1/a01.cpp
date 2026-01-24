#include <filesystem>
#include <generator>
#include <iostream>
#include <queue>

namespace fs = std::filesystem;

bool is_direct_child_path_of(fs::path const &possible_child_path,
                             fs::path const &known_path)
{
  auto cpath = fs::canonical(possible_child_path);

  if (!cpath.has_parent_path())
    return true;

  auto known_canonical_path = fs::canonical(known_path);
  return cpath.parent_path() == known_canonical_path;
}

std::generator<fs::path> bfs_scan(fs::path root_path)
{
  if (fs::symlink_status(root_path).type() != fs::file_type::directory)
  {
    co_yield root_path;
    co_return;
  }

  std::queue<fs::path> q;
  q.push(root_path);

  while (!q.empty())
  {
    fs::path current_dir = q.front();
    q.pop();

    for (fs::directory_entry const &entry : fs::directory_iterator(current_dir))
    {
      fs::path p = entry.path();

      if (entry.is_symlink())
      {
        if (is_direct_child_path_of(p, current_dir))
          co_yield p;
        continue;
      }

      fs::file_type t = entry.status().type();
      if (t == fs::file_type::directory)
        q.push(p);
      else
        co_yield p;
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    std::cerr << "Usage: " << argv[0] << " PATH [PATH]...\n";
    return 1;
  }

  for (int i = 1; i < argc; ++i)
  {
    try
    {
      std::cerr << "Processing path " << argv[i] << '\n';

      for (fs::path const &file_name : bfs_scan(fs::path{argv[i]}))
      {
        std::cout << file_name << '\n';
      }
    }
    catch (fs::filesystem_error const &ex)
    {
      std::cerr << "EXCEPTION: path: " << argv[i] << ", reason: " << ex.what() << '\n';
    }
    catch (...)
    {
      std::cerr << "EXCEPTION: Unknown exception.\n";
    }
  }

  return 0;
}
