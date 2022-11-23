//
// Created by kubaa on 21/11/2022.
//

#include <iostream>
#include "search.h"
#include "constants.h"
#include "evaluation.h"
#include "move_generator.h"
#include "precalculated_data.h"
#include "transposition_table.h"

namespace engine {

    Search::Search(Position& position) : position(position) {
        this->transpositionTable = *new TranspositionTable(999983);
        this->position = position;
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;
    }

    /**
     * Current best implementation fo the search algorithm.
     */
    int Search::findBestMove(int depth) {
        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        // at the binning there is no best move, hence score is greater than wining score
        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;

        // finds the best move from the generated moves
        for (int& move : MoveGenerator::generateMoves(position)){
            // evaluates this move
            position.makeMove(move);
            //int score = alphaBetaTT(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
            int score = miniMaxTT(depth - 1);
            position.unMakeMove();
            // logs branch evaluation data
            std::cout << "column: " + to_string(move % 7) << " score: " + to_string(score) << std::endl;
            // if this move is better than previous best move, it becomes the new best move
            if (score < minScore){
                minScore = score;
                betsMove = move;
            }
        }

        // logs data about this search
        std::cout << to_string(betsMove % 7) << ":"
                  << leafNodes << "l:"
                  << branchNodes << "b:"
                  << (leafNodes + branchNodes)  << "t:"
                  << TTHits << "tth:"
                  << to_string(minScore) << std::endl;

        // returns best move
        return betsMove;
    }

    /**
     * This method implements the search using algorithm at baseLevel.
     * Base 0, was fully tested manually can be used for automatic testing of enhancements.
     * Other bases can be also used for testing with greater depth and speed, but they need to be validated
     * using base level 0 first.
     */
    int Search::findBestMoveBaseTest(int depth, int baseLevel) {
        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        // at the binning there is no best move, hence score is greater than wining score
        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;

        for (int& move : MoveGenerator::generateMoves(position)){
            int score;
            // evaluates this move using the specified base level algorithm
            position.makeMove(move);
            switch (baseLevel) {
                case 0:
                    score = miniMax(depth - 1);
                    break;
                case 1:
                    score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
                    break;
                case 2:
                    score = miniMaxTT(depth - 1);
                    break;
                case 3:
                    score = alphaBetaTT(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
                    break;
                default:
                    throw std::invalid_argument("Base level not supported!");
            }
            position.unMakeMove();

            // if this move is better than previous best move, it becomes the new best move
            if (score < minScore){
                minScore = score;
                betsMove = move;
            }
        }

        // returns best move
        return betsMove;
    }

    /**
     * Basic implementation of the minimax algorithm without any enchantments.
     *
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::miniMax(int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // updates counters
        this->branchNodes++;

        // finds max value of this position
        int max = -EVAL_INFINITY;
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            position.makeMove(move);
            int score = -miniMax( depthLeft - 1 );
            position.unMakeMove();
            if(score > max){
                max = score;
            }
        }

        // returns the max value
        return max;
    }

    /**
     * Minimax algorithm with transposition table.
     *
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::miniMaxTT(int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // checks if we have a transpositionTable match
        bool hit;
        TTEntry entry = this->transpositionTable.porbe(this->position.getHash(), hit);
        // entry.getDepth() == depthLeft, has a high chance to catch a key collision
        if (hit && entry.getDepth() == depthLeft){
                this->TTHits++;
                return entry.getEval();
        }

        // updates counters
        this->branchNodes++;

        // finds max value of this position
        int max = -EVAL_INFINITY;
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            position.makeMove(move);
            int score = -miniMaxTT( depthLeft - 1 );
            position.unMakeMove();
            if(score > max){
                max = score;
            }
        }

        // adds the result to transposition table
        this->transpositionTable.save(position.getHash(), depthLeft, max);

        // returns the max value
        return max;
    }

    /**
     * Minimax algorithm with alpha beta pruning.
     *
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::alphaBeta(int alpha, int beta, int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if(depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // updates counters
        this->branchNodes++;

        // finds max value of this position
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            this->position.makeMove(move);
            int score = -alphaBeta( -beta, -alpha, depthLeft - 1 );
            this->position.unMakeMove();
            if(score >= beta){
                return beta;   //  fail hard beta-cutoff
            }
            if(score > alpha){
                alpha = score; // alpha acts like max in MiniMax
            }
        }

        // returns the max (alpha) value
        return alpha;
    }

    /**
     * Minimax algorithm with alpha beta pruning and transposition table.
     *
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::alphaBetaTT(int alpha, int beta, int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if(depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // checks if we have a transpositionTable match
        bool hit;
        TTEntry entry = this->transpositionTable.porbe(this->position.getHash(), hit);
        // entry.getDepth() == depthLeft, has a high chance to catch a key collision
        if (hit && entry.getDepth() == depthLeft){
            this->TTHits++;
            return entry.getEval();
        }

        // updates counters
        this->branchNodes++;

        // finds max value of this position
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            this->position.makeMove(move);
            int score = -alphaBetaTT( -beta, -alpha, depthLeft - 1 );
            this->position.unMakeMove();
            if(score >= beta){
                return beta;   //  fail hard beta-cutoff
            }
            if(score > alpha){
                alpha = score; // alpha acts like max in MiniMax
            }
        }

        // adds the result to transposition table
        this->transpositionTable.save(position.getHash(), depthLeft, alpha);

        // returns the max (alpha) value
        return alpha;
    }

    int Search::getLeafNodes() const {
        return this->leafNodes;
    }

    int Search::getBranchNodes() const {
        return this->branchNodes;
    }

    int Search::getTTHits() const {
        return this->TTHits;
    }

} // engine