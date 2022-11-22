//
// Created by kubaa on 22/11/2022.
//

#ifndef MARLIN_CONSOLE_HVC_TESTER_H
#define MARLIN_CONSOLE_HVC_TESTER_H

#include "position.h"
#include "search.h"

using namespace engine;

namespace testers {

    class ConsoleHvCTester {
    public:
        ConsoleHvCTester();
        void startGame(bool humanFirst, int depth);
    private:
        Position position;
    };

} // testers

#endif //MARLIN_CONSOLE_HVC_TESTER_H
