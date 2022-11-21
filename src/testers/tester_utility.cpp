//
// Created by kubaa on 20/11/2022.
//

#include <iostream>
#include "tester_utility.h"
#include "types.h"
#include "bitops.h"

using namespace std;
using namespace engine;

namespace testers {
    int TesterUtility::getUserInput() {
        int move = -1;
        string input;
        while (move < 1 || move > 7){
            std::cout << "Enter column to play:";
            std::cin >> input;
            if (isInteger(input)){
                move = std::stoi(input);
            }
        }
        return move - 1;
    }

    bool TesterUtility::isInteger(string number) {
        for (char& c : number){
            if (!std::isdigit(c)){
                return false;
            }
        }
        return true;
    }

    std::string TesterUtility::formatPosition(Position position) {
        string formatted = "";
        for (int i = 0; i < 6; i++){
            string line = "";
            for (int j = 0; j < 7; j++){
                if ((position.getPosition(Player::YELLOW) & BitOps::flipBit(0, i * 7 + j)) != 0){
                    line += "X";
                }
                else if ((position.getPosition(Player::RED) & BitOps::flipBit(0, i * 7 + j)) != 0){
                    line += "O";
                }
                else{
                    line += "_";
                }
            }
            formatted = line + "\n" + formatted;
        }
        return formatted;
    }
} // testers