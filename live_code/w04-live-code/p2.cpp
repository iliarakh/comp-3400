#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

int main()
{
  using namespace std;

  vector<string> v;

  string s;
  while (cin >> s)
    v.push_back(s);

  sort(v.begin(), v.end());

  for (auto const& elem : v)
    cout << elem << ' ';
  cout << '\n';
}
