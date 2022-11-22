//
// Created by kubaa on 22/11/2022.
//

#include <iostream>
#include "console_hvc_tester.h"

using namespace testers;

int main() {
    ConsoleHvCTester tester = *new ConsoleHvCTester();
    tester.startGame(true, 8);
    return 0;
}