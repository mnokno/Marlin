#include <iostream>
#include "search.h"
#include "position.h"

using namespace engine;

int main() {
    std::cout << "Hello, World!" << std::endl;
    //engine::Search search = *new engine::Search(*new engine::Position());
    //search.findBestMoveMT(9);
    //std::cout << "Hello, World!" << std::endl;
    Position p1 = *new Position();
    p1.makeMove(2);

    Position p2 = p1;
    p1.makeMove(1);
    std::cout << to_string(p1.getMoveCount()) << std::endl;
    std::cout << to_string(p2.getMoveCount()) << std::endl;
    return 0;
}
