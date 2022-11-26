//
// Created by kubaa on 19/11/2022.
//

#ifndef MARLIN_TYPES_H
#define MARLIN_TYPES_H

namespace engine {

    typedef unsigned long long ulong;
    typedef unsigned int uint;

    enum GameState : int{
        YELLOW_WON = 0,
        RED_WON = 1,
        ON_GOING = 2,
        DRAW = 3
    };

    enum Player : int {
        YELLOW = 0,
        RED = 1
    };

    enum NodeType : int {
        EXACT = 0,
        LOWER_BOUND = 1,
        UPPER_BOUND = 2
    };

    enum BaseLevel : int {
        MINI_MAX = 0,
        ALPHA_BETA = 1,
        MINI_MAX_TT = 2,
        ALPHA_BETA_TT = 3,
        ALPHA_BETA_TT_MO = 4
    };

} // engine

#endif //MARLIN_TYPES_H
