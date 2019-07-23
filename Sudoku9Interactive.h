#ifndef SUDOKU_SUDOKU9INTERACTIVE_H
#define SUDOKU_SUDOKU9INTERACTIVE_H

#include <ncurses.h>
#include "Sudoku9.h"

// will loop on a song
struct MusicPlayer {
   explicit MusicPlayer(const std::string &track_name);

   ~MusicPlayer();
};

// The actual game on 9x9 grids
class Sudoku9Interactive {
public:
   static constexpr int SIZE = Sudoku9::SIZE;
   typedef Sudoku9::Table Table;

   explicit Sudoku9Interactive();

   ~Sudoku9Interactive();

   bool play();

private:
   bool initial_log();

   bool play_single_game();

   enum Compass : int {
      North = -SIZE, East = 1, South = SIZE, West = -1
   };

   void create_sudoku_map();

   bool try_move(Compass direction);

   void clear_all();

   void display_solution();

   bool try_clear(int position);

   bool try_record(int position, int number);

   void switch_guess_mode();

   bool revert_guess();  // revert from previous guess

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
      int guess_idx;  // tells how many guesses there have been so far

      explicit GridEntry(int entry_ = 0, GridEntryStatus status_ = Empty, int guesses_idx_ = 0) :
            number{entry_}, status{status_}, guess_idx{guesses_idx_} {}
   };

   static constexpr int WHITE_BLACK = 1;
   static constexpr int YELLOW_BLACK = 2;
   static constexpr int RED_BLACK = 3;
   static constexpr int WHITE_BLUE = 4;
   static constexpr int YELLOW_BLUE = 5;
   static constexpr int RED_BLUE = 6;
   static constexpr int WHITE_GREEN = 7;
   static constexpr int YELLOW_GREEN = 8;
   static constexpr int RED_GREEN = 9;
   static constexpr int WHITE_MAGENTA = 10;
   static constexpr int YELLOW_MAGENTA = 11;
   static constexpr int RED_MAGENTA = 12;

   static int start_pos_x, start_pos_y;

   const std::string _opening_theme;
   const std::vector <std::string> _level_themes;

   unsigned int _level;
   int _guess_idx;
   bool _guess_mode;
   bool _remove_guess_mode_at_next_move;
   int _position;
   Table _solution;
   std::array<std::array<GridEntry, 9>, 9> _grid;
};


#endif //SUDOKU_SUDOKU9INTERACTIVE_H
