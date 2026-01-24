#include <filesystem>
#include <generator>
#include <iostream>
#include <queue>

namespace fs = std::filesystem; // alias for std::filesystem

// Returns true if possible_child_path is a direct child of known_path
bool is_direct_child_path_of(const fs::path &possible_child_path,
                             const fs::path &known_path)
{
  // Convert possible_child_path to its canonical (absolute, normalized) path
  auto cpath = fs::canonical(possible_child_path);

  // If the path has no parent (e.g., root), treat it as a direct child
  if (!cpath.has_parent_path())
    return true;

  // Convert known_path to its canonical form for reliable comparison
  auto known_canonical_path = fs::canonical(known_path);

  // Check whether known_path is the direct parent of possible_child_path
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
