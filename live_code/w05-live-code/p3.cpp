#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

template <typename InIter, typename OutIter>
void mycopy(InIter first, InIter last, OutIter out)
{
  for (; first != last; ++first)
    *out = *first;
}

int main()
{
  using namespace std;

  vector<string> stuff;
  string s;
  mycopy(
    istream_iterator<string>(cin),
    istream_iterator<string>(),
    back_inserter(stuff)
  );

  auto first = stuff.begin();
  auto last = stuff.end();
  for (; first != last; ++first)
  {
    cout << *first << ' ';
  }
  cout << '\n';
}
