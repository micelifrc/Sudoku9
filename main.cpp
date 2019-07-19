#include <iostream>
#include "Sudoku9.h"

int main() {
   Rand::initialize_seed();

   // the following is only to show that Sudoku9 works correctly
   Sudoku9 sudoku_non_det(false);
   Sudoku9::print_table(sudoku_non_det.solution());
   std::cout << std::endl;
   Sudoku9::print_table(sudoku_non_det.puzzle());
   std::cout << std::endl << "-------------------\n" << std::endl;

   Sudoku9 sudoku_det(true);
   Sudoku9::print_table(sudoku_det.solution());
   std::cout << std::endl;
   Sudoku9::print_table(sudoku_det.puzzle());
   return 0;
}