//
// Created by kubaa on 25/11/2022.
//

#ifndef MARLIN_MOVE_ORDERING_H
#define MARLIN_MOVE_ORDERING_H

#include <list>
#include "position.h"

namespace engine {

    class MoveOrdering {
    public:
        static void orderMove(int*& moves, int size, Position& position);
    private:
        static void bobbleSort(int*& moves, int*& scores, int size);
    };

} // engine

#endif //MARLIN_MOVE_ORDERING_H
