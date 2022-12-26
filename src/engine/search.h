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
        explicit Search(Position &position, TranspositionTable &transpositionTable);
        int findBestMove(int depth);
        int findBestMoveIn(int milliseconds);
        int findBestMoveBaseTest(int depth, BaseLevel baseLevel);
        int findBestMoveABMT(int depth, int threads);
        int findBestMoveMMMT(int depth);

        [[nodiscard]] int getLeafNodes() const;
        [[nodiscard]] int getBranchNodes() const;
        [[nodiscard]] int getTTHits() const;
    private:
        int alphaBeta(int alpha, int beta, int depthLeft);
        int alphaBetaSimple(int alpha, int beta, int depthLeft);

        static int alphaBetaStatic(int alpha, int beta, Position& position, TranspositionTable& tt, int depthLeft, bool& abort, int id);

        static void searchAlphaBetaTask(Position lPosition, int depth, int& result, bool& abort, TranspositionTable& tt, int id);

        int miniMax(int depthLeft);
        static int miniMaxStatic(int depthLeft, Search& search, Position& lPosition, int id, bool& abort);
        static void searchMiniMaxTask(Search& search, Position lPosition, int id, int depth, bool& abort);

        static void timedSearchTask(Search& search);

        static void abortAfter(bool& abortFlag, int milliseconds);

        Position& position;
        TranspositionTable& transpositionTable;
        int leafNodes;
        int branchNodes;
        int TTHits;

        map<int, int> results;
        map<int, int> leafCounts;
        map<int, int> branchCounts;
        map<int, int> TTCounts;
        int currentBestMove;
    };

} // engine

#endif //MARLIN_SEARCH_H
