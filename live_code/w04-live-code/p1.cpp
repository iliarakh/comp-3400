#include <algorithm>
#include <generator>
#include <iostream>
#include <iterator>
#include <vector>

//
// Since std::next_permutation() requires the range passed to it needs to be
// sorted to output all permutations, e.g., see p0.cpp, and not all types
// are permitted to be copied and/or moved, it is perhaps best to create
// an algorithm, e.g., called all_permutations() below, that is capable
// of computing all permutations of a range EVEN IF the type that the range
// stores in it cannot be copied and/or moved.
//
// The all_permutations algortihms below does the following:
//   1) Given a sized, forward range, r (whose type is R below):
//   2) Create a vector of iterators to elements in r that has the same
//      size as r. Note:
//        * std::ranges::iterator_t<R> is the iterator type of r
//        * so std::vector< std::ranges::iterator_t<R> > is a vector
//          of iterators to elements in r
//   3) Populate the vector in step (2) with iterators that refer to
//      every position in r.
//        * Since r is a forward iterator, do this in a forward direction
//          using ++. Remember * is used to access what the iterator points to.
//        * std::vector's iterators are contiguous iterators which are also
//          output iterator capable so *out = some_iterator_value; will write
//          some_iterator_value to where *out refers.
//   4) Since std::next_permutation() needs to start out sorted, call
//      std::sort() with the range for the vector (from step (2)) AND tell
//      sort() how to compare elements. The latter needs to be done since
//      one needs to use *iterator_variable to access an element so
//      in order to determine a < b one needs to write *a < *b. (The
//      comparison function passed to sort() is less than.)
//   5) After sorting loop over all permutations calling std::next_permutation().
//

template <typename R>
requires
  std::ranges::forward_range<R> &&
  std::ranges::sized_range<R>
std::generator<std::vector<std::ranges::iterator_t<R>>> all_permutations(R&& r)
{
  using std::vector;
  using std::sort;
  using std::next_permutation;

  // create v with the same reserved size as r's size...
  vector<std::ranges::iterator_t<R>> v;
  v.reserve(r.size());

    // populate v with iterators to elements of r...
  auto in{ r.begin() };
  auto in_end{ r.end() };
  auto out{ std::back_inserter(v) };
  for (; in != in_end; ++in, ++out)
    *out = in;

  // sort v based on what its iterators refer to...
  sort(v.begin(), v.end(),
    [](auto a, auto b) { return *a < *b; });

  // iterate over all permutations...
  do
  {
    co_yield v;
  }
  while (
    next_permutation(
      v.begin(),v.end(),
      [](auto a, auto b) { return *a < *b; }
    )
  );
}

int main()
{
  // read in all int values from stdin into a vector...
  std::vector<int> v;
  for (int i; std::cin >> i; )
    v.push_back(i);

  // output all permutations of that vector...
  auto gr{ all_permutations(v) };
  auto f{ gr.begin() };
  auto l{ gr.end() };
  for (; f != l; ++f)
  {
    for (auto const& elem : *f)
      std::cout << *elem << ' ';
    std::cout << '\n';
  }
}


