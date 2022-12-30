//
// Created by Jakub Nowacki on 19/12/2022.
//

#include "server.h"

using namespace hosting;

int main() {
    Server server = *new Server();
    server.startServer();
    return 0;
}