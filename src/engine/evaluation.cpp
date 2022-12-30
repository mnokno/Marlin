//
// Created by kubaa on 21/11/2022.
//

#include "precalculated_data.h"
#include "evaluation.h"
#include "constants.h"
#include "types.h"
#include "bitops.h"

namespace engine {

    /**
     * Performs a static evaluation of the position.
     *
     * @param position position to evaluate
     * @return evaluation of the position
     */
    int Evaluation::eval(Position& position) {
        switch (position.getGameState()) {
            case GameState::ON_GOING:
                return staticEval(position);
            case GameState::YELLOW_WON:
                return EVAL_INFINITY - (position.getMoveCount() << 6);
            case GameState::RED_WON:
                return -EVAL_INFINITY + (position.getMoveCount() << 6);
            case GameState::DRAW:
                return 0;
        }
        return 0;
    }

    /**
     * Performs a static evaluation of the position.
     *
     * @param position position to evaluate
     * @return evaluation of the position
     */
    int Evaluation::staticEval(Position& position) {
        return winingPossibilityEval(position);
    }

    /**
     * Evaluation score based on the number of winning possibilities.
     *
     * @param position position to evaluate
     * @return evaluation score
     */
    int Evaluation::winingPossibilityEval(Position& position) {
        // allocates memory to score counters
        int score[2] = {0, 0};

        // finds edgesExtractionMask used to extract edges from the position
        ulong edgesExtractionMask = 0;
        for (int j = 0; j < 7; j++){
            edgesExtractionMask |= PrecalculatedData::pillarMasks[position.convertFileToMove(j)];
        }

        // finds score for yellow and red
        for (int i = 0; i < 2; i++){
            ulong exposedOnEdge = position.getPosition(i) & edgesExtractionMask;
            int opponent = i == 0 ? 1 : 0;

            while (!BitOps::isEmpty(exposedOnEdge)){
                int nextBitPos = BitOps::bitScanForward(exposedOnEdge);
                for (ulong winingLine : PrecalculatedData::winingLinesMasks[nextBitPos]){
                    if ((winingLine & ~position.getPosition(opponent)) == winingLine){
                        score[i] += 1;
                    }
                }
                exposedOnEdge ^= (ulong)1 << nextBitPos;
            }
        }

        // return the difference between yellows and reds score
        return score[0] - score[1];
    }

} // engine