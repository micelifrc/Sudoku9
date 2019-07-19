#ifndef SUDOKU_RAND_H
#define SUDOKU_RAND_H

#include <vector>
#include <array>

namespace Rand {
   void initialize_seed();

   int create_random_number(int lower_limit, int upper_limit);  // creates random int in [lower_limit, upper_limit)

   int create_random_number(int upper_limit);  // creates random int in [0, upper_limit)

   void write_random_array(std::array<int, 9> &random_array);
};


#endif //SUDOKU_RAND_H
