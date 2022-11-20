//
// Created by kubaa on 20/11/2022.
//

#include "position.h"

namespace engine{

    ulong Position::getPosition(Player player) {
        return this->positions[player];
    }

    GameState Position::getGameState() {
        return this->gameState;
    }

    Player Position::getPlayerToMove() {
        return this->playerToMove;
    }

    short Position::getStackHeight(short stack) {
        return this->stackHeights[stack];
    }
}