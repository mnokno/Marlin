//
// Created by kubaa on 19/11/2022.
//

#include "precalculated_data.h"
#include "bitops.h"

namespace engine {

    /**
     * Precalculates data that is used by the engine during calculations.
     */
    void PrecalculatedData::init() {
        // precalculates boarder masks
        // vertical boarders
        for (int i = 0; i < 2; i++){
            ulong mask = 0;
            for (int j = 0; j < 6; j++){
                mask ^= BitOps::flipBit(0,  j * 7 + i * 6);
            }
            boarderMasks[i] = mask;
        }
        // horizontal boarders
        for (int i = 0; i < 2; i++){
            ulong mask = 0;
            for (int j = 0; j < 7; j++){
                mask ^= BitOps::flipBit(0, j + i * 5 * 7);
            }
            boarderMasks[i + 2] = mask;
        }

        // precalculates columMasks
        for (int i = 0; i < 7; i++){
            ulong mask = 0;
            for (int j = 0; j < 6; j++){
                mask |= BitOps::flipBit(0, i + 7 * j);
            }
            columMasks[i] = mask;
        }

        // precalculates moveMasks
        // (sizeof(PrecalculatedData::moveMasks)/sizeof(*PrecalculatedData::moveMasks))
        for (int i = 0; i < 42; i++){
            moveMasks[i] = BitOps::flipBit(0, i);
        }

        // precalculates winingLinesMasks
        // positive ray directions
        int rayDirections[4] {1, 7, 8, 6};
        // for each position on the board
        for (int pos = 0; pos < 42; pos++){
            // in each direction there are 8
            for (int& rayDir : rayDirections){
                // finds the starting pos for the ray eight 3 away from pos or reached edge
                int startPos = pos;
                int stepBacks = 0;
                for (int i = 0; i < 3; i++){
                    // we know that the startPos is valid, now we need to check if the
                    // newPos would be valid to consider it for new start pos
                    int newPos = startPos - rayDir;

                    // catches the case where the new pos is outside the board
                    if (!isOnBoard(newPos)){
                        break;
                    }
                    // we had wrapped over the edge
                    if (hasWrappedOver(startPos, newPos)){
                        break;
                    }

                    // the position is valid hence we can update the start pos
                    startPos = newPos;
                    stepBacks++;
                }

                // check the validity of possible wining lines
                for (int i = 0; i < stepBacks + 1; i++){

                    ulong winingLine = 0;
                    int localPos = startPos + i * rayDir;
                    bool reachedEnd = false;
                    bool lineSuccessful = false;

                    for (int j = 0; j < 4; j++){
                        winingLine ^= BitOps::flipBit(0, localPos);
                        if (j == 3){
                            lineSuccessful = true;
                        }
                        localPos += rayDir;
                        // this mean that we had wrapped around the edge or gone out of the board
                        if (hasWrappedOver(localPos - rayDir, localPos) || !isOnBoard(localPos)){
                            reachedEnd = true;
                            break;
                        }
                    }

                    if (lineSuccessful){
                        winingLinesMasks[pos].push_back(winingLine);
                    }
                    if (reachedEnd){
                        break;
                    }
                }
            }
        }

        // precalculates adjacencySquareMasks
        short offsetDirections[8] {1, -1, 7, -7, 8, -8, 6, -6};
        for (short center = 0; center < 42; center++){
            ulong mask = 0;
            for (short & dir : offsetDirections){
                if (!hasWrappedOver(center, center + dir) && isOnBoard(center + dir)){
                    mask = BitOps::flipBit(mask, center + dir);
                }
            }
            adjacencySquareMasks[center] = mask;
        }

        // precalculate pillarMasks bases
        ulong pillarBaseMasks[7];
        for (int i = 0; i <= 6; i++){
            ulong mask = 0;
            mask = BitOps::flipBit(mask, i);
            if (!hasWrappedOver(i, i - 1) && isOnBoard(i - 1)){
                mask = BitOps::flipBit(mask, i - 1);
            }
            if (!hasWrappedOver(i, i + 1) && isOnBoard(i + 1)){
                mask = BitOps::flipBit(mask, i + 1);
            }
            pillarBaseMasks[i] = (mask | (mask << 7));
        }
        // precalculate pillarMasks
        for (int x = 0; x < 7; x++){
            for (int y = 0; y < 6; y++){
                ulong mask = 0;
                for (int h = y - 1; h < 5; h++){
                    mask |= pillarBaseMasks[x] << (h * 7);
                }
                // gets rid of overflow
                mask = ((mask << 22) >> 22);
                pillarMasks[x + y * 7] = mask;
            }
        }
    }

    /**
     * Parses an ulong to a string.
     *
     * @param number Ulong to be parsed.
     * @return String representation of the ulong.
     */
    std::string PrecalculatedData::formatUlong(ulong number){
        std::string formatted = "";
        for (ulong i = 0; i < 64; i++){
            if ((number & ((ulong)1 << i)) == (ulong)1 << i){
                formatted = "1" + formatted;
            }
            else{
                formatted = "0" + formatted;
            }
        }
        return formatted;
    }

    /**
     * Formats ulong representing a board mask to a string.
     *
     * @param number Ulong to be parsed.
     * @return String representation of the ulong.
     */
    std::string PrecalculatedData::formatBoard(ulong number){
        std::string formatted = "";
        for (ulong i = 0; i < 6; i++){
            std::string line = "";
            for (ulong j = 0; j < 7; j++){
                if ((number & ((ulong)1 << (j + i * 7))) == (ulong)1 << (j + i * 7)){
                    line = "1" + line;
                }
                else{
                    line = "0" + line;
                }
            }
            formatted = line + "\n" + formatted;
        }
        return formatted;
    }

    /**
     * Checks if th given pos/move is on the board.
     *
     * @param pos /move to be checked.
     * @return true if the pos/move is on the board, false otherwise.
     */
    bool PrecalculatedData::isOnBoard(int pos) {
        return (pos >= 0 && pos <= 41);
    }

    /**
     * Checks if the given position is on an edge of the board.
     *
     * @param pos Position to be checked.
     * @return true if the position is on an edge, false otherwise.
     */
    bool PrecalculatedData::isOnEdge(int pos) {
        for (ulong& border : boarderMasks){
            if ((border & moveMasks[pos]) != (ulong)0){
                return true;
            }
        }
        return false;
    }

    /**
     * Checks if the the given position has wrapped over the edge of the board.
     *
     * @param oldPos Old position, where we were coming from.
     * @param newPos New position, where we are going to.
     * @return true if the position has wrapped over the edge, false otherwise.
     */
    bool PrecalculatedData::hasWrappedOver(int oldPos, int newPos) {
        int oldX = oldPos % 7;
        int oldY = (oldPos - oldX) / 7;
        int newX = newPos % 7;
        int newY = (newPos - newX) / 7;
        return (abs(oldX - newX) > 1) || (abs(oldY - newY) > 1);
    }

} // engine