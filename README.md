# Marlin

Marlin is a high-performance Connect 4 engine designed specifically for a 7x6 board size, written in C++ and utilizing a bitboard representation of the game board for maximum efficiency.

The engine offers two options for search: the first uses a custom evaluation function, while the second uses a null evaluation. The latter is particularly effective for killer search.

To further enhance its searching capabilities, Marlin utilizes a range of optimizations, including alpha-beta pruning, aspiration windows, transposition tables, iterative deepening, and move ordering. These optimizations work together to improve the efficiency and effectiveness of the engine's search algorithm.

The engine's implementation also allows for easy integration into other applications or game interfaces.

Overall, Marlin is a powerful and efficient engine that provides a competitive edge in the game of Connect 4. Its advanced search capabilities, paired with its easy integration, make it a valuable tool for any Connect 4 enthusiast or developer.
