//
// Created by Jakub Nowacki on 21/11/2022.
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
        int findBestMoveMMMT(int depth, int threads);

        [[nodiscard]] int getLeafNodes() const;
        [[nodiscard]] int getBranchNodes() const;
        [[nodiscard]] int getTTHits() const;
    private:
        int alphaBeta(int alpha, int beta, int depthLeft);
        int alphaBetaSimple(int alpha, int beta, int depthLeft);
        static int alphaBetaStatic(int alpha, int beta, Position& position, TranspositionTable& tt, int depthLeft, bool& abort, int id);
        static void searchAlphaBetaTask(Position lPosition, int depth, int& result, bool& abort, TranspositionTable& tt, int id);

        int miniMax(int depthLeft);
        static int miniMaxStatic(Position& position, int depthLeft, bool& abort, int id);
        static void searchMiniMaxTask(Position lPosition, int depth, int& result, bool& abort, int id);
        static void timedSearchTask(int& result, Position position, TranspositionTable& tt, int milliseconds);
        static void abortAfter(bool& abortFlag, int& currentDepth, int& currentScore, int rootPositionDepth, int milliseconds);
        static void waitForAbortAfter(bool& abortFlag);

        Position& position;
        TranspositionTable& transpositionTable;
        int leafNodes;
        int branchNodes;
        int TTHits;

        map<int, int> results;
        map<int, int> leafCounts;
        map<int, int> branchCounts;
        map<int, int> TTCounts;
    };

} // engine

#endif //MARLIN_SEARCH_H
