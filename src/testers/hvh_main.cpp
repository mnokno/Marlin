//
// Created by Jakub Nowacki on 20/11/2022.
//

#include <iostream>
#include "console_hvh_tester.h"

using namespace testers;

int main() {
    ConsoleHvHTester tester = *new ConsoleHvHTester();
    tester.startGame();
    return 0;
}