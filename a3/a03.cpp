#include <algorithm>
#include <cstddef>
#include <generator>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <vector>

template <typename R>
	requires std::ranges::forward_range<R> &&
			 std::ranges::sized_range<R>
std::generator<std::vector<std::ranges::iterator_t<R>>> all_subsets(R &&r)
{
	using std::sort;
	using std::vector;

	std::size_t const n{r.size()};
	if (n >= std::numeric_limits<std::size_t>::digits)
		throw std::length_error("range too large");
	std::size_t const num_subsets{std::size_t(1) << n};

	// populate all with iterators to elements of r...
	vector<std::ranges::iterator_t<R>> all;
	all.reserve(r.size());
	auto in{r.begin()};
	auto in_end{r.end()};
	auto out{std::back_inserter(all)};
	for (; in != in_end; ++in, ++out)
		*out = in;

	// populate v suitably to hold subsets of r...
	vector<std::ranges::iterator_t<R>> v;
	v.reserve(r.size());

	// iterate through all subsets...
	for (std::size_t i = 0; i != num_subsets; ++i)
	{
		v.clear();
		for (std::size_t j = 0; j != n; ++j)
		{
			// if the jth bit if i is set, then add to v...
			if ((i >> j) & 1)
				v.push_back(all[j]);
		}
		co_yield v;
	}
}

int main()
{
	// read in all int values from stdin into a vector...
	std::vector<int> v;
	for (int i; std::cin >> i;)
		v.push_back(i);

	// output all permutations of that vector...
	auto gr{all_subsets(v)}; // returns std::generator< /* vector of iterators */ >
	auto f{gr.begin()};		 // returns iterator to std::vector< /* iterator */ >
	auto l{gr.end()};		 // returns "end" iterator / sentinel relative to f
	for (; f != l; ++f)
	{
		// f is an iterator to std::vector of iterators
		// So *f is a std::vector of iterators.
		// So the elements of *f are all iterators.
		// So *element is needed to access the element.
		std::cout << "SUBSET: ";
		for (auto const &elem : *f)
			std::cout << *elem << ' ';
		std::cout << '\n';
	}
}
