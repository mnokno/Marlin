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
        return 0;
    }

    int Search::alphaBeta(int alpha, int beta, int depthLeft) {
        int bestScore = -EVAL_INFINITY;
        if(depthLeft == 0) {
            return Evaluation::eval(this->position);
        }
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            int score = -alphaBeta( -beta, -alpha, depthLeft - 1 );
            if( score >= beta )
                return score;  // fail-soft beta-cutoff
            if( score > bestScore ) {
                bestScore = score;
                if( score > alpha )
                    alpha = score;
            }
        }
        return bestScore;
    }
} // engine