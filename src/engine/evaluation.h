//
// Created by kubaa on 21/11/2022.
//

#ifndef MARLIN_EVALUATION_H
#define MARLIN_EVALUATION_H

#include "position.h"

using namespace engine;

namespace engine {

    class Evaluation {
    public:
        static int eval(Position position);
    private:
        static int staticEval(Position position);
    };

} // engine

#endif //MARLIN_EVALUATION_H
