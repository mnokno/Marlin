//
// Created by Jakub Nowacki on 20/11/2022.
//

#ifndef MARLIN_TESTER_UTILITY_H
#define MARLIN_TESTER_UTILITY_H

#include <istream>
#include "position.h"

using namespace engine;
using namespace std;

namespace testers {

    class TesterUtility {
    public:
        static bool isInteger(string number);
        static int getUserInput();
        static string formatPosition(Position position);
    };

} // testers

#endif //MARLIN_TESTER_UTILITY_H
