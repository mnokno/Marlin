//
// Created by kubaa on 21/11/2022.
//

#ifndef MARLIN_SEARCH_H
#define MARLIN_SEARCH_H

#include "position.h"
#include "transposition_table.h"
#include "types.h"

namespace engine {

    class Search {
    public:
        explicit Search(Position &position);
        int findBestMove(int depth);
        int findBestMoveBaseTest(int depth, BaseLevel baseLevel);

        [[nodiscard]] int getLeafNodes() const;
        [[nodiscard]] int getBranchNodes() const;
        [[nodiscard]] int getTTHits() const;
    private:
        int alphaBeta(int alpha, int beta, int depthLeft);
        int alphaBetaTT(int alpha, int beta, int depthLeft);
        int alphaBetaTTMO(int alpha, int beta, int depthLeft);
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
