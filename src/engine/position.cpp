//
// Created by kubaa on 20/11/2022.
//

#include "position.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

namespace engine{

    Position::Position() {
        this->positions[0] = (ulong)0;
        this->positions[1] = (ulong)0;
        this->gameState = GameState::ON_GOING;
        this->playerToMove = Player::YELLOW;
        this->hash = ZobristHashing::generateHash(this);
        for (int i = 0; i < 7; i++){
            this->stackHeights[i] = 0;
        }
        // stack should be empty at the beginning
        //this->history;
    }

    void Position::makeMove(int move) {
        // plays the move
        this->positions[playerToMove] ^= PrecalculatedData::moveMasks[move];
        // updates hash, using progressive hash updating
        this->hash = ZobristHashing::updateHash(this->hash, move, this->playerToMove);
        // updates game state only looks at possible changes made by the played move;
        this->gameState = gameStateAfterMove(move, this->playerToMove );
        // updates stack heights
        this->stackHeights[move % 7]++;
        // pushes this move onto the history stack
        this->history.push(move);
        // update player to move
        this->playerToMove = this->playerToMove == Player::YELLOW ? Player::RED : Player::YELLOW;
    }

    void Position::unMakeMove() {
        // update game state
        this->playerToMove = playerToMove == Player::YELLOW ? Player::RED : Player::YELLOW;
        // recovers the played move
        int move = this->history.top();
        this->history.pop();
        // updates stack weights
        this->stackHeights[move % 7]--;
        // updates hashing
        this->hash = ZobristHashing::updateHash(this->hash, move, this->playerToMove);
        // un plays the move
        this->positions[playerToMove] ^= PrecalculatedData::moveMasks[move];
    }

    GameState Position::gameStateAfterMove(int move, Player playerWhoMoved) {
        for (ulong& winingLine : PrecalculatedData::winingLinesMasks[move]){
            if ((winingLine & this->positions[playerWhoMoved]) == winingLine){
                return playerWhoMoved == Player::YELLOW ? GameState::YELLOW_WON : GameState::RED_WON;
            }
        }
        return GameState::ON_GOING;
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