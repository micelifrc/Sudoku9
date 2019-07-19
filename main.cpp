#include <iostream>
#include "Sudoku9.h"

int main() {
   Rand::initialize_seed();

   // the following is only to show that Sudoku9 works correctly
   Sudoku9 sudoku;
   Sudoku9::print_table(sudoku.solution());
   std::cout << std::endl;
   Sudoku9::print_table(sudoku.puzzle());
   return 0;
}