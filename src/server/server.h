//
// Created by kubaa on 18/12/2022.
//

#ifndef MARLIN_SERVER_H
#define MARLIN_SERVER_H

#include <string>
#include <map>
#include "position.h"
#include "transposition_table.h"
#include "search.h"


using namespace engine;
using namespace std;

namespace hosting {

    class Server {
    public:
        int startServer();

    private:
        int runServer(string port);
        string handleInitializeEngineRequest(int TTMemoryPool);
        string handleMoveRequest(int opponentMove, int timeLimit);
        map<string, string> stringToMap(const string& input);

        Position* position;
        TranspositionTable* transpositionTable;
        Search* search;
    };

} // hosting

#endif //MARLIN_SERVER_H
