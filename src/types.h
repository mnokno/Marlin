//
// Created by kubaa on 13/11/2022.
//

#ifndef MARLIN_TYPES_H
#define MARLIN_TYPES_H

namespace engine{

    typedef unsigned long long int ulong;

    enum GameState : int{
        YELLOW_WON = 0,
        RED_WON = 1,
        ON_GOING = 2
    };

    enum Player : int {
        YELLOW = 0,
        RED = 1
    };
}

#endif //MARLIN_TYPES_H
