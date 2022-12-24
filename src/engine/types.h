//
// Created by kubaa on 19/11/2022.
//

#ifndef MARLIN_TYPES_H
#define MARLIN_TYPES_H

namespace engine {

    typedef unsigned long long int ulong;
    typedef unsigned int uint;
    typedef signed char byte;

    enum GameState : byte {
        YELLOW_WON = 0,
        RED_WON = 1,
        ON_GOING = 2,
        DRAW = 3
    };

    enum Player : byte {
        YELLOW = 0,
        RED = 1
    };

    enum NodeType : byte {
        EXACT = 0,
        LOWER_BOUND = 1,
        UPPER_BOUND = 2,
        END = 4,
        EMPTY = 5
    };

    enum BaseLevel : byte {
        MINI_MAX = 0,
        ALPHA_BETA_SIMPLE = 1,
        ALPHA_BETA = 2,
        ALPHA_BETA_MT = 3
    };

} // engine

#endif //MARLIN_TYPES_H
