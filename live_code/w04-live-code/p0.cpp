// This program was added and was not created live in class.
// This program has comments added in it to help with understanding
// p1.cpp.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  // read in all int values from stdin into a vector...
  std::vector<int> v;
  for (int i; std::cin >> i; )
    v.push_back(i);

  // NOTE: If entering directly from the keyboard, type at the start of a line
  //       (i.e., after hitting the ENTER key):
  //         * Ctrl-Z if using MS-DOS/Windows (Ctrl-Z is the EOF character for those OSes)
  //         * Ctrl-D for all other operating systems (Ctrl-D is the EOF character for these OSes)
  //       Basically "std::cin >> i" when cast to a bool will only be false when the integer
  //       i is not read in. So typing an EOF character or letters will cause such to occur.

  // Type in some numbers that are not in sorted order and then hit the EOF character, e.g.,
  //   1 3 2
  //   <EOF character>
  //
  // Notice that v_copy1's output does not output all permutations since 
  // std::next_permutation() requires the range passed to it to be sorted to start.
  // The v_copy2 output does output all permutations since it starts sorted.

  // make a copy of v...
  auto v_copy1{ v };

  // output all permutations of v_copy1 as-is (i.e., without sorting first)...
  // iterate over all permutations...
  std::cout << "\n\nv_copy1 all permutations...\n";
  do
  {
    // output all integers in v_copy1...
    for (auto const& elem : v_copy1)
      std::cout << elem << ' ';
    std::cout << '\n';
  }
  while (next_permutation(v_copy1.begin(),v_copy1.end()));

  // make a copy of v...
  auto v_copy2{ v };

  // sort v_copy2...
  sort(v_copy2.begin(), v_copy2.end());

  // output all permutations of v_copy2...
  std::cout << "\n\nv_copy2 all permutations...\n";
  do
  {
    // output all integers in v_copy2...
    for (auto const& elem : v_copy2)
      std::cout << elem << ' ';
    std::cout << '\n';
  }
  while (next_permutation(v_copy2.begin(),v_copy2.end()));
}
