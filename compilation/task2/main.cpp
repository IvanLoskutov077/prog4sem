#include "time.hpp"

int main() {
    Time a {5, 9};
    Time b {10, 55};

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << (a + b) << std::endl;
    
    return 0;
}