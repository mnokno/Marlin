//
// Created by kubaa on 20/11/2022.
//

#include "position.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

namespace engine{

    /**
     * Creates a new position with all pointers set to being of a new game.
     */
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
        this->moveCount = 0;
    }

    /**
     * Plays a move on the position and updates all trackers.
     *
     * @param move Move to be played.
     */
    void Position::makeMove(int move) {
        // update move count
        this->moveCount++;
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

    /**
     * Undoes the last move played on the position and updates all trackers.
     */
    void Position::unMakeMove() {
        // updates move count, -- since we went back in time
        this-moveCount--;
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

    /**
     * Calculates game state checking for wins, loses and draws after specific move was
     * made for efficiency.
     *
     * @param move Move that was made
     * @param playerWhoMoved Player who made the move
     * @return GameState after the move was made
     */
    GameState Position::gameStateAfterMove(int move, Player playerWhoMoved) {
        for (ulong& winingLine : PrecalculatedData::winingLinesMasks[move]){
            if ((winingLine & this->positions[playerWhoMoved]) == winingLine){
                return playerWhoMoved == Player::YELLOW ? GameState::YELLOW_WON : GameState::RED_WON;
            }
        }
        // there board is fully filled, there is no winner, so it's a draw
        if (moveCount == 42){
            return GameState::DRAW;
        }
        return GameState::ON_GOING;
    }

    /**
     * Getter for a mask of a player's position.
     *
     * @param player Player whose position is to be returned
     * @return Mask of a player's position
     */
    ulong Position::getPosition(Player player) {
        return this->positions[player];
    }

    /**
     * Getter for a mask of a player's position.
     *
     * @param player Player whose position is to be returned
     * @return Mask of a player's position
     */
    ulong Position::getPosition(int player) {
        return this->positions[player];
    }

    /**
     * Getter for a game state.
     *
     * @return Game state
     */
    GameState Position::getGameState() {
        return this->gameState;
    }

    /**
     * Getter for a player to move.
     *
     * @return Player to move
     */
    Player Position::getPlayerToMove() {
        return this->playerToMove;
    }

    /**
     * Getter for a hash of a position.
     *
     * @return Hash of a position
     */
    ulong Position::getHash() {
        return this->hash;
    }

    /**
     * Getter for a stack height.
     *
     * @param stack Stack whose height is to be returned
     * @return Stack height
     */
    short Position::getStackHeight(short stack) {
        return this->stackHeights[stack];
    }

    /**
     * Getter for a move count.
     *
     * @return Move count
     */
    short Position::getMoveCount() {
        return moveCount;
    }

    /**
     * Convert given column to a move.
     *
     * @param column Column to be converted
     * @return Move
     */
    int Position::convertFileToMove(int column) {
        return this->stackHeights[column] * 7 + column;
    }
}