//
// Created by kubaa on 21/11/2022.
//

#ifndef MARLIN_MOVE_GENERATOR_H
#define MARLIN_MOVE_GENERATOR_H

#include <list>
#include "position.h"

namespace engine {

    class MoveGenerator {
    public:
        static list<int> generateMoves(Position position);
    };

} // engine

#endif //MARLIN_MOVE_GENERATOR_H
