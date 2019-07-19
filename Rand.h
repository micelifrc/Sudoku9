#ifndef SUDOKU_RAND_H
#define SUDOKU_RAND_H

#include <vector>
#include <array>

namespace Rand {
   void initialize_seed();

   // creates random int in [lower_limit, upper_limit)
   int create_random_number(int lower_limit, int upper_limit);

   // creates random int in [0, upper_limit)
   int create_random_number(int upper_limit);

   // create a vector containing all the elements of [lower_limit, upper_limit) in random order
   std::vector<int> create_shuffled_vector(int lower_limit, int upper_limit);

   // create a vector containing all the elements of [0, upper_limit) in random order
   std::vector<int> create_shuffled_vector(int upper_limit);

   // override @p random_array with entries in [1,9] all different, in random order
   void override_random_array(std::array<int, 9> &random_array);
};


#endif //SUDOKU_RAND_H
