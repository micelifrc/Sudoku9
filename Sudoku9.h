#ifndef SUDOKU_SUDOKU9_H
#define SUDOKU_SUDOKU9_H

#include "Rand.h"

// This class will create a sudoku puzzle.
// We can get the puzzle with get_puzzle(), and can get the solution with get_solution()
class Sudoku9 {
public:
   static constexpr int SIZE = Rand::SIZE;
   static constexpr int NUM_LEVELS = 4;
   typedef std::array<std::array<int, SIZE>, SIZE> Table;

   explicit Sudoku9(unsigned int level_ = 0);

   const Table &get_solution() const { return _solution; }

   const Table &get_puzzle() const { return _puzzle; }

   const unsigned int get_level() const { return _level; }

   static void initialize_table_to_constant(Table &table, int constant);

private:
   void create_solution();

   // this will create the task from the solution, by removing some redundant points.
   // This can probably be improved
   void filter_out_redundant_points();

   bool try_to_fill_solution_entry_in_solution_table(int idx,
         const std::array<std::array<int, SIZE>, SIZE * SIZE> &random_orders);

   // return true if there are no conflicts with table[idx/9][idx%9]
   bool is_legal_solution_point(int idx, const Table &table) const;

   bool has_legal_extension(int idx, Table &table) const;

   Table _solution;
   Table _puzzle;
   unsigned int _level;  // from 0 (easy) to NUM_LEVELS-1 (nightmare)
};


#endif //SUDOKU_SUDOKU9_H
