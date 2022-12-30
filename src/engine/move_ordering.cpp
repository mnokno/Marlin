//
// Created by Jakub Nowacki on 25/11/2022.
//

#include "move_ordering.h"
#include <iostream>
#include <random>

namespace engine {

    /**
     * Orders moves based on how close they are to the center of the board.
     *
     * @param moves Array of moves to be ordered.
     * @param size Size of the array.
     * @param position Position to which the move belong to, not used in the current implementation.
     */
    void MoveOrdering::orderMove(int *&moves, int size, Position &position) {
        // defines an array of scores
        int* scores = new int[size]();

        // scores each move
        for (int i = 0;  i < size; i++){
            int column = moves[i] % 7;
            scores[i] = column < 3 ? column : 6 - column;
        }

        // sorts the moves according to scores
        bobbleSort(moves, scores, size);
        delete [] scores;
    }

    /**
     * Shuffles the given array of moves.
     *
     * @param moves Array of moves to be shuffled.
     * @param size Size of the array.
     */
    void MoveOrdering::shuffleMoves(int *&moves, int size) {
        // NOT TESTED
        shuffle(moves, moves + size, std::mt19937(std::random_device()()));
    }

    /**
     * Performs a bobble sort on the given array of moves.
     * NOTE: Perhaps insertion sort would yield better results.
     * Max size of the array is 7, therefore this implementation is more
     * efficient than more complex algorithms like quicksort or mergesort.
     *
     * @param moves Array of moves to be sorted.
     * @param scores Scores by which the moves are sorted.
     * @param size Size of the arrays.
     */
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