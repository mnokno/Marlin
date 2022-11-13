//
// Created by kubaa on 13/11/2022.
//

#ifndef MARLIN_POSITION_H
#define MARLIN_POSITION_H

#include "types.h"

namespace engine {

    class Position {
    private:
        ulong postion;
        GameState gameState;
        Player playerToMove;
        int* stackHeights [7];
        ulong hash;

    public:
        Position();
        void makeMove(ulong move);
        void unMakeMove(ulong move);
        void makeMove(int move);
        void unMakeMove(int move);
        GameState getGameState();
    };

} // engine

#endif //MARLIN_POSITION_H
