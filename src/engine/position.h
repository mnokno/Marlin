//
// Created by kubaa on 20/11/2022.
//

#ifndef MARLIN_POSITION_H
#define MARLIN_POSITION_H

#include <stack>
#include "types.h"

using namespace std;

namespace engine{
    class Position {
    public:
        Position();
        void makeMove(int move);
        void unMakeMove();

        ulong getPosition(Player player);
        ulong getPosition(int player);
        GameState getGameState();
        Player getPlayerToMove();
        ulong getHash();
        short getStackHeight(short stack);
        short getMoveCount();
        int convertFileToMove(int column);
    private:
        GameState gameStateAfterMove(int move, Player playerWhoMoved);

        ulong positions[2];
        GameState gameState;
        Player playerToMove;
        ulong hash;
        short stackHeights[7];
        stack<int> history;
        short moveCount;
    };
}



#endif //MARLIN_POSITION_H
