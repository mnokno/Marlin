//
// Created by kubaa on 20/11/2022.
//

#ifndef MARLIN_POSITION_H
#define MARLIN_POSITION_H

#include "types.h"

namespace engine{
    class Position {
    public:
        ulong getPosition(Player player);
        GameState getGameState();
        Player getPlayerToMove();
        short getStackHeight(short stack);
    private:
        ulong positions[2];
        GameState gameState;
        Player playerToMove;
        ulong hash;
        short stackHeights[7];
    };
}



#endif //MARLIN_POSITION_H
