#ifndef SUDOKU_SUDOKU9_H
#define SUDOKU_SUDOKU9_H

#include "Rand.h"

// This class will create a sudoku task.
// We can get the puzzle with task(), and can get the solution with solution()
// Currently all task are deterministic (i.e. will never require to guess)
class Sudoku9 {
public:
   static constexpr unsigned int SIZE = Rand::SIZE;
   static constexpr unsigned int NUM_LEVELS = 4;
   typedef std::array<std::array<int, SIZE>, SIZE> Table;

   explicit Sudoku9(unsigned int level_ = 0);

   const Table &solution() const { return _solution; }

   const Table &task() const { return _task; }

   static void print_table(const Table &table);

   static void initiaze_table_to_constant(Table &table, int constant);

private:
   void create_solution();

   // this will create the task from the solution, by removing some redundant points.
   // This can probably be improved
   void filter_out_redundant_points();

   bool
   try_to_fill_solution_entry_in_solution(int idx, const std::array<std::array<int, SIZE>, SIZE * SIZE> &random_orders);

   // return true if there are no conflicts with table[idx/9][idx%9]
   bool is_legal_solution_point(int idx, const Table &table) const;

   bool has_legal_extension(int idx, Table &table) const;

   Table _solution;
   Table _task;
   unsigned int _level;  // from 0 (easy) to NUM_LEVELS-1 (nightmare)
};


#endif //SUDOKU_SUDOKU9_H
