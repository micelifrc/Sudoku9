//
// Created by mich on 19/07/19.
//

#include <cstdlib>
#include <iostream>
#include <ctime>
#include "Rand.h"

void Rand::initialize_seed() {
   std::srand(std::time(nullptr));
}

int Rand::create_random_number(int lower_limit, int upper_limit) {
   int num_possible_choices = upper_limit - lower_limit;
   if(num_possible_choices <= 0) {
      throw std::logic_error("Cannot create random number from empty interval");
   }
   unsigned int num_rand_choices = (unsigned int)(RAND_MAX) + 1;
   unsigned int limit = num_possible_choices * (num_rand_choices / num_possible_choices);
   int random_number = 0;
   while (true) {
      random_number = std::rand();
      if (random_number < limit) {
         break;
      }
   }
   return random_number % num_possible_choices + lower_limit;
}

int Rand::create_random_number(int upper_limit) {
   return create_random_number(0, upper_limit);
}

void Rand::write_random_array(std::array<int, 9> &random_array) {
   std::vector<int> residual_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
   int random_number = create_random_number(9*8*7*6*5*4*3*2*1);
   for (unsigned int iter = 9; iter != 0; --iter) {
      std::swap(residual_vec[random_number % iter], residual_vec.back());
      random_array[9 - iter] = residual_vec.back();
      residual_vec.pop_back();
      random_number /= iter;
   }
}