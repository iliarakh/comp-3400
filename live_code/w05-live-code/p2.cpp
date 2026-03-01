#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>

int main()
{
  using namespace std;

  vector<string> stuff;
  string s;
  copy(
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
