#include <iostream>
#include <vector>
#include <string>

int main()
{
  using namespace std;

  vector<string> stuff;
  string s;
  while (cin >> s)
    stuff.push_back(s);

  auto first = stuff.begin();
  auto last = stuff.end();
  for (; first != last; ++first)
  {
    cout << *first << ' ';
  }
  cout << '\n';
}
