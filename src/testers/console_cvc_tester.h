//
// Created by Jakub Nowacki on 22/11/2022.
//

#ifndef MARLIN_CONSOLE_CVC_TESTER_H
#define MARLIN_CONSOLE_CVC_TESTER_H

#include "position.h"
#include "search.h"

using namespace engine;

namespace testers {

    class ConsoleCvCTester {
    public:
        ConsoleCvCTester();
        void startGame(int depthFirstAI, int depthSecondAI);
    private:
        Position position;
    };

} // tester

#endif //MARLIN_CONSOLE_CVC_TESTER_H
