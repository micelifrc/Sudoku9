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
   for (unsigned int row_idx = 0; row_idx != 9; ++row_idx) {
      std::cout << "|";
      for (unsigned int col_idx = 0; col_idx != 9; ++col_idx) {
         char c = (col_idx == 2 or col_idx == 5) ? '|' : ' ';
         char number = table[row_idx][col_idx] == 0 ? ' ' : '0' + table[row_idx][col_idx];
         std::cout << number << c;
      }
      std::cout << "\b|\n";
      if(row_idx == 2 or row_idx == 5) {
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
   std::array<std::array<int, 9>, 81> order;
   for (auto &entry: order) {
      Rand::override_random_array(entry);
   }
   try_to_fill_solution_entry_in_solution(0, order);
}

// using this version of filter_out_redundant_points we will never need to guess!
// it could be better to design a more complex version of filter_out_redundant_points to create more complex puzzles
void Sudoku9::filter_out_redundant_points() {
   _puzzle = _solution;
   if (_is_deterministic) {
      std::vector<int> indices_left;
      indices_left.reserve(81);
      for (unsigned int idx = 0; idx != 81; ++idx) {
         indices_left.emplace_back(idx);
      }
      int idx;
      for (unsigned int iter = 0; iter != 100000; ++iter) {
         unsigned int vector_idx = Rand::create_random_number(indices_left.size());
         idx = indices_left[vector_idx];
         unsigned int value_cand;
         for (value_cand = 9; value_cand != 0; --value_cand) {
            if (value_cand == _solution[idx / 9][idx % 9]) {
               continue;
            }
            _puzzle[idx / 9][idx % 9] = value_cand;
            if (is_legal_solution_point(idx, _puzzle)) {
               break;
            }
         }
         if (value_cand == 0) {
            // _puzzle[idx/9][idx%9] doesn't have any other legal solution
            _puzzle[idx / 9][idx % 9] = 0;
            std::swap(indices_left[vector_idx], indices_left.back());
            indices_left.pop_back();
         } else {
            _puzzle[idx / 9][idx % 9] = _solution[idx / 9][idx % 9];
         }
      }
   } else {
      Table residual_to_test = _puzzle;
      std::vector<int> shuffled_indices = Rand::create_shuffled_vector(81);
      for (int idx: shuffled_indices) {
         unsigned int value_cand;
         for (value_cand = 9; value_cand != 0; --value_cand) {
            if (value_cand == _solution[idx / 9][idx % 9]) {
               continue;
            }
            residual_to_test = _puzzle;
            residual_to_test[idx / 9][idx % 9] = value_cand;
            if (is_legal_solution_point(idx, residual_to_test) and has_legal_extension(0, residual_to_test)) {
               break;
            }
         }
         _puzzle[idx / 9][idx % 9] = value_cand == 0 ? 0 : _solution[idx / 9][idx % 9];
      }
   }
}


bool Sudoku9::try_to_fill_solution_entry_in_solution(int idx, const std::array<std::array<int, 9>, 81> &random_orders) {
   if (idx == 81) {
      return true;
   }
   for (int random_try: random_orders[idx]) {
      _solution[idx / 9][idx % 9] = random_try;
      if (is_legal_solution_point(idx, _solution) and try_to_fill_solution_entry_in_solution(idx + 1, random_orders)) {
         return true;
      }
   }
   _solution[idx / 9][idx % 9] = 0;
   return false;
}

bool Sudoku9::is_legal_solution_point(int idx, const Table &table) const {
   int fixed_row_idx = idx / 9, fixed_col_idx = idx % 9;
   for (unsigned int row_idx = 0; row_idx != 9; ++row_idx) {
      if (table[row_idx][fixed_col_idx] == table[fixed_row_idx][fixed_col_idx] and row_idx != fixed_row_idx) {
         return false;
      }
   }
   for (unsigned int col_idx = 0; col_idx != 9; ++col_idx) {
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
   if (idx == 81) {
      return true;
   }
   if (table[idx / 9][idx % 9] != 0) {
      return has_legal_extension(idx + 1, table);
   }
   for (unsigned int value_to_try = 1; value_to_try <= 9; ++value_to_try) {
      table[idx / 9][idx % 9] = value_to_try;
      if (is_legal_solution_point(idx, table) and has_legal_extension(idx + 1, table)) {
         return true;
      }
   }
   table[idx / 9][idx % 9] = 0;
   return false;
}