#include <filesystem>
#include <generator>
#include <iostream>
#include <queue>

bool is_direct_child_path_of(std::filesystem::path const &possible_child_path,
                             std::filesystem::path const &known_path)
{

  namespace fs = std::filesystem;

  auto cpath{fs::canonical(possible_child_path)};

  // consider a path that does not have a parent path to be under known_path...
  if (not cpath.has_parent_path())
    return true;

  auto parent_cpath{cpath.parent_path()};

  // otherwise ensure possible_child_path's parent path is known_path
  // and use canonical() to determine this...
  auto known_canonical_path{fs::canonical(known_path)};
  return parent_cpath == known_canonical_path;
}

std::generator<std::filesystem::path> bfs_scan(std::filesystem::path root_path)
{

  std::queue<std::filesystem::path> q;

  q.push(root_path);

  while (q.empty()) // if queue is not empty continue 
  {

    std::filesystem::path current_dir = q.front();
    q.pop();
  }
  
}

int main(int argc, char *argv[])
{

  if (argc == 1)
  {
    std::cerr << "Usage" << argv[0] << "PATH[PATH]...";

    return 1;
  }

  for (int i = 0; i < argc; i++)
  {
    try
    {
    }
    catch ()
    {
    }
  }
}