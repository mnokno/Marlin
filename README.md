# Marlin

Marlin is a connect 4 engine designed for a 7x6 connect 4 board size, written in c++ for maximum performance using only bitboard representation of the position. For killer search a null evaluation can be used, otherwise a custom evaluation function is used. For search the engine uses mini-max algorithm with the following enchantments: alpha-beta pruning, aspiration windows, transposition tables, iterative deepening and move ordering.
