#include <cstddef>
#include <iostream>
#include <map>
#include <print>
#include <set>
#include <string>

auto invert(std::map<std::string,std::size_t> const& m)
{
  using namespace std;

  map<size_t,set<string>> retval;
  for (auto const& elem : m)
  {
    auto pos{ retval.find(elem.second) };
    if (pos == retval.end())
      retval[elem.second].insert(elem.first);
    else
      pos->second.insert(elem.first);
  }
  return retval;
}

int main()
{
  using namespace std;

  string start;
  string stop;
  if (not (cin >> start >> stop))
  {
    cerr << "Invalid input. Aborting...\n";
    return 1;
  }

  map<string,size_t> hist;
  for (string s; cin >> s; )
    ++hist[s];

  println("orig: {}",hist);
  println("inverted: {}",invert(hist));

  set<size_t> freqs;
  auto first{ hist.lower_bound(start) };
  auto last{ hist.upper_bound(stop) };
  for (; first != last; ++first)
    freqs.insert(first->second);

  print("In [{},{})", start, stop);

  if (not freqs.empty())
    println(" frequencies vary between {} and {} inclusively.",
      *freqs.begin(), *prev(freqs.end()));
  else
    println(" there are no data points.");
}
