//
// Created by Jakub Nowacki on 22/11/2022.
//

#include <iostream>
#include "console_cvc_tester.h"

using namespace testers;

int main() {
    ConsoleCvCTester tester = *new ConsoleCvCTester();
    tester.startGame(2, 10);
    return 0;
}