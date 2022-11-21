//
// Created by kubaa on 21/11/2022.
//

#include "search.h"
#include "constants.h"
#include "evaluation.h"
#include "move_generator.h"

namespace engine {
    Search::Search(Position position) {
        this->position = position;
    }

    int Search::findBestMove(int depth) {
        //todo
        // currently only work for yellow player
        int maxScore = -EVAL_INFINITY;
        int betsMove = -1;
        for (int& move : MoveGenerator::generateMoves(position)){
            int score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
            if (score > maxScore){
                maxScore = score;
                betsMove = move;
            }
        }
        return betsMove;
    }

    int Search::alphaBeta(int alpha, int beta, int depthLeft) {
        int bestScore = -EVAL_INFINITY;
        if(depthLeft == 0) {
            return Evaluation::eval(this->position);
        }
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            position.makeMove(move);
            int score = -alphaBeta( -beta, -alpha, depthLeft - 1 );
            if( score >= beta )
                return score;  // fail-soft beta-cutoff
            if( score > bestScore ) {
                bestScore = score;
                if( score > alpha )
                    alpha = score;
            }
            position.unMakeMove();
        }
        return bestScore;
    }
} // engine