# Advanced Apply
Using template metaprogramming to 'magically' and ergonomically destructure pairs / tuples at compile time without any performance penalty. The operations are usually [inlined](https://godbolt.org/z/46GYsaGz4) on the first optimization pass. The order of the lambda parameters doesn't matter, only in the case where you retrieve items of the same type. When retrieving items of the same type they will be retrieved in the order they are defined in the pair / tuple. 

This was an experiment to explore the type system of C++ and its capabilities.

``` c++
#include "adv_apply.hpp"

#include <iostream>
#include <string>

auto main() -> int {
    int const array[] = { 10, 20, 30 };
    auto x = std::make_tuple(0.5, array, std::make_pair(1, 1000000));
    for (auto i = 0; i < 10; i++) {
        auto y = adv::apply(x, [&](std::pair<int, int> & a, double b, int const p[]) {
            std::cout << "array: " << p[i % 3] << std::endl;
            a.second = (int)(a.second * b);
            a.first = a.first + i;

            return a;
        });

        std::cout << "pair second: " << y.second << std::endl;
        std::cout << "pair first: " << y.first << std::endl;
    }
}
```
**Note:** this utility could obliterate the compile time of your project, use with care.
