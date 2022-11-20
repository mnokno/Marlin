//
// Created by kubaa on 19/11/2022.
//

#include "precalculated_data.h"

namespace engine {

    void PrecalculatedData::init() {
        // precalculates boarder masks
        // vertical boarders
        for (int i = 0; i < 2; i++){
            ulong mask = 0;
            for (int j = 0; j < 7; j++){
                mask ^= flipBit(0,  j * 7 + i * 6);
            }
            PrecalculatedData::boarderMasks[i] = mask;
        }
        // horizontal boarders
        for (int i = 0; i < 2; i++){
            ulong mask = 0;
            for (int j = 0; j < 7; j++){
                mask ^= flipBit(0, j + i * 5 * 7);
            }
            PrecalculatedData::boarderMasks[i + 2] = mask;
        }

        // TODO
        // precalculates winingLinesMasks

        // precalculates columMasks
        for (int i = 0; i < 7; i++){
            ulong mask = 0;
            for (int j = 0; j < 6; j++){
                mask |= PrecalculatedData::flipBit(0, i + 7 * j);
            }
            PrecalculatedData::columMasks[i] = mask;
        }

        // precalculates moveMasks
        // (sizeof(PrecalculatedData::moveMasks)/sizeof(*PrecalculatedData::moveMasks))
        for (int i = 0; i < 42; i++){
            PrecalculatedData::moveMasks[i] = PrecalculatedData::flipBit(0, i);
        }
    }

    ulong PrecalculatedData::flipBit(ulong map, int bitToFlip) {
        ulong a = (ulong)((ulong)0 ^ ((ulong)1 << (ulong)bitToFlip));
        return a;
    }

    std::string PrecalculatedData::format(ulong number){
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

} // engine