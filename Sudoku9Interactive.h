#ifndef SUDOKU_SUDOKU9INTERACTIVE_H
#define SUDOKU_SUDOKU9INTERACTIVE_H

#include <ncurses.h>
#include "Sudoku9.h"

class Sudoku9Interactive {
public:
   static constexpr unsigned int SIZE = Sudoku9::SIZE;
   typedef Sudoku9::Table Table;

   explicit Sudoku9Interactive();

   ~Sudoku9Interactive();

   bool play();

private:
   bool initial_log();

   bool play_single_game();

   enum Compass : int {
      North = -9, East = 1, South = 9, West = -1
   };

   void create_sudoku_map();

   bool try_move(Compass direction);

   void clear_all();

   void display_solution();

   bool try_clear(int position);

   bool try_record(int position, int number);

   void print_all() const;

   void print_entry(unsigned int idx) const;

   void check_legality();

   bool is_legal_term(unsigned int idx) const;

   static void start_colors();

   enum GridEntryStatus {
      Empty, Fixed, Legal, Illegal
   };

   struct GridEntry {
      int number;  // from 0 to 9 (0 means empty)
      GridEntryStatus status;

      explicit GridEntry(int entry_ = 0, GridEntryStatus status_ = Empty) : number{entry_}, status{status_} {}
   };

   static constexpr int BLACK_WHITE = 1;
   static constexpr int BLACK_YELLOW = 2;
   static constexpr int BLUE_WHITE = 3;
   static constexpr int BLUE_YELLOW = 4;
   static constexpr int RED_WHITE = 5;
   static constexpr int RED_YELLOW = 6;

   static int start_pos_x, start_pos_y;

   unsigned int _level;
   int _position;
   Table _solution;
   std::array<std::array<GridEntry, 9>, 9> _grid;
};


#endif //SUDOKU_SUDOKU9INTERACTIVE_H
