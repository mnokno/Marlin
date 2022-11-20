//
// Created by kubaa on 20/11/2022.
//

#include "position.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

namespace engine{

    void Position::makeMove(int move) {
        // plays the move
        this->positions[playerToMove] ^= PrecalculatedData::moveMasks[move];
        // updates hash, using progressive hash updating
        this->hash = ZobristHashing::updateHash(this->hash, move, this->playerToMove);
        // updates game state if the played move ended the game
        this->gameState = gameStateAfterMove(move);
        // updates stack heights
        this->stackHeights[move % 7]++;
        // update game state
        this->playerToMove = playerToMove == Player::YELLOW ? Player::RED : Player::YELLOW;
        // pushes this move onto the history stack
        this->history.push(move);
    }

    void Position::unMakeMove(int move) {

    }

    GameState Position::gameStateAfterMove(int move) {
        // TODO
        return static_cast<GameState>(0);
    }

    ulong Position::getPosition(Player player) {
        return this->positions[player];
    }

    ulong Position::getPosition(int player) {
        return this->positions[player];
    }

    GameState Position::getGameState() {
        return this->gameState;
    }

    Player Position::getPlayerToMove() {
        return this->playerToMove;
    }

    ulong Position::getHash() {
        return this->hash;
    }

    short Position::getStackHeight(short stack) {
        return this->stackHeights[stack];
    }
}