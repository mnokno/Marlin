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

    int Search::findBestMove(int depth) {
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;
        for (int& move : MoveGenerator::generateMoves(position)){
            position.makeMove(move);
            //int score = miniMax(depth - 1);
            int score = alphaBetaTT(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
            std::cout << "column: " + to_string(move % 7) << " score: " + to_string(score) << std::endl;
            if (score < minScore){
                minScore = score;
                betsMove = move;
            }
            position.unMakeMove();
        }
        std::cout << to_string(betsMove % 7) << ":"
                  << leafNodes << "l:"
                  << branchNodes << "b:"
                  << (leafNodes + branchNodes)  << "t:"
                  << TTHits << "tth:"
                  << to_string(minScore) << std::endl;
        return betsMove;
    }

    int Search::findBestMove(int depth, bool useTT) {
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;
        for (int& move : MoveGenerator::generateMoves(position)){
            position.makeMove(move);
            int score;
            //
            if (useTT){
                score = miniMaxTT(depth - 1);
            }
            else{
                score = miniMax(depth - 1);
                //score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
            }
            std::cout << "column: " + to_string(move % 7) << " score: " + to_string(score) << std::endl;
            if (score < minScore){
                minScore = score;
                betsMove = move;
            }
            position.unMakeMove();
        }
        std::cout << to_string(betsMove % 7) << ":"
        << leafNodes << "l:"
        << branchNodes << "b:"
        << (leafNodes + branchNodes)  << "t:"
        << TTHits << "tth:"
        << to_string(minScore) << std::endl;
        return betsMove;
    }

    int Search::miniMax(int depthLeft) {
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }
        this->branchNodes++;
        int max = -EVAL_INFINITY;
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            position.makeMove(move);
            int score = -miniMax( depthLeft - 1 );
            position.unMakeMove();
            if(score > max){
                max = score;
            }
        }
        return max;
    }

    int Search::miniMaxTT(int depthLeft) {
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        bool hit;
        TTEntry entry = this->transpositionTable.porbe(this->position.getHash(), hit);
        if (hit && entry.getDepth() == depthLeft){
            //if (entry.getDepth() != depthLeft){
            //    std::cout << "depth missmatch" << std::endl;
            //}
            if (entry.getHash() == this->position.getHash()){
                this->TTHits++;
                return entry.getEval();
            }
            else{
                std::cout << "hash collision" << std::endl;
            }
        }

        this->branchNodes++;
        int max = -EVAL_INFINITY;
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            position.makeMove(move);
            int score = -miniMaxTT( depthLeft - 1 );
            position.unMakeMove();
            if(score > max){
                max = score;
            }
        }

        this->transpositionTable.save(position.getHash(), depthLeft, max, position);

        return max;
    }

    int Search::alphaBeta(int alpha, int beta, int depthLeft) {
        if(depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }
        this->branchNodes++;
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
        return alpha;
    }

    int Search::alphaBetaTT(int alpha, int beta, int depthLeft) {
        if(depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        bool hit;
        TTEntry entry = this->transpositionTable.porbe(this->position.getHash(), hit);
        if (hit && entry.getDepth() == depthLeft){
            //if (entry.getDepth() != depthLeft){
            //    std::cout << "depth missmatch" << std::endl;
            //}
            if (entry.getHash() == this->position.getHash()){
                this->TTHits++;
                return entry.getEval();
            }
            else{
                std::cout << "hash collision" << std::endl;
            }
        }

        this->branchNodes++;
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

        this->transpositionTable.save(position.getHash(), depthLeft, alpha, position);

        return alpha;
    }
} // engine