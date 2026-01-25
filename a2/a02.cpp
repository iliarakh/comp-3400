#include <compare>
#include <format>
#include <iostream>
#include <memory>
#include <print>
#include <vector>

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
  auto parse(std::format_parse_context& ctx)
  { 
    // If one only wants to support the empty format specifier {} 
    // this function could be written as return ctx.begin(); 
    // If one needs to parse what is inside {} then more code is needed. 
    // This code returns the last character parsed (and only supports {}).
    if (ctx.begin() == ctx.end())
      return ctx.begin();
    
    auto pos{ ctx.begin() };
    if (*pos == '{')
      ++pos; 
    if (*pos != '}')
      throw std::format_error("invalid format specification");
    return pos;
  }
  
  auto format(point const& p, std::format_context& ctx) const
  { 
    return std::format_to(ctx.out(), "({},{})", p.x, p.y);
  }
};