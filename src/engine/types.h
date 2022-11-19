//
// Created by kubaa on 19/11/2022.
//

#ifndef MARLIN_TYPES_H
#define MARLIN_TYPES_H

namespace engine {

    typedef unsigned long long ulong;

    enum GameState : int{
        YELLOW_WON = 0,
        RED_WON = 1,
        ON_GOING = 2
    };

    enum Player : int {
        YELLOW = 0,
        RED = 1
    };

} // engine

#endif //MARLIN_TYPES_H
