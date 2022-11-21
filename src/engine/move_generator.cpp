//
// Created by kubaa on 21/11/2022.
//

#include "move_generator.h"
#include "position.h"

namespace engine {
    list<int> MoveGenerator::generateMoves(Position position) {
        list<int> moves;
        for (short i = 0; i < 7; i++){
            if (position.getStackHeight(i) < 6){
                moves.push_back(position.getStackHeight(i) * 7 + i);
            }
        }
        return moves;
    }
} // engine