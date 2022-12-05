#include <iostream>
#include "search.h"
#include "position.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    engine::Search search = *new engine::Search(*new engine::Position());
    search.findBestMoveMT(9);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
