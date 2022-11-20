//
// Created by kubaa on 19/11/2022.
//

#include "precalculated_data.h"

namespace engine {

    void PrecalculatedData::init() {
        // TODO
        // precalculates winingLinesMasks

        // TODO
        // precalculates columMasks

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