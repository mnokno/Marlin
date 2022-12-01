//
// Created by kubaa on 25/11/2022.
//

#include "move_ordering.h"
#include <iostream>

namespace engine {

    void MoveOrdering::orderMove(int *&moves, int size, Position &position) {
        // defines an array of scores
        int* scores = new int[size]();

        // scores each move
        for (int i = 0;  i < size; i++){
            scores[i] = position.getStackHeight(moves[i]);
        }

        // sorts the moves according to scores
        bobbleSort(moves, scores, size);
    }


    void MoveOrdering::orderMove(int *&moves, int size, Position &position, TranspositionTable& transpositionTable) {
        // defines an array of scores
        int* scores = new int[size]();

        bool hasFound = false;
        TTEntry entry = transpositionTable.probe(position.getHash(), hasFound);

        // scores each move
        for (int i = 0;  i < size; i++){
            scores[i] = position.getStackHeight(moves[i]);
            if (hasFound && moves[i] == entry.getMove()){
                scores[i] += entry.getEval() << 6;
            }
        }

        // sorts the moves according to scores
        bobbleSort(moves, scores, size);
    }

    void MoveOrdering::orderMove(int *&moves, int size, Position &position, int previousBestMove) {
        // defines an array of scores
        int* scores = new int[size]();

        // scores each move
        for (int i = 0;  i < size; i++){
            scores[i] = position.getStackHeight(moves[i]);
            if (moves[i] == previousBestMove){
                scores[i] += 1000;
            }
        }

        // sorts the moves according to scores
        bobbleSort(moves, scores, size);
    }

    void MoveOrdering::bobbleSort(int *&moves, int *&scores, int size) {
        for (int i = 0; i < size - 1; i++){
            for (int j = 0; j < size - 1 - i; j++){
                if (scores[j] < scores[j + 1]){
                    swap(scores[j],scores[j + 1]);
                    swap(moves[j],moves[j + 1]);
                }
            }
        }
    }

} // engine