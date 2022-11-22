//
// Created by kubaa on 21/11/2022.
//

#include <iostream>
#include "search.h"
#include "constants.h"
#include "evaluation.h"
#include "move_generator.h"
#include "precalculated_data.h"

namespace engine {
    Search::Search(Position& position) : position(position) {
        this->position = position;
        this->leafNodes = 0;
        this->branchNodes = 0;
    }

    int Search::findBestMove(int depth) {
        this->leafNodes = 0;
        this->branchNodes = 0;

        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;
        for (int& move : MoveGenerator::generateMoves(position)){
            position.makeMove(move);
            int score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
            //int score = negaMax(depth - 1);
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
} // engine