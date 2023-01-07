//
// Created by Jakub Nowacki on 18/12/2022.
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
        string handleInitializeEngineRequest(map<string, string> &request);
        string handleMoveRequest(map<string, string> &request);
        string handleNewGameRequest(map<string, string> &request);
        map<string, string> stringToMap(const string& input);
        static string formatPosition(Position position);

        Position* position;
        TranspositionTable* transpositionTable;
        Search* search;
    };

} // hosting

#endif //MARLIN_SERVER_H
