//
// Created by mich on 19/07/19.
//

#include <iostream>
#include "Sudoku9.h"

Sudoku9::Sudoku9(bool deterministic) : _is_deterministic{deterministic} {
   create_solution();
   filter_out_redundant_points();
}

void Sudoku9::print_table(const Table &table) {
   std::string hline = "-------------------\n";
   std::cout << hline;
   for (unsigned int row_idx = 0; row_idx != SIZE; ++row_idx) {
      std::cout << "|";
      for (unsigned int col_idx = 0; col_idx != SIZE; ++col_idx) {
         char c = (col_idx == 2 or col_idx == 5) ? '|' : ' ';
         char number = table[row_idx][col_idx] == 0 ? ' ' : '0' + table[row_idx][col_idx];
         std::cout << number << c;
      }
      std::cout << "\b|\n";
      if (row_idx == 2 or row_idx == 5) {
         std::cout << hline;
      }
   }
   std::cout << hline;
}

void Sudoku9::initiaze_table_to_constant(Table &table, int constant) {
   for (auto &row: table) {
      for (auto &entry: row) {
         entry = constant;
      }
   }
}

void Sudoku9::create_solution() {
   initiaze_table_to_constant(_solution, 0);
   std::array<std::array<int, SIZE>, SIZE * SIZE> order;
   for (auto &entry: order) {
      Rand::override_random_array(entry);
   }
   try_to_fill_solution_entry_in_solution(0, order);
}

// using this version of filter_out_redundant_points we will never need to guess!
// it could be better to design a more complex version of filter_out_redundant_points to create more complex puzzles
void Sudoku9::filter_out_redundant_points() {
   _task = _solution;
   if (_is_deterministic) {
      std::vector<int> indices_left;
      indices_left.reserve(SIZE * SIZE);
      for (unsigned int idx = 0; idx != SIZE * SIZE; ++idx) {
         indices_left.emplace_back(idx);
      }
      int idx;
      for (unsigned int iter = 0; iter != 100000; ++iter) {
         unsigned int vector_idx = Rand::create_random_number(indices_left.size());
         idx = indices_left[vector_idx];
         unsigned int value_cand;
         for (value_cand = SIZE; value_cand != 0; --value_cand) {
            if (value_cand == _solution[idx / SIZE][idx % SIZE]) {
               continue;
            }
            _task[idx / SIZE][idx % SIZE] = value_cand;
            if (is_legal_solution_point(idx, _task)) {
               break;
            }
         }
         if (value_cand == 0) {
            // _task[idx/SIZE][idx%SIZE] doesn't have any other legal solution
            _task[idx / SIZE][idx % SIZE] = 0;
            std::swap(indices_left[vector_idx], indices_left.back());
            indices_left.pop_back();
         } else {
            _task[idx / SIZE][idx % SIZE] = _solution[idx / SIZE][idx % SIZE];
         }
      }
   } else {
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
            if (is_legal_solution_point(idx, residual_to_test) and has_legal_extension(0, residual_to_test)) {
               break;
            }
         }
         _task[idx / SIZE][idx % SIZE] = value_cand == 0 ? 0 : _solution[idx / SIZE][idx % SIZE];
      }
   }
}


bool Sudoku9::try_to_fill_solution_entry_in_solution(int idx, const std::array<std::array<int, SIZE>,
      SIZE * SIZE> &random_orders) {
   if (idx == SIZE * SIZE) {
      return true;
   }
   for (int random_try: random_orders[idx]) {
      _solution[idx / SIZE][idx % SIZE] = random_try;
      if (is_legal_solution_point(idx, _solution) and try_to_fill_solution_entry_in_solution(idx + 1, random_orders)) {
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