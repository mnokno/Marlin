//
// Created by kubaa on 22/11/2022.
//

#include "console_cvc_tester.h"
#include <iostream>
#include "position.h"
#include "tester_utility.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

namespace testers {

    ConsoleCvCTester::ConsoleCvCTester() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
        this->position = *new Position();
    }

    void ConsoleCvCTester::startGame(int depthFirstAI, int depthSecondAI) {
        // plays out the game
        while (this->position.getGameState() == GameState::ON_GOING){
            Search searchFirst = *new Search(position);
            position.makeMove(searchFirst.findBestMove(depthFirstAI));
            cout << TesterUtility::formatPosition(this->position) << endl;
            if (position.getGameState() != GameState::ON_GOING){
                break;
            }
            Search searchSecond = *new Search(position);
            position.makeMove(searchSecond.findBestMove(depthSecondAI));
            cout << TesterUtility::formatPosition(this->position) << endl;
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

} // tester