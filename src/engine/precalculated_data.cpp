//
// Created by kubaa on 19/11/2022.
//

#include "precalculated_data.h"
#include "bitops.h"

namespace engine {

    void PrecalculatedData::init() {
        // precalculates boarder masks
        // vertical boarders
        for (int i = 0; i < 2; i++){
            ulong mask = 0;
            for (int j = 0; j < 7; j++){
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
        int rayDirection[4] {1, 7, 8, 6};
        // for each position on the board
        for (int pos = 0; pos < 42; pos++){
            // in each direction there are 8
            for (int& rayDir : rayDirection){
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

                //if (pos == 41){
                //    std::cout << "Pos: " + std::to_string(pos) + " " +
                //            "StartPos: " + std::to_string(startPos) + " " +
                //            "StepBacks: " + std::to_string(stepBacks) + " " +
                //            "RayDir: " + std::to_string(rayDir) + " "
                //                << std::endl;
                //}

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
    }

    std::string PrecalculatedData::formatUlong(ulong number){
        std::string formatted = "";
        for (ulong i = 0; i < 42; i++){
            if ((number & ((ulong)1 << i)) == (ulong)1 << i){
                formatted = "1" + formatted;
            }
            else{
                formatted = "0" + formatted;
            }
        }
        return formatted;
    }

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

    bool PrecalculatedData::isOnBoard(int pos) {
        return (pos >= 0 && pos <= 41);
    }

    bool PrecalculatedData::isOnEdge(int pos) {
        for (ulong& border : boarderMasks){
            if ((border & moveMasks[pos]) != (ulong)0){
                return true;
            }
        }
        return false;
    }

    bool PrecalculatedData::hasWrappedOver(int oldPos, int newPos) {
        int oldX = oldPos % 7;
        int oldY = (oldPos - oldX) / 7;
        int newX = newPos % 7;
        int newY = (newPos - newX) / 7;
        return (abs(oldX - newX) > 1) || (abs(oldY - newY) > 1);
    }

} // engine