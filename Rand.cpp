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
   if (num_possible_choices <= 0) {
      throw std::logic_error("Cannot create random number from empty interval");
   }
   unsigned int num_rand_choices = (unsigned int) (RAND_MAX) + 1;
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

std::vector<int> Rand::create_shuffled_vector(int lower_limit, int upper_limit) {
   int length = upper_limit - lower_limit;
   if (length <= 0) {
      throw std::logic_error("Cannot create shuffled vercot of negative length");
   }

   std::vector<int> shuffled_vector;
   shuffled_vector.reserve(length);
   for (unsigned int entry = lower_limit; entry != upper_limit; ++entry) {
      shuffled_vector.emplace_back(entry);
   }

   //shuffle
   int rand = 0;
   for (unsigned int pos = 1; pos <= length; ++pos) {
      std::swap(shuffled_vector[create_random_number(pos + 1)], shuffled_vector[pos]);
   }
   return shuffled_vector;
}

std::vector<int> Rand::create_shuffled_vector(int upper_limit) {
   return create_shuffled_vector(0, upper_limit);
}

void Rand::override_random_array(std::array<int, SIZE> &random_array) {
   std::vector<int> shuffled_vector = create_shuffled_vector(1, 10);
   for (int idx = 0; idx != 9; ++idx) {
      random_array[idx] = shuffled_vector[idx];
   }
}