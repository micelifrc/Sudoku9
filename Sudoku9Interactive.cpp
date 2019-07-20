//
// Created by mich on 19/07/19.
//

#include "Sudoku9Interactive.h"

int Sudoku9Interactive::start_pos_x = 0, Sudoku9Interactive::start_pos_y = 0;

Sudoku9Interactive::Sudoku9Interactive() :
      _level{0}, _position{(SIZE * SIZE) / 2}, _solution(), _grid() {
   initscr();
   start_colors();
   keypad(stdscr, true);
   curs_set(0);
   noecho();
   cbreak();
   getmaxyx(stdscr, start_pos_y, start_pos_x);
   start_pos_y = (start_pos_y - 3 * SIZE) / 2;
   start_pos_x = (start_pos_x - 5 * SIZE) / 2;
   if (initial_log()) {
      play();
   }
}

Sudoku9Interactive::~Sudoku9Interactive() {
   curs_set(1);
   echo();
   nocbreak();
   endwin();
}

bool Sudoku9Interactive::play() {
   bool keep_playing = true;
   while (keep_playing) {
      create_sudoku_map();
      _position = (SIZE * SIZE) / 2;
      keep_playing = play_single_game();
   }
}

bool Sudoku9Interactive::initial_log() {
   clear();
   mvprintw(start_pos_y, start_pos_x, "Press 'q' to quit the game");
   mvprintw(start_pos_y + 2, start_pos_x, "Press 'e' to start an easy game");
   mvprintw(start_pos_y + 4, start_pos_x, "Press 'i' to start an intermediate game");
   mvprintw(start_pos_y + 6, start_pos_x, "Press 'h' to start an hard game");
   mvprintw(start_pos_y + 8, start_pos_x, "Press 'n' to start a nightmare game");
   refresh();
   int command;
   while (true) {
      command = getch();
      switch (command) {
         case 'q':
         case 'Q' : {
            return false;
         }
         case 'e':
         case 'E': {
            _level = 0;
            return true;
         }
         case 'i':
         case 'I': {
            _level = 1;
            return true;
         }
         case 'h':
         case 'H': {
            _level = 2;
            return true;
         }
         case 'n':
         case 'N': {
            _level = 3;
            return true;
         }
         default: {
            break;
         }
      }
   }
}

bool Sudoku9Interactive::play_single_game() {
   print_all();
   int command;
   while (true) {
      command = getch();
      switch (command) {
         case KEY_UP: {
            try_move(North);
            break;
         }
         case KEY_RIGHT: {
            try_move(East);
            break;
         }
         case KEY_DOWN: {
            try_move(South);
            break;
         }
         case KEY_LEFT: {
            try_move(West);
            break;
         }
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': {
            try_record(_position, command - '0');
            break;
         }
         case '0': {
            try_clear(_position);
            break;
         }
         case 'c':
         case 'C': {
            clear_all();
            break;
         }
         case 's':
         case 'S' : {
            display_solution();
            break;
         }
         case 'e':
         case 'E': {
            _level = 0;
            return true;
         }
         case 'i':
         case 'I': {
            _level = 1;
            return true;
         }
         case 'h':
         case 'H': {
            _level = 2;
            return true;
         }
         case 'n':
         case 'N': {
            _level = 3;
            return true;
         }
         case 'q':
         case 'Q': {
            return false;
         }
         default: {
            break;
         }
      }
   }
}

void Sudoku9Interactive::create_sudoku_map() {
   Sudoku9 sudoku9(_level);
   _solution = sudoku9.solution();
   const Table &task = sudoku9.task();
   for (unsigned int row_idx = 0; row_idx != SIZE; ++row_idx) {
      for (unsigned int col_idx = 0; col_idx != SIZE; ++col_idx) {
         if (task[row_idx][col_idx] == 0) {
            _grid[row_idx][col_idx].number = 0;
            _grid[row_idx][col_idx].status = Empty;
         } else {
            _grid[row_idx][col_idx].number = task[row_idx][col_idx];
            _grid[row_idx][col_idx].status = Fixed;
         }
      }
   }
}

bool Sudoku9Interactive::try_move(Compass direction) {
   switch (direction) {
      case North: {
         if (_position < SIZE) return false;
         break;
      }
      case East: {
         if ((_position + 1) % SIZE == 0) return false;
         break;
      }
      case South: {
         if (_position + SIZE >= SIZE * SIZE) return false;
         break;
      }
      case West: {
         if (_position % SIZE == 0) return false;
         break;
      }
   }
   int old_pos = _position;
   _position = _position + direction;
   print_entry(old_pos);
   print_entry(_position);
   refresh();
   return true;
}

void Sudoku9Interactive::clear_all() {
   for (unsigned int position = 0; position != SIZE * SIZE; ++position) {
      try_clear(position);
   }
}

void Sudoku9Interactive::display_solution() {
   _position = SIZE * SIZE * SIZE;
   for (unsigned int y = 0; y != SIZE; ++y) {
      for (unsigned int x = 0; x != SIZE; ++x) {
         GridEntry &grid_entry = _grid[y][x];
         if (grid_entry.status != Fixed) {
            if (grid_entry.number == _solution[y][x]) {
               grid_entry.status = Legal;
            } else {
               grid_entry.status = Illegal;
               grid_entry.number = _solution[y][x];
            }
         }
      }
   }
   print_all();
}

bool Sudoku9Interactive::try_clear(int position) {
   GridEntry &grid_entry = _grid[position / SIZE][position % SIZE];
   if (grid_entry.status == Empty or grid_entry.status == Fixed) {
      return false;
   }
   grid_entry.number = 0;
   grid_entry.status = Empty;
   check_legality();
   print_entry(position);
   refresh();
   return true;
}

bool Sudoku9Interactive::try_record(int position, int number) {
   GridEntry &grid_entry = _grid[position / SIZE][position % SIZE];
   if (grid_entry.status == Fixed) {
      return false;
   }
   grid_entry.number = number;
   grid_entry.status = Legal;
   check_legality();
   bool won = true;
   for (unsigned int idx = 0; idx != SIZE * SIZE; ++idx) {
      if (_grid[idx / SIZE][idx % SIZE].status == Empty or _grid[idx / SIZE][idx % SIZE].status == Illegal) {
         won = false;
         break;
      }
   }
   if (won) {
      _position = SIZE * SIZE * SIZE;  // out of the grid
      attron(A_BOLD);
      attron(A_BLINK);
      mvprintw(start_pos_y - 2, start_pos_x, "You won!");
      attroff(A_BLINK);
      attroff(A_BOLD);
   }
   print_entry(position);
   refresh();
   return true;
}

void Sudoku9Interactive::print_all() const {
   clear();
   for (unsigned int idx = 0; idx != SIZE * SIZE; ++idx) {
      print_entry(idx);
   }
   mvprintw(3 * SIZE + start_pos_y + 1, start_pos_x, "Press '0' to delete a number");
   mvprintw(3 * SIZE + start_pos_y + 2, start_pos_x, "Press 'q' to quit the game");
   mvprintw(3 * SIZE + start_pos_y + 3, start_pos_x, "Press 'c' to clear the grid");
   mvprintw(3 * SIZE + start_pos_y + 4, start_pos_x, "Press 'e' to start a new easy game");
   mvprintw(3 * SIZE + start_pos_y + 5, start_pos_x, "Press 'i' to start a new intermediate game");
   mvprintw(3 * SIZE + start_pos_y + 6, start_pos_x, "Press 'h' to start a new hard game");
   mvprintw(3 * SIZE + start_pos_y + 7, start_pos_x, "Press 'n' to start a new nightmare game");
   refresh();
}

void Sudoku9Interactive::print_entry(unsigned int idx) const {
   unsigned int y = idx / SIZE, x = idx % SIZE;
   const GridEntry &grid_entry = _grid[y][x];
   int bg_color = 0, fg_color = 0;
   if (idx != _position) {
      bg_color = (y / 3) % 2 == (x / 3) % 2 ? 1 : 2;
   }
   if (grid_entry.status == Legal) {
      fg_color = 1;
   } else if (grid_entry.status == Illegal) {
      fg_color = 2;
   }
   int color = 3 * bg_color + fg_color + 1;
   attron(COLOR_PAIR(color));
   attron(A_BOLD);
   mvprintw(3 * y + start_pos_y, 5 * x + start_pos_x, "     ");
   if (grid_entry.status == Empty) {
      mvprintw(3 * y + 1 + start_pos_y, 5 * x + start_pos_x, "     ");
   } else {
      mvprintw(3 * y + 1 + start_pos_y, 5 * x + start_pos_x, ("  " + std::to_string(grid_entry.number) + "  ").c_str());
   }
   mvprintw(3 * y + 2 + start_pos_y, 5 * x + start_pos_x, "     ");
   attroff(COLOR_PAIR(color));
   attroff(A_BOLD);
}

void Sudoku9Interactive::check_legality() {
   for (unsigned int y = 0; y != SIZE; ++y) {
      for (unsigned int x = 0; x != SIZE; ++x) {
         GridEntry &grid_entry = _grid[y][x];
         if (grid_entry.status == Empty or grid_entry.status == Fixed) continue;
         bool is_legal = is_legal_term(SIZE * y + x);
         if (is_legal and grid_entry.status == Illegal) {
            grid_entry.status = Legal;
            print_entry(SIZE * y + x);
         } else if (not is_legal and grid_entry.status == Legal) {
            grid_entry.status = Illegal;
            print_entry(SIZE * y + x);
         }
      }
   }
}

bool Sudoku9Interactive::is_legal_term(unsigned int idx) const {
   unsigned int y_to_check = idx / SIZE, x_to_check = idx % SIZE;
   const GridEntry &grid_entry = _grid[y_to_check][x_to_check];
   if (grid_entry.status == Empty or grid_entry.status == Fixed) {
      return true;
   }
   for (unsigned int y = 0; y != SIZE; ++y) {
      if (y == y_to_check) continue;
      if (_grid[y][x_to_check].number == grid_entry.number) {
         return false;
      }
   }
   for (unsigned int x = 0; x != SIZE; ++x) {
      if (x == x_to_check) continue;
      if (_grid[y_to_check][x].number == grid_entry.number) {
         return false;
      }
   }
   for (unsigned int y = 3 * (y_to_check / 3); y != 3 * (y_to_check / 3 + 1); ++y) {
      if (y == y_to_check) continue;
      for (unsigned int x = 3 * (x_to_check / 3); x != 3 * (x_to_check / 3 + 1); ++x) {
         if (x == x_to_check) continue;
         if (_grid[y][x].number == grid_entry.number) {
            return false;
         }
      }
   }
   return true;
}

void Sudoku9Interactive::start_colors() {
#ifndef INITIALIZE_CHARACTER_MAP
#define INITIALIZE_CHARACTER_MAP
   start_color();
   init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
   init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
   init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
   init_pair(WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
   init_pair(YELLOW_BLUE, COLOR_YELLOW, COLOR_BLUE);
   init_pair(RED_BLUE, COLOR_RED, COLOR_BLUE);
   init_pair(WHITE_GREEN, COLOR_WHITE, COLOR_GREEN);
   init_pair(YELLOW_GREEN, COLOR_YELLOW, COLOR_GREEN);
   init_pair(RED_GREEN, COLOR_RED, COLOR_GREEN);
#endif
}