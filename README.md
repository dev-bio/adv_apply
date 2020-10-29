# Advanced Apply
Using template metaprogramming to 'magically' destructure pairs and tuples at compile time. Not advicable to use anywhere, unless you want to obliterate the compile time of your project. This was an experiment to explore the type system of C++ and its capabilities, this also allowed me to create some neat interfaces for a side project.

``` c++
#include "adv_apply.hpp"

#include <iostream>
#include <string>

auto main() -> int {
	// Expected output: 15.0, 8.0, 1, "amazing!", 99, 66, 2
	auto x = std::make_tuple(1, 2, 3, 4, 5, std::make_pair(99, 66), 7, 8.0, 9, 10, std::string { "amazing!" }, 33, 13, 14, 15.f, 16, 17, 18, 2);
	std::cout << adv::apply(x, [](float & a, double b, int c, int d, std::string & s, std::pair<int, int> & p) {
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << c << std::endl;
		std::cout << s << std::endl;
		std::cout << p.first << std::endl;
		std::cout << p.second << std::endl;

		return d;
	}) << std::endl;

	// Expected output: 15.0, 1
	auto y = std::make_pair(1, 15.f);
	std::cout << adv::apply(x, [](float & a, int b) {
		std::cout << a << std::endl;

		return b;
	}) << std::endl;
}
