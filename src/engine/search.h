//
// Created by kubaa on 21/11/2022.
//

#ifndef MARLIN_SEARCH_H
#define MARLIN_SEARCH_H

#include "position.h"
#include "transposition_table.h"

namespace engine {

    class Search {
    public:
        explicit Search(Position &position);
        int findBestMove(int depth);
        int findBestMove(int depth, bool useTT);
    private:
        int alphaBeta(int alpha, int beta, int depthLeft);
        int miniMax(int depthLeft);
        int miniMaxTT(int depthLeft);

        Position& position;
        TranspositionTable transpositionTable = TranspositionTable(0);
        int leafNodes;
        int branchNodes;
        int TTHits;
    };

} // engine

#endif //MARLIN_SEARCH_H
