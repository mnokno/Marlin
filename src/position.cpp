//
// Created by kubaa on 13/11/2022.
//

#include "position.h"
#include "types.h"

namespace engine {

    Position::Position() {
        this->postion = (ulong)0;
        this->gameState = GameState::ON_GOING;
        this->playerToMove = Player::YELLOW;
        for (int i = 0; i < sizeof(this->stackHeights); i++){
            this->stackHeights[i] = 0;
        }
        //TODO
        this->hash = -1;
    }

    void Position::makeMove(ulong move) {
        this->postion ^= move;
        this->playerToMove = playerToMove == Player::YELLOW ? Player::RED : Player::YELLOW;
        //TODO
        // update game state
        // update stack height
        // update hash
    }

    void Position::unMakeMove(ulong move) {
        postion ^= move;
        this->playerToMove = playerToMove == Player::YELLOW ? Player::RED : Player::YELLOW;
        //TODO
        // update game state
        // update stack height
        // update hash
    }

    void Position::makeMove(int move) {
        //TODO
    }

    void Position::unMakeMove(int move) {
        //TODO
    }

    GameState Position::getGameState() {
        return this->gameState;
    }
} // engine