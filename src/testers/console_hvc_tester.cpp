//
// Created by kubaa on 22/11/2022.
//

#include "console_hvc_tester.h"
#include <iostream>
#include "position.h"
#include "constants.h"
#include "tester_utility.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

namespace testers {

    ConsoleHvCTester::ConsoleHvCTester() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
        this->position = *new Position();
    }

    void ConsoleHvCTester::startGame(bool humanFirst, int depth) {
        // plays out the game
        TranspositionTable transpositionTable = *new TranspositionTable(TranspositionTable::calculateTableCapacity(5000));
        Search search = *new Search(position, transpositionTable);
        if (humanFirst){
            while (this->position.getGameState() == GameState::ON_GOING){
                cout << TesterUtility::formatPosition(this->position) << endl;
                position.makeMove(position.convertFileToMove(TesterUtility::getUserInput()));
                if (position.getGameState() != GameState::ON_GOING){
                    break;
                }
                std::cout << "C1" << std::endl;
                position.makeMove(search.findBestMoveIn(5000));
                std::cout << "C2" << std::endl;
            }
        }
        else{
            while (this->position.getGameState() == GameState::ON_GOING){
                position.makeMove(search.findBestMoveIn(5000));
                if (position.getGameState() != GameState::ON_GOING){
                    break;
                }
                cout << TesterUtility::formatPosition(this->position) << endl;
                position.makeMove(position.convertFileToMove(TesterUtility::getUserInput()));
            }
        }

        // outputs winner
        cout << TesterUtility::formatPosition(this->position) << endl;
        if (position.getGameState() == GameState::YELLOW_WON){
            cout << "Yellow Won" << endl;
        }
        else if (position.getGameState() == GameState::RED_WON){
            cout << "Red Won" << endl;
        }
        else{
            cout << "Draw" << endl;
        }
    }
} // testers