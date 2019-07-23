//
// Created by mich on 19/07/19.
//

#include "Sudoku9.h"

Sudoku9::Sudoku9(unsigned int level_) : _level{level_} {
   create_solution();
   filter_out_redundant_points();
}

void Sudoku9::initialize_table_to_constant(Table &table, int constant) {
   for (auto &row: table) {
      for (auto &entry: row) {
         entry = constant;
      }
   }
}

void Sudoku9::create_solution() {
   initialize_table_to_constant(_solution, 0);
   std::array < std::array < int, SIZE >, SIZE * SIZE > order;
   for (auto &entry: order) {
      Rand::override_random_array(entry);
   }
   try_to_fill_solution_entry_in_solution_table(0, order);
}

void Sudoku9::filter_out_redundant_points() {
   _task = _solution;
   Table residual_to_test = _task;
   std::vector<int> shuffled_indices = Rand::create_shuffled_vector(SIZE * SIZE);
   for (int idx: shuffled_indices) {
      unsigned int value_cand;
      for (value_cand = SIZE; value_cand != 0; --value_cand) {
         if (value_cand == _solution[idx / SIZE][idx % SIZE]) {
            continue;
         }
         residual_to_test = _task;
         residual_to_test[idx / SIZE][idx % SIZE] = value_cand;
         if (is_legal_solution_point(idx, residual_to_test)) {
            break;
         }
      }
      if(value_cand == 0) {
         _task[idx / SIZE][idx % SIZE] = 0;
         continue;
      }
      for (value_cand = SIZE; value_cand != 0; --value_cand) {
         if (value_cand == _solution[idx / SIZE][idx % SIZE]) {
            continue;
         }
         residual_to_test = _task;
         residual_to_test[idx / SIZE][idx % SIZE] = value_cand;
         if (is_legal_solution_point(idx, residual_to_test) and has_legal_extension(0, residual_to_test)) {
            break;
         }
      }
      _task[idx / SIZE][idx % SIZE] = value_cand == 0 and Rand::create_random_number(1, NUM_LEVELS) <= _level ? 0 : _solution[idx / SIZE][idx % SIZE];
   }
}


bool Sudoku9::try_to_fill_solution_entry_in_solution_table(int idx, const std::array<std::array<int, SIZE>,
      SIZE * SIZE> &random_orders) {
   if (idx == SIZE * SIZE) {
      return true;
   }
   for (int random_try: random_orders[idx]) {
      _solution[idx / SIZE][idx % SIZE] = random_try;
      if (is_legal_solution_point(idx, _solution) and
         try_to_fill_solution_entry_in_solution_table(idx + 1, random_orders)) {
         return true;
      }
   }
   _solution[idx / SIZE][idx % SIZE] = 0;
   return false;
}

bool Sudoku9::is_legal_solution_point(int idx, const Table &table) const {
   int fixed_row_idx = idx / SIZE, fixed_col_idx = idx % SIZE;
   for (unsigned int row_idx = 0; row_idx != SIZE; ++row_idx) {
      if (table[row_idx][fixed_col_idx] == table[fixed_row_idx][fixed_col_idx] and row_idx != fixed_row_idx) {
         return false;
      }
   }
   for (unsigned int col_idx = 0; col_idx != SIZE; ++col_idx) {
      if (table[fixed_row_idx][col_idx] == table[fixed_row_idx][fixed_col_idx] and col_idx != fixed_col_idx) {
         return false;
      }
   }
   for (unsigned int row_idx = 3 * (fixed_row_idx / 3); row_idx != 3 * (fixed_row_idx / 3 + 1); ++row_idx) {
      if (row_idx == fixed_row_idx) {
         continue;
      }
      for (unsigned int col_idx = 3 * (fixed_col_idx / 3); col_idx != 3 * (fixed_col_idx / 3 + 1); ++col_idx) {
         if (col_idx == fixed_col_idx) {
            continue;
         }
         if (table[row_idx][col_idx] == table[fixed_row_idx][fixed_col_idx]) {
            return false;
         }
      }
   }
   return true;
}

bool Sudoku9::has_legal_extension(int idx, Table &table) const {
   if (idx == SIZE * SIZE) {
      return true;
   }
   if (table[idx / SIZE][idx % SIZE] != 0) {
      return has_legal_extension(idx + 1, table);
   }
   for (unsigned int value_to_try = 1; value_to_try <= SIZE; ++value_to_try) {
      table[idx / SIZE][idx % SIZE] = value_to_try;
      if (is_legal_solution_point(idx, table) and has_legal_extension(idx + 1, table)) {
         return true;
      }
   }
   table[idx / SIZE][idx % SIZE] = 0;
   return false;
}