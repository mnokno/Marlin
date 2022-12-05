//
// Created by kubaa on 21/11/2022.
//

#include <iostream>
#include <thread>
#include "search.h"
#include "constants.h"
#include "evaluation.h"
#include "move_generator.h"
#include "precalculated_data.h"
#include "transposition_table.h"
#include "types.h"
#include "move_ordering.h"

namespace engine {

    Search::Search(Position& position, TranspositionTable& transpositionTable) : position(position), transpositionTable(transpositionTable) {
        this->transpositionTable = transpositionTable;
        this->position = position;
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;
        this->results = map<int, int>();
    }

    /**
     * Current best implementation fo the search algorithm.
     */
    int Search::findBestMove(int depth) {
        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        // at the binning there is no best move, hence score is greater than wining score
        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;

        // finds the best move from the generated moves
        for (int& move : MoveGenerator::generateMoves(position)){
            // evaluates this move
            position.makeMove(move);
            int score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
            position.unMakeMove();
            // logs branch evaluation data
            std::cout << "column: " + to_string(move % 7) << " score: " + to_string(score) << std::endl;
            // if this move is better than previous best move, it becomes the new best move
            if (score < minScore){
                minScore = score;
                betsMove = move;
            }
        }

        // logs data about this search
        std::cout << to_string(betsMove % 7) << ":"
                  << leafNodes << "l:"
                  << branchNodes << "b:"
                  << (leafNodes + branchNodes)  << "t:"
                  << TTHits << "tth:"
                  << to_string(minScore) << std::endl;

        // returns best move
        return betsMove;
    }

    /**
     * This method implements the search using algorithm at baseLevel.
     * Base 0, was fully tested manually can be used for automatic testing of enhancements.
     * Other bases can be also used for testing with greater depth and speed, but they need to be validated
     * using base level 0 first.
     */
    int Search::findBestMoveBaseTest(int depth, BaseLevel baseLevel) {
        //if (baseLevel == ALPHA_BETA_MT){
        //    return findBestMoveMT(depth);
        //}
        //// is PD (progressing deepening) enabled so different method needs to be used.
        //if (baseLevel == ALPHA_BETA_TT_MO_PD){
        //    return findBestMoveProgressiveTest(depth, baseLevel);
        //}

        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        // at the binning there is no best move, hence score is greater than wining score
        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;

        for (int& move : MoveGenerator::generateMoves(position)){
            int score;
            // evaluates this move using the specified base level algorithm
            position.makeMove(move);
            switch (baseLevel) {
                case MINI_MAX:
                    score = miniMax(depth - 1);
                    break;
                case ALPHA_BETA:
                    score = alphaBetaSimple(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
                    break;
                default:
                    throw std::invalid_argument("Base level not supported!");
            }
            position.unMakeMove();

            // if this move is better than previous best move, it becomes the new best move
            if (score < minScore){
                minScore = score;
                betsMove = move;
            }
        }

        // returns best move
        return betsMove;
    }

    /**
     * Finds the best move using the specified base level algorithm and progressive deepening.
     *
     * @param depth depth to which the search should be performed.
     * @param baseLevel Algorithm to be used for search.
     * @return best move.
     */
    int Search::findBestMoveProgressiveTest(int depth, BaseLevel baseLevel){
        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;
        // initial there is no best move
        int bestMove = -1;

        for (int cDepth = 1; cDepth <= depth; cDepth++){
            // at the binning there is no best move, hence score is greater than wining score
            int minScore = EVAL_INFINITY + 100;
            int iterationBetsMove = -1;

            // generate and orders moves
            list<int> moveList = MoveGenerator::generateMoves(this->position);
            int arr[moveList.size()];
            std::copy(moveList.begin(), moveList.end(), arr);
            int* moves = arr;
            MoveOrdering::orderMove(moves, moveList.size(), this->position, bestMove);

            for (int i = 0; i < moveList.size(); i++){
                int score;
                // evaluates this move using the specified base level algorithm
                position.makeMove(moves[i]);
                switch (baseLevel) {
                    case ALPHA_BETA_TT_MO_PD:
                        score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, cDepth - 1);
                        break;
                    default:
                        throw std::invalid_argument("Base level not supported!");
                }
                position.unMakeMove();

                // if this move is better than previous best move, it becomes the new best move
                if (score < minScore){
                    minScore = score;
                    iterationBetsMove = moves[i];
                }
            }

            // sets best move to the best move from this iteration (it's always the iteration from the highest depth)
            bestMove = iterationBetsMove;
        }


        // returns best move
        return bestMove;
    }

    int Search::findBestMoveMT(int depth) {
        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        vector<thread> threads;

        // finds the best move from the generated moves
        for (int& move : MoveGenerator::generateMoves(position)){
            // evaluates this move
            position.makeMove(move);
            threads.push_back(thread(searchTask, this, position, move, depth - 1));
            position.unMakeMove();
        }

        for (int i = 0; i < threads.size(); i++){
            threads[i].join();
        }

        // at the binning there is no best move, hence score is greater than wining score
        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;

        for ( auto [key, value]: results ) {
            if (value > minScore){
                minScore = value;
                betsMove = key;
            }
        }

        // logs data about this search
        std::cout << to_string(betsMove % 7) << ":"
                  << leafNodes << "l:"
                  << branchNodes << "b:"
                  << (leafNodes + branchNodes)  << "t:"
                  << TTHits << "tth:"
                  << to_string(minScore) << std::endl;

        // returns best move
        return betsMove;
    }

    void Search::searchTask(Search* search, Position lPosition, int move, int depth) {
        search->results.insert({move, alphaBetaStatic(-EVAL_INFINITY, EVAL_INFINITY, lPosition, search->transpositionTable, depth, *search)});
    }

#pragma region Algorythms

    /**
     * Basic implementation of the minimax algorithm without any enchantments.
     *
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::miniMax(int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // updates counters
        this->branchNodes++;

        // finds max value of this position
        int max = -EVAL_INFINITY;
        for (int& move : MoveGenerator::generateMoves(this->position))  {
            position.makeMove(move);
            int score = -miniMax( depthLeft - 1 );
            position.unMakeMove();
            if(score > max){
                max = score;
            }
        }

        // returns the max value
        return max;
    }

    /**
     * Minimax algorithm with alpha beta pruning, transposition table and move ordering.
     *
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::alphaBeta(int alpha, int beta, int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        //// checks if we have a transpositionTable match
        //bool hit;
        //TTEntry entry = this->transpositionTable.probe(this->position.getHash(), hit, alpha, beta);
        //// entry.getDepth() == depthLeft, has a high chance to catch a key collision
        //if (hit && entry.getDepth() == depthLeft){
        //    this->TTHits++;
        //    return entry.getEval();
        //}

        // updates counters
        this->branchNodes++;
        //// default node type
        //NodeType nodeType = UPPER_BOUND;
        //// keeps track of the best move
        //int bestMove = -1;

        // generate and orders moves
        //list<int> moveList = MoveGenerator::generateMoves(this->position);
        //int arr[moveList.size()];
        //std::copy(moveList.begin(), moveList.end(), arr);
        //int* moves = arr;
        //MoveOrdering::orderMove(moves, moveList.size(), this->position, transpositionTable);

        // finds max value of this position
        for (int& move : MoveGenerator::generateMoves(this->position)){
            this->position.makeMove(move);
            int score = -alphaBeta( -beta, -alpha, depthLeft - 1 );
            this->position.unMakeMove();
            if(score >= beta){
                //this->transpositionTable.save(position.getHash(), depthLeft, beta, moves[i], LOWER_BOUND);
                return beta;   //  fail hard beta-cutoff
            }
            if(score > alpha){
                //bestMove = move;
                //nodeType = EXACT;
                alpha = score; // alpha acts like max in MiniMax
            }
        }

        // adds the result to transposition table
        //this->transpositionTable.save(position.getHash(), depthLeft, alpha, bestMove, nodeType);

        // returns the max (alpha) value
        return alpha;
    }

    int Search::alphaBetaSimple(int alpha, int beta, int depthLeft){
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // updates counters
        this->branchNodes++;

        for (int& move : MoveGenerator::generateMoves(this->position)){
            position.makeMove(move);
            int score = -alphaBetaSimple(-beta, -alpha, depthLeft - 1);
            position.unMakeMove();

            if (score >= beta){
                return beta;
            }
            else if (score > alpha){
                alpha = score;
            }
        }

        return alpha;
    }

    // TODO
    int Search::alphaBetaStatic(int alpha, int beta, Position &position, TranspositionTable &tt, int depthLeft, Search& search) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if(depthLeft == 0 || position.getGameState() != GameState::ON_GOING) {
            search.leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(position) : -Evaluation::eval(position);
        }
        // checks if we have a transpositionTable match
        //bool hit;
        //TTEntry entry = tt.probe(position.getHash(), hit, alpha, beta);
        //// entry.getDepth() == depthLeft, has a high chance to catch a key collision
        //if (hit && entry.getDepth() == depthLeft){
        //    search.TTHits++;
        //    return entry.getEval();
        //}

        // updates counters
        search.branchNodes++;
        // default node type
        NodeType nodeType = UPPER_BOUND;
        // keeps track of the best move
        int bestMove = -1;

        // generate and orders moves
        list<int> moveList = MoveGenerator::generateMoves(position);
        int arr[moveList.size()];
        std::copy(moveList.begin(), moveList.end(), arr);
        int* moves = arr;
        MoveOrdering::orderMove(moves, moveList.size(), position, tt);

        // finds max value of this position
        for (int i = 0; i < moveList.size(); i++) {
            position.makeMove(moves[i]);
            int score = -alphaBetaStatic(-beta, -alpha, position, tt, depthLeft - 1, search);
            position.unMakeMove();
            if(score >= beta){
                tt.save(position.getHash(), depthLeft, beta, moves[i], LOWER_BOUND);
                return beta;   //  fail hard beta-cutoff
            }
            if(score > alpha){
                bestMove = moves[i];
                nodeType = EXACT;
                alpha = score; // alpha acts like max in MiniMax
            }
        }

        // adds the result to transposition table
        tt.save(position.getHash(), depthLeft, alpha, bestMove, nodeType);

        // returns the max (alpha) value
        return alpha;
    }

#pragma endregion Algorythms

#pragma region Getters

    /**
     * Getter for the number of leaf nodes.
     *
     * @return number of leafs nodes.
     */
    int Search::getLeafNodes() const {
        return this->leafNodes;
    }

    /**
     * Getter for the number of branch nodes.
     *
     * @return number of branch nodes.
     */
    int Search::getBranchNodes() const {
        return this->branchNodes;
    }

    /**
     *  Getter for the number of transposition table hits.
     *
     *  @return transposition table hits.
     */
    int Search::getTTHits() const {
        return this->TTHits;
    }

#pragma endregion Getters

} // engine