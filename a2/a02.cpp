
#include <compare>
#include <format>
#include <iostream>
#include <memory>
#include <print>
#include <vector>

class shape; // forward declaration
using oo_shape_type = std::shared_ptr<shape>;

struct point
{
	int x;
	int y;
};

template <>
struct std::formatter<point>
{
// After C++20 was standardized std::formatter::parse() was defined to permit
// constexpr parsers. If such, however, is not supported by the compiler
// this code does not define parse() to be constexpr...
//   * ASIDE: GCC version 15 does not allow one to throw from a constexpr function
//            which will cause a compiler error if one throws. (GCC cannot
//            fully implement this feature until such is supported.)
//  * The __cpp_lib_format macro is part of the C++ Feature Testing Macros defined
//    by the C++ standard. The value of this macro corresponds to the __cplusplus
//    macro value (which is the integer date YYYYMMDD of the C++ standard being compiled).
#if __cpp_lib_format >= 202106
	constexpr
#endif
		auto parse(std::format_parse_context &ctx)
	{
		// If one only wants to support the empty format specifier {}
		// this function could be written as return ctx.begin();
		// If one needs to parse what is inside {} then more code is needed.
		// This code returns the last character parsed (and only supports {}).
		if (ctx.begin() == ctx.end())
			return ctx.begin();

		auto pos{ctx.begin()};
		if (*pos == '{')
			++pos;
		if (*pos != '}')
			throw std::format_error("invalid format specification");
		return pos;
	}

	auto format(point const &p, std::format_context &ctx) const
	{
		return std::format_to(ctx.out(), "({},{})", p.x, p.y);
	}
};

class shape
{
private:
	// private member variable of type point.
	point p_;

	// Note for myself:
	// constructor-name ( parameter-list ) : member-initializer-list { body }
	// colon means “Before the constructor body runs, construct these members like this.”

public:
	// public default constructor
	shape() : p_() {}

	// public constructor that accepts a single argument of type point
	shape(point p) : p_(p) {}

	// public defaulted copy constructor.
	shape(shape const &) = default;

	// public defaulted copy assignment operator.
	shape &operator=(shape const &) = default; // shape& returns *this(as a reference), operator= is assignment, shape const& is source object
	/*   --Conceptually--
		  shape& shape::operator=(shape const& other) {
				p_ = other.p_;
				return *this;
		  }                                                      */
	// public defaulted move constructor.
	shape(shape &&) = default;

	// public defaulted move assignment operator.
	shape &operator=(shape &&) = default;

	// public virtual destructor
	virtual ~shape() = default; // virtual ~shape() {} is equivalent

	virtual oo_shape_type clone() const = 0;
	/*  “Every derived shape must know how to make a deep copy of itself.”
		virtual → runtime polymorphism
		clone() → a function named clone
		const → does not modify *this
		= 0 → no implementation here
		return type (oo_shape_type) → usually a pointer or smart pointer to base
	*/
	void const *void_address() const
	{
		return static_cast<void const *>(this);
	}

	virtual bool operator==(shape const &other) const
	{
		return void_address() == other.void_address();
	}

	virtual std::partial_ordering operator<=>(shape const &other) const
	{
		if (void_address() == other.void_address())
			return std::partial_ordering::equivalent;
		else
			return std::partial_ordering::unordered;
	}

	virtual void draw() const
	{
		using std::println;
		println("this: {}, shape::draw(): anchor_point: {}",
				void_address(), anchor_point());
	}

	point const &anchor_point() const
	{
		return p_;
	}
};
class circle : public virtual shape
{
private:
	int r_; // radius

public:
	circle(point p, int r) : shape(p), r_(r) {} // using shape's constructor accepting point; r == radius

	oo_shape_type clone() const override
	{
		return std::make_shared<circle>(*this);
	}
	void draw() const override
	{
		std::println("this: {}, circle::draw(): circle({},{})",
					 void_address(), anchor_point(), radius());
	}

	decltype(auto) centre() const
	{
		return this->anchor_point();
	}
	int radius() const
	{
		return r_;
	}
};

class line : public virtual shape
{
private:
	int slope_num_; // numerator of slope of line
	int slope_den_; // denominator of slope of line

public:
	line(int delta_y, int delta_x, int y_intercept_value) : shape(point{0, y_intercept_value}),
															slope_num_(delta_y),
															slope_den_(delta_x)
	{
	}

	oo_shape_type clone() const override
	{
		return std::make_shared<line>(*this);
	}

	void draw() const override
	{
		std::println("this: {}, line::draw(): line({},{},{})",
					 void_address(), slope_num_, slope_den_, anchor_point().y);
	}
};

class line_segment : public virtual shape
{
private:
	point stop_point;

public:
	line_segment(point start_point, point end_point) : shape(start_point), stop_point(end_point) {}

	oo_shape_type clone() const override
	{
		return std::make_shared<line_segment>(*this);
	}

	void draw() const override
	{
		shape::draw(); // this calls the parent (shape's) draw() function

		std::println("this: {}, line_segment::draw(): line_segment({},{})",
					 void_address(), anchor_point(), stop());
	}

	point const &stop() const
	{
		return stop_point;
	}

	decltype(auto) start() const
	{
		return this->anchor_point();
	}
};

int main()
{
	using namespace std;

	// Declare a std::vector that holds oo_shape_type instance:
	vector<oo_shape_type> v;

	// Appending a dynamically allocated circle object that is at point (3,5) with a radius of 45:
	v.push_back(make_shared<circle>(point{3, 5}, 45));

	// Appending a dynamically allocated line_segment object constructed with the start point (1,3) and stop point (7,9) to the vector v:
	v.push_back(make_shared<line_segment>(point{1, 3}, point{7, 9}));

	// Appending to the vector v a dynamically allocated line object constructed with 3, 5, and 10 passed to it (slope 3/5 and y-intercept is 10).
	v.push_back(make_shared<line>(3, 5, 10));

	// Write a for loop (range-based --not traditional) that iterates over v. Inside the loop body should be this code:
	for (auto elem : v)
	{
		elem->clone()->draw(); // yes, this is inefficient
	}
}