# Advanced Apply
Using template metaprogramming to 'magically' destructure pairs and tuples at compile time.

``` c++
auto main() -> int {
	auto x = std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8.0, 9, 10, 11, 33, 13, 14, 15.f, 16, 17, 18, 2);
	std::cout << adv::apply(x, [](float & a, double b, int c, int d) {
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << c << std::endl;

		return d;
	}); // Expected output: 15.0, 8.0, 1, 2

	auto y = std::make_pair(1, 15.f);
	std::cout << adv::apply(x, [](float & a, int b) {
		std::cout << a << std::endl;
		std::cout << b << std::endl;

		return b;
	}); // Expected output: 15.0, 1
}
