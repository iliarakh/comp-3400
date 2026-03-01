#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

template <typename InIter, typename OutIter, typename UnaryFunc>
void mytransform(InIter first, InIter last, OutIter out, UnaryFunc f)
{
  for (; first != last; ++first)
    *out = f(*first);
}

int main()
{
  using namespace std;

  vector<string> stuff;
  string s;
  mytransform(
    istream_iterator<string>(cin),
    istream_iterator<string>(),
    back_inserter(stuff),
    [](std::string s)
    {
      std::string s_copy;
      for (char c : s)
        s_copy.push_back(toupper(c));
      return s_copy;
    }
  );

  copy(
    stuff.begin(), stuff.end(),
    ostream_iterator<string>(cout, " ")
  );
  cout << '\n';
}
