//
// Created by kubaa on 21/11/2022.
//

#ifndef MARLIN_SEARCH_H
#define MARLIN_SEARCH_H

#include "position.h"

namespace engine {

    class Search {
    public:
        Search(Position &position);
        int findBestMove(int depth);
    private:
        int alphaBeta(int alpha, int beta, int depthLeft);
        int miniMax(int depthLeft);

        Position& position;
        int leafNodes;
        int branchNodes;
    };

} // engine

#endif //MARLIN_SEARCH_H
