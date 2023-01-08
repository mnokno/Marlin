//
// Created by Jakub Nowacki on 21/11/2022.
//

#include <iostream>
#include <thread>
#include <unistd.h>
#include "search.h"
#include "constants.h"
#include "evaluation.h"
#include "move_generator.h"
#include "precalculated_data.h"
#include "transposition_table.h"
#include "types.h"
#include "move_ordering.h"

namespace engine {

    /**
     * Constructor for a new search
     *
     * @param position reference to the searched position
     * @param transpositionTable reference to the transposition table
     */
    Search::Search(Position& position, TranspositionTable& transpositionTable) : position(position), transpositionTable(transpositionTable) {
        this->transpositionTable = transpositionTable;
        this->position = position;
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        this->results = map<int, int>();
        this->leafCounts = map<int, int>();
        this->branchCounts = map<int, int>();
        this->TTCounts = map<int, int>();
    }

    /**
     * Current best implementation fo the search algorithm.
     *
     * @param depth Depth of the search
     * @return Returns the best found move at the specified depth
     */
    int Search::findBestMove(int depth) {
        return findBestMoveBaseTest(depth, BaseLevel::ALPHA_BETA);
    }

    /**
     * Find bet move in the given amount of time
     *
     * @param milliseconds How much time the function will have to find the best move in milliseconds
     * @return Returns the best found move
     */
    int Search::findBestMoveIn(int milliseconds) {
        int result = -1;
        thread search = thread(timedSearchTask, ref(result), position, ref(transpositionTable), milliseconds);
        search.join();
        if (result == -1){
            std::cout << "We should not be here" << std::endl;
        }
        return result;
    }

    /**
     * This method implements the search using algorithm at baseLevel.
     * Base 0, was fully tested manually can be used for automatic testing of enhancements.
     * Other bases can be also used for testing with greater depth and speed, but they need to be validated
     * using base level 0 first.
     *
     * @param depth Depth of the search
     * @param baseLevel search algorithm to use
     * @return reruns best move
     */
    int Search::findBestMoveBaseTest(int depth, BaseLevel baseLevel) {
        // rests counters used for data collection
        this->leafNodes = 0;
        this->branchNodes = 0;
        this->TTHits = 0;

        // at the binning there is no best move, hence score is greater than wining score
        int minScore = EVAL_INFINITY + 100;
        int betsMove = -1;

        if (baseLevel == ALPHA_BETA_MT){
            return findBestMoveABMT(depth, 12);
        }
        else if (baseLevel == MINI_MAX_MT){
            return findBestMoveMMMT(depth, 12);
        }

        for (int& move : MoveGenerator::generateMoves(position)){
            int score;
            // evaluates this move using the specified base level algorithm
            position.makeMove(move);
            switch (baseLevel) {
                case MINI_MAX:
                    score = miniMax(depth - 1);
                    break;
                case ALPHA_BETA_SIMPLE:
                    score = alphaBetaSimple(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
                    break;
                case ALPHA_BETA:
                    score = alphaBeta(-EVAL_INFINITY, EVAL_INFINITY, depth - 1);
                    break;
                default:
                    throw std::invalid_argument("Base level not supported!");
            }
            position.unMakeMove();
            //std::cout << "M:" << move << " S:" << score << std::endl;
            // if this move is better than previous best move, it becomes the new best move
            if (score < minScore || (score == minScore && move < betsMove)){
                minScore = score;
                betsMove = move;
            }
        }

        // returns best move
        return betsMove;
    }

    /**
     * Finds best move using minimax and utilizing multithreading
     *
     * @param depth Depth of the search
     * @param threads Number of threads to use
     * @return reruns best move
     */
    int Search::findBestMoveMMMT(int depth, int threads) {
        // stores threads
        vector<thread> threadPool;
        // will be used to stores the results of the search
        int bestMove = -1;
        // flags used to tell other threads to abort the search
        bool abort = false;
        // spawns search threads
        for (int i = 0; i < threads; i++){
            threadPool.push_back(thread(searchMiniMaxTask, position, depth, ref(bestMove), ref(abort), i));
        }
        // waits for all the threads to finish
        for (thread& thread : threadPool){
            thread.join();
        }
        // returns the best move
        return bestMove;
    }

    /**
     * Finds best move using alphaBeta and utilizing multithreading
     *
     * @param depth Depth of the search
     * @param threads Number of threads to use
     * @return Reruns best move
     */
    int Search::findBestMoveABMT(int depth, int threads) {
        // stores threads
        vector<thread> threadPool;
        // will be used to stores the results of the search
        int bestMove = -1;
        // flags used to tell other threads to abort the search
        bool abort = false;
        // spawns search threads
        for (int i = 0; i < threads; i++){
            threadPool.push_back(thread(searchAlphaBetaTask, position, depth, ref(bestMove), ref(abort), ref(transpositionTable), i));
        }
        // waits for all the threads to finish
        for (thread& thread : threadPool){
            thread.join();
        }
        // returns the best move
        return bestMove;
    }

    /**
     * Starts a search and saves the result, can by used to spawn multiple threads
     *
     * @param lPosition Copy of the position
     * @param depth Depth of the search
     * @param result Reference to the result
     * @param abort Reference to the abort flag
     * @param id thread id
     */
    void Search::searchMiniMaxTask(Position lPosition, int depth, int& result, bool& abort, int id) {
        // ensures that correct abort flag is used
        if (abort){
            std::cout << "Something is wrong wiht abort flags, we should not be here!" << std::endl;
            result = 0;
            return;
        }
        // ensures that the thread will only search ongoing passions
        if (depth == 0 || lPosition.getGameState() != GameState::ON_GOING) {
            std::cout << "Incorrect use of searchAlphaBetaTask, we should not be here!" << std::endl;
            result = lPosition.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(lPosition) : -Evaluation::eval(lPosition);
            return;
        }

        // generates moves, and shuffles them
        list<int> moveList = MoveGenerator::generateMoves(lPosition);
        int arr[moveList.size()];
        std::copy(moveList.begin(), moveList.end(), arr);
        int* moves = arr;
        MoveOrdering::shuffleMoves(moves, moveList.size());

        // keeps track of the bet move as they are progressively evaluated
        int bestMove = -1;
        int betsScore = -EVAL_INFINITY;

        for (int i = 0; i < moveList.size(); i++){
            // check if we should abort, other thread could have found a solution
            if (abort){
                std::cout << "thread " + to_string(id) +  " returns due to abort flag evaluating to true!" << std::endl;
                return;
            }

            std::cout << "Thread " + to_string(id) + " is searching move " + to_string(moves[i]) + "\n";

            // make.s the move
            lPosition.makeMove(moves[i]);
            // keeps track of this moves score
            int score = -Search::miniMaxStatic(lPosition, depth - 1, abort, id);
            // unmake the move
            lPosition.unMakeMove();

            // checks if this move is better than the current best
            if (score > betsScore){
                betsScore = score;
                bestMove = moves[i];
            }
        }

        // ensures that the result was calculated and not aborted
        std::cout << to_string(id) +  " FINISHED SEARCH A " + to_string(result) << std::endl;
        if (!abort){
            std::cout << to_string(id) + " FINISHED SEARCH B " + to_string(result)<< std::endl;
            // tell other thread that this one has found a solution, so they should abort
            abort = true;
            // result is passed by reference, main thread will retrieve the result from them
            result = bestMove;

            std::cout << to_string(id) + " FINISHED SEARCH C " + to_string(result) << std::endl;
        }
    }

    /**
     * Starts a search and saves the result, can by used to spawn multiple threads
     *
     * @param lPosition Copy of the position
     * @param depth Depth of the search
     * @param result Reference to the result
     * @param abort Reference to the abort flag
     * @param tt Reference to the transposition table
     * @param id thread id
     */
    void Search::searchAlphaBetaTask(Position lPosition, int depth, int& result, bool& abort, TranspositionTable& tt, int id) {
        // ensures that correct abort flag is used
        if (abort){
            std::cout << "Something is wrong wiht abort flags, we should not be here!" << std::endl;
            result = 0;
            return;
        }
        // ensures that the thread will only search ongoing passions
        if (depth == 0 || lPosition.getGameState() != GameState::ON_GOING) {
            std::cout << "Incorrect use of searchAlphaBetaTask, we should not be here!" << std::endl;
            result = lPosition.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(lPosition) : -Evaluation::eval(lPosition);
            return;
        }

        // generates moves, and shuffles them
        list<int> moveList = MoveGenerator::generateMoves(lPosition);
        int arr[moveList.size()];
        std::copy(moveList.begin(), moveList.end(), arr);
        int* moves = arr;
        MoveOrdering::shuffleMoves(moves, moveList.size());

        // keeps track of the bet move as they are progressively evaluated
        int bestMove = -1;
        int betsScore = -EVAL_INFINITY;

        for (int i = 0; i < moveList.size(); i++){
            // check if we should abort, other thread could have found a solution
            if (abort){
                return;
            }
            std::cout << "Thread " + to_string(id) + " is searching move " + to_string(moves[i]) + "\n";

            // make.s the move
            lPosition.makeMove(moves[i]);
            // keeps track of this moves score
            int score = -Search::alphaBetaStatic(-EVAL_INFINITY, EVAL_INFINITY, lPosition, tt, depth - 1, abort, id);
            // unmake the move
            lPosition.unMakeMove();

            // checks if this move is better than the current best
            if (score > betsScore){
                betsScore = score;
                bestMove = moves[i];
            }
        }

        // ensures that the result was calculated and not aborted
        if (!abort){
            // tell other thread that this one has found a solution, so they should abort
            abort = true;
            // result is passed by reference, main thread will retrieve the result from them
            result = bestMove;
        }
    }

    /**
     * Finds bets move in the given amount of time
     *
     * @param result Reference to the result
     * @param position Position to search
     * @param tt Transposition table
     * @param milliseconds Time to search in milliseconds
     */
    void Search::timedSearchTask(int& result, Position position, TranspositionTable& tt, int milliseconds) {
        // stores current search depth
        int currentDepth = 1;
        // reference to abort flag
        bool abort = false;
        // stores the evaluation of the current depths move
        int currentEval = 0;
        // start timer, will set abort flag to true when time is up
        thread waitThread = thread(abortAfter, ref(abort), ref(currentDepth), ref(currentEval), position.getMoveCount(), milliseconds);
        waitThread.detach();

        // start search using progressive deepening
        do {
            int bestMove = -1;
            int bestScore = -EVAL_INFINITY;

            for (int &move: MoveGenerator::generateMoves(position)) {
                position.makeMove(move);
                int score = -Search::alphaBetaStatic(-EVAL_INFINITY, EVAL_INFINITY, position, tt, currentDepth - 1,abort, 0);
                position.unMakeMove();
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }

            if (!abort) {
                result = bestMove;
                std::cout << "Calculated depth " + to_string(currentDepth) + " best move " + to_string(bestMove) + " score:" + to_string(bestScore) + "\n";
            }
            currentEval = bestScore;
            currentDepth++;

            // checks it the search has solved the game
            // its 43 not 42 because currentDepths is first incremented and then searched
            if (currentDepth + position.getMoveCount() >= 43){
                std::cout << "Exited progressive deepening loop because we had solved the position." << std::endl;
                break;
            }
            // checks if the search has a premature forced end
            if (abs(currentEval) > EVAL_INFINITY * .9){
                std::cout << "Exited progressive deepening loop because premature forced end has been found." << std::endl;
                break;
            }
        } while (!abort);
        thread waitForAbortAfterThread = thread(waitForAbortAfter, ref(abort));
        waitForAbortAfterThread.join();
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
     * Static implementation of minimax algorithm without any enchantments.
     *
     * @param position reference to the thread local position
     * @param depthLeft how many moves to look ahead
     * @param abort reference to the abort flag
     * @param id id of the thread performing the search (used to correctly assign collected data to the processes)
     * @return the score of the move
     */
    int Search::miniMaxStatic(Position& position, int depthLeft, bool& abort, int id) {
        // checks if we should abort
        if (abort){
            return 0;
        }

        // if the target depth was reach or the game is over, return the static evaluation of the position
        if(depthLeft == 0 || position.getGameState() != GameState::ON_GOING) {
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(position) : -Evaluation::eval(position);
        }

        // finds max value of this position
        int max = -EVAL_INFINITY;
        for (int& move : MoveGenerator::generateMoves(position))  {
            position.makeMove(move);
            int score = -miniMaxStatic(position, depthLeft - 1, abort, id);
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
     * @param alpha from the previous node, call with -infinity on first node
     * @param beta from the previous node, call with infinity on first node
     * @param depthLeft how many moves to look ahead
     * @return the score of the move
     */
    int Search::alphaBeta(int alpha, int beta, int depthLeft) {
        // if the target depth was reach or the game is over, return the static evaluation of the position
        if (depthLeft == 0 || this->position.getGameState() != GameState::ON_GOING) {
            this->leafNodes++;
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(this->position) : -Evaluation::eval(this->position);
        }

        // checks if we have a transpositionTable match
        bool hit;
        TTEntry entry = this->transpositionTable.probe(this->position.getHash(), depthLeft, hit, alpha, beta);
        // entry.getDepth() == depthLeft, has a high chance to catch a key collision
        if (hit) {
            this->TTHits++;
            return entry.getEval();
        }

        // updates counters
        this->branchNodes++;
        // default node type
        NodeType nodeType = UPPER_BOUND;
        // keeps track of the best move
        int bestMove = -1;

        // generates moves
        list<int> moveList = MoveGenerator::generateMoves(this->position);
        // converts the list to array
        int arr[moveList.size()];
        std::copy(moveList.begin(), moveList.end(), arr);
        int* moves = arr;
        // orders the moves
        MoveOrdering::orderMove(moves, moveList.size(), this->position);

        // finds max value of this position
        for (int i = 0; i < moveList.size(); i++){
            this->position.makeMove(moves[i]);
            int score = -alphaBeta( -beta, -alpha, depthLeft - 1 );
            this->position.unMakeMove();
            if(score >= beta){
                this->transpositionTable.save(position.getHash(), depthLeft + this->position.getMoveCount(), beta, moves[i], LOWER_BOUND);
                return beta;   //  fail hard beta-cutoff
            }
            if(score > alpha){
                bestMove = moves[i];
                nodeType = EXACT;
                alpha = score; // alpha acts like max in MiniMax
            }
        }

        // adds the result to transposition table
        this->transpositionTable.save(position.getHash(), depthLeft + this->position.getMoveCount(), alpha, bestMove, nodeType);

        // returns the max (alpha) value
        return alpha;
    }

    /**
     * Simple implementation of alpha beta using negamax for testing only
     *
     * @param alpha from the previous node, call with -infinity on first node
     * @param beta from the previous node, call with infinity on first node
     * @return the score of the move
     */
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

    /**
     * Static implementation of alphaBeta algorithm
     *
     * @param alpha from the previous node, call with -infinity on first node
     * @param beta from the previous node, call with infinity on first node
     * @param position reference to the thread local position
     * @param tt transposition table reference
     * @param depthLeft how many moves to look ahead
     * @param abort reference to the abort flag
     * @param id id of the thread performing the search (used to correctly assign collected data to the processes)
     * @return the score of the move
     */
    int Search::alphaBetaStatic(int alpha, int beta, Position &position, TranspositionTable &tt, int depthLeft, bool& abort, int id) {
        // checks if we should abort
        if (abort){
            return 0;
        }

        // if the target depth was reach or the game is over, return the static evaluation of the position
        if(depthLeft == 0 || position.getGameState() != GameState::ON_GOING) {
            return position.getPlayerToMove() == Player::YELLOW ? Evaluation::eval(position) : -Evaluation::eval(position);
        }

        // checks if we have a transpositionTable match
        bool hit;
        TTEntry entry = tt.probe(position.getHash(), depthLeft, hit, alpha, beta);
        if (hit) {
            return entry.getEval();
        }

        // default node type
        NodeType nodeType = UPPER_BOUND;
        // keeps track of the best move
        int bestMove = -1;

        // generate and orders moves
        list<int> moveList = MoveGenerator::generateMoves(position);
        int arr[moveList.size()];
        std::copy(moveList.begin(), moveList.end(), arr);
        int* moves = arr;
        MoveOrdering::orderMove(moves, moveList.size(), position);

        // finds max value of this position
        for (int i = 0; i < moveList.size(); i++) {
            position.makeMove(moves[i]);
            int score = -alphaBetaStatic(-beta, -alpha, position, tt, depthLeft - 1, abort, id);
            position.unMakeMove();
            if(score >= beta){
                tt.save(position.getHash(), depthLeft + position.getMoveCount(), beta, moves[i], LOWER_BOUND);
                return beta;   //  fail hard beta-cutoff
            }
            if(score > alpha){
                bestMove = moves[i];
                nodeType = EXACT;
                alpha = score; // alpha acts like max in MiniMax
            }
        }

        // adds the result to transposition table
        tt.save(position.getHash(), depthLeft + position.getMoveCount(), alpha, bestMove, nodeType);

        // returns the max (alpha) value
        return alpha;
    }

#pragma endregion Algorythms

#pragma region Other

    /**
     * Sets given abort flag to true after given time
     *
     * @param abortFlag reference to the abort flag
     * @param currentDepth current depth of the search, used to abort early if the search has solved the game
     * @param currentScore current score of the search, used to abort early if the search has solved the game
     * @param rootPositionDepth used to check if the search has solved the game
     * @param milliseconds how many milliseconds to wait before setting abort flag to true
     */
    void Search::abortAfter(bool &abortFlag, int& currentDepth, int& currentScore, int rootPositionDepth,  int milliseconds) {
        std::cout << "Search timer has been started." << std::endl;
        for (int i = 0; i < 100; i++){
            // usleep take useconds not milliseconds
            usleep(milliseconds);
            // checks it the search has solved the game
            // its 43 not 42 because currentDepths is first incremented and then searched
            if (currentDepth + rootPositionDepth >= 43){
                std::cout << "Abort flag has been set to true because we had solved the position." << std::endl;
                abortFlag = true;
                return;
            }
            // checks if the search has a premature forced end
            if (abs(currentScore) > EVAL_INFINITY * .9){
                std::cout << "Abort flag has been set to true because premature forced end has been found." << std::endl;
                abortFlag = true;
                return;
            }
        }
        std::cout << "Abort flag has been set to true because the time has run out. " << std::endl;
        abortFlag = true;
    }

    /**
     * Wait for the abortAfter thread to finish
     *
     * @param abortFlag reference to the abort flag
     */
    void Search::waitForAbortAfter(bool &abortFlag) {
        while (!abortFlag){
            usleep(10);
        }
    }

#pragma endregion

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