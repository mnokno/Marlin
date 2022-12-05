//
// Created by kubaa on 21/11/2022.
//

#ifndef MARLIN_SEARCH_H
#define MARLIN_SEARCH_H

#include <map>
#include "position.h"
#include "transposition_table.h"
#include "types.h"

namespace engine {

    class Search {
    public:
        explicit Search(Position &position);
        int findBestMove(int depth);
        int findBestMoveBaseTest(int depth, BaseLevel baseLevel);
        int findBestMoveProgressiveTest(int depth, BaseLevel baseLevel);
        int findBestMoveMT(int depth);

        [[nodiscard]] int getLeafNodes() const;
        [[nodiscard]] int getBranchNodes() const;
        [[nodiscard]] int getTTHits() const;
    private:
        int alphaBeta(int alpha, int beta, int depthLeft);
        int miniMax(int depthLeft);
        static int alphaBetaStatic(int alpha, int beta, Position& position, TranspositionTable& tt, int depthLeft, Search& search);
        static void searchTask(Search *search, Position lPosition, int move, int depth);

        Position& position;
        TranspositionTable transpositionTable = TranspositionTable(0);
        int leafNodes;
        int branchNodes;
        int TTHits;

        map<int, int> results;
    };

} // engine

#endif //MARLIN_SEARCH_H
