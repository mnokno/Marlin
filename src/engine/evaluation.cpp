//
// Created by kubaa on 21/11/2022.
//

#include "precalculated_data.h"
#include "evaluation.h"
#include "constants.h"
#include "types.h"
#include "bitops.h"

namespace engine {
    int Evaluation::eval(Position position) {
        switch (position.getGameState()) {
            case GameState::ON_GOING:
                return staticEval(position);
            case GameState::YELLOW_WON:
                return EVAL_INFINITY;
            case GameState::RED_WON:
                return -EVAL_INFINITY;
            case GameState::DRAW:
                return 0;
        }
        return 0;
    }

    int Evaluation::staticEval(Position position) {
        ulong yellow = 0;
        ulong red = 0;
        for (int i = 0; i < 7; i++){
            if (position.getStackHeight(i) < 6){
                int pos = position.convertFileToMove(i);
                yellow |= (PrecalculatedData::adjacencySquareMasks[pos] & position.getPosition(Player::YELLOW));
                red |= (PrecalculatedData::adjacencySquareMasks[pos] & position.getPosition(Player::RED));
            }
        }
        return BitOps::countBits(yellow) - BitOps::countBits(red);
    }
} // engine