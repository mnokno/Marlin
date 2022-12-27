//
// Created by kubaa on 18/12/2022.
//

// developed using recourses from
// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

// STANDARD REQUESTS
// requestType:moveCalculation,opponentMove:0,timeLimit:2000
// requestType:initialization,TTMemoryPool:3000

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <ws2tcpip.h>
#include <iostream>
#include "server.h"

// includes engine
#include "position.h"
#include "transposition_table.h"
#include "search.h"
#include "precalculated_data.h"
#include "zobrist_hashing.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;
using namespace engine;

namespace hosting {

    /**
     * Starts the server at DEFAULT_PORT.
     *
     * @return 0 if server exited successfully, 1 otherwise
     */
    int Server::startServer() {
        return runServer(DEFAULT_PORT);
    }

    /**
     * Runs the server at the given port.
     *
     * @return 0 if server exited successfully, 1 otherwise
     */
    int Server::runServer(string port) {

        WSADATA wsaData;
        int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        int iSendResult;
        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
        if ( iResult != 0 ) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            return 1;
        }

        // Create a SOCKET for the server to listen for client connections.
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        // No longer need server socket, since a connection has been made and this server is single-threaded
        closesocket(ListenSocket);

        // Receive until the peer shuts down the connection
        do {

            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                // Logs received message
                printf("---------------------------------\n");
                printf("Bytes received: %d\n", iResult);
                printf("Message received: %s\n", recvbuf);

                // converts the message to a map
                map<string, string> request = stringToMap(recvbuf);

                string response = "ERROR";
                if (request.contains("requestType")){
                    if (request["requestType"] == "initialization"){
                        if (request.contains("TTMemoryPool")){
                            response = handleInitializeEngineRequest(stoi(request["TTMemoryPool"]));
                        }
                    } else if (request["requestType"] == "moveCalculation"){
                        if (request.contains("opponentMove") && request.contains("timeLimit")) {
                            response = handleMoveRequest(stoi(request["opponentMove"]), stoi(request["timeLimit"]));
                        }
                    }
                }

                // Echo the buffer back to the sender
                iSendResult = send( ClientSocket, response.c_str(),  response.size(), 0 );
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return 1;
                }
                printf("Bytes sent: %llu\n", response.size());
                printf("Message sent: %s\n", response.c_str());
                printf("---------------------------------\n");
            }
            else if (iResult == 0)
                printf("Connection closing...\n");
            else  {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

            memset(recvbuf, 0, sizeof recvbuf);
        } while (iResult > 0);

        // shutdown the connection since we're done
        iResult = shutdown(ClientSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        // cleanup
        closesocket(ClientSocket);
        WSACleanup();

        return 0;
    }

    /**
     * Handles the initialize engine request.
     *
     * @param TTMemoryPool the memory pool for the transposition table
     * @return the response to the request
     */
    string Server::handleInitializeEngineRequest(int TTMemoryPool) {
        // deletes previews data
        delete this->position;
        delete this->transpositionTable;
        delete this->search;
        // precalculates data
        PrecalculatedData::init();
        ZobristHashing::initHashes();
        // creates new objects
        this->position = new Position();
        this->transpositionTable = new TranspositionTable(TranspositionTable::calculateTableCapacity(TTMemoryPool));
        this->search = new Search(*this->position, *this->transpositionTable);
        // returns response
        return "exitCode:0";
    }

    /**
     * Handles the move request.
     *
     * @param opponentMove the opponent's move
     * @param timeLimit the time limit for the engine to make a move
     * @return the response to the request
     */
    string Server::handleMoveRequest(int opponentMove, int timeLimit) {
        // updates position
        this->position->makeMove(opponentMove);
        // searches for the best move
        int bestMove = this->search->findBestMoveIn(timeLimit);
        // updates position
        this->position->makeMove(bestMove);
        // returns response
        return "exitCode:0 move:" + to_string(bestMove) + " gameStatus:" + to_string(this->position->getGameState());
    }

    /**
     * Converts input string to a map, expected input key:value,key:value,...
     *
     * @param input the input string
     * @return the string converted to a map
     */
    map<string, string> Server::stringToMap(const string& input) {
        // Initialize the input string and the output map
        map<string, string> output;

        // Split the input string on the ',' character to get a list of key-value pairs
        vector<string> pairs;
        istringstream iss(input);
        string pair;
        while (getline(iss, pair, ',')) {
            pairs.push_back(pair);
        }

        // Iterate through the list of key-value pairs and add them to the output map
        for (const string& p : pairs) {
            // Split the pair on the ':' character to separate the key and value
            istringstream pairIss(p);
            vector<string> pairParts;
            string pairPart;
            while (std::getline(pairIss, pairPart, ':')) {
                pairParts.push_back(pairPart);
            }

            // Add the key-value pair to the output map
            if (pairParts.size() == 2) {
                output[pairParts[0]] = pairParts[1];
            }
        }

        // Print the contents of the output map
        //for (const auto& [key, value] : output) {
        //    cout << key << "-" << value << endl;
        //}

        return output;
    }

} // hosting