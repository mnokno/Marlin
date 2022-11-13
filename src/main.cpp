#include <iostream>
#include "position.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    engine::Position* position = new engine::Position();
    std::cout << position->test(2, 3) << std::endl;
    return 0;
}
