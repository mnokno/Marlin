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

} // engine

#endif //MARLIN_TYPES_H
