//
// Created by Jakub Nowacki on 20/11/2022.
//

#include <iostream>
#include "console_hvh_tester.h"
#include "position.h"
#include "tester_utility.h"
#include "zobrist_hashing.h"
#include "precalculated_data.h"

using namespace engine;

namespace testers {
    ConsoleHvHTester::ConsoleHvHTester() {
        ZobristHashing::initHashes();
        PrecalculatedData::init();
        this->position = *new Position();
    }

    void ConsoleHvHTester::startGame() {
        // plays out the game
        while (this->position.getGameState() == GameState::ON_GOING){
            cout << TesterUtility::formatPosition(this->position) << endl;
            position.makeMove(position.convertFileToMove(TesterUtility::getUserInput()));
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