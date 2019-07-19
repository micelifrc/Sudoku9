#include <iostream>
#include "Sudoku9Interactive.h"

int main() {
   Rand::initialize_seed();
   std::cout << "This program will allow to play some sudoku puzzles." << std::endl
             << "In any moment you can press 'q' to quit the program." << std::endl
             << "Type 'e' to start a new easy game." << std::endl
             << "Type 'h' to start a new hard game." << std::endl;
   char c = ' ';
   while (true) {
      c = getchar();
      if (c == 'q') {
         std::cout << "Thank you for playing with us :)" << std::endl;
         return 0;
      }
      if (c == 'e' or c == 'h') {
         Sudoku9Interactive sudoku_game(c == 'e');
         sudoku_game.play();
         return 0;
      }
   }
   return 0;
}