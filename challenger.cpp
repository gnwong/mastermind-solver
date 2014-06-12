/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include "Challenger.hpp"

// Takes "type" as arguement and generates
// a first set of random numbers.
Challenger::Challenger (int t) {
  this->type = t;
  this->range[0] = 1;
  this->range[1] = 9;
  this->reset();
}

// Destroys associated objects.
Challenger::~Challenger () {
}

// Generates a new randomized set.
bool Challenger::reset () {

  std::default_random_engine gen(time(NULL));
  std::uniform_int_distribution<int> distribution(this->range[0],this->range[1]);

  this->my_numbers.clear();
  this->my_numbers.push_back(distribution(gen));
  this->my_numbers.push_back(distribution(gen));
  this->my_numbers.push_back(distribution(gen));
  this->my_numbers.push_back(distribution(gen));

  return true;
}

// Changes the type of PRG used but does NOT
// update the list.
bool Challenger::change_type (int t) {
  this->type = t;
  return true;
}

// Returns the current values stored in the challenger.
void Challenger::get_truth (std::vector <int> *vec) {
  vec->clear();
  vec->push_back(this->my_numbers.at(0));
  vec->push_back(this->my_numbers.at(1));
  vec->push_back(this->my_numbers.at(2));
  vec->push_back(this->my_numbers.at(3));
}

// Returns the challenger type.
int Challenger::get_type () {
  return this->type;
}

// Returns the "red" and "white" numbers for a given guess.
bool Challenger::query (std::vector<int> *guess, int *rw) {
  return this->get_correlation (guess, &this->my_numbers, rw);
}

// By convention, we think of the response in terms of "red" and "white", 
// where red is the number of objects in the correct position and white is
// the number of correct objects (in incorrect positions). In our function,
// *a is a pointer to the guess and *b points to the truth.
bool Challenger::get_correlation (std::vector<int> *a, std::vector<int> *b, int *rw) {
  bool aused[4] = {false, false, false, false};
  bool bused[4] = {false, false, false, false};

  rw[0] = 0;
  rw[1] = 0;

  for (int i=0; i<4; i++) {
    if (a->at(i) == b->at(i)) {
      rw[0] += 1;
      aused[i] = true;
      bused[i] = true;
    }
  }

  for (int i=0; i<4; i++) {
    if (aused[i]) continue;
    for (int j=0; j<4; j++) {
      if (bused[j]) continue;
      if (a->at(i) == b->at(j)) {
        rw[1] += 1;
        bused[j] = true;
      }
    }
  }

  if (rw[0] == 4) return true;
  return false;
}



