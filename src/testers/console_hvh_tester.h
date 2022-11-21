//
// Created by kubaa on 20/11/2022.
//

#ifndef MARLIN_CONSOLE_HVH_TESTER_H
#define MARLIN_CONSOLE_HVH_TESTER_H

#include "position.h"

using namespace engine;

namespace testers {

    class ConsoleHvHTester {
    public:
        ConsoleHvHTester();
        void startGame();
    private:
        Position position;
    };

} // testers

#endif //MARLIN_CONSOLE_HVH_TESTER_H
