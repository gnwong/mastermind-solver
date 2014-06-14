/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include "challenger.hpp"

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

  this->truth.reset(distribution(gen),distribution(gen),distribution(gen),distribution(gen));

  return true;
}

// Changes the type of PRG used but does NOT
// update the list.
bool Challenger::change_type (int t) {
  this->type = t;
  return true;
}

// Returns the current values stored in the challenger.
void Challenger::get_truth (Guess &guess) {
  guess.reset(this->truth.a,this->truth.b,this->truth.c,this->truth.d);
}

// Returns the challenger type.
int Challenger::get_type () {
  return this->type;
}

// Returns the "red" and "white" numbers for a given guess.
bool Challenger::query (Guess *guess, int *rw) {
  return this->get_correlation (guess, &this->truth, rw);
}

// By convention, we think of the response in terms of "red" and "white", 
// where red is the number of objects in the correct position and white is
// the number of correct objects (in incorrect positions). In our function,
// *a is a pointer to the guess and *b points to the truth.
bool Challenger::get_correlation (Guess *guess, Guess *truth, int *rw) {
  bool aused[4] = {false, false, false, false};
  bool bused[4] = {false, false, false, false};

  rw[0] = 0;
  rw[1] = 0;

  for (int i=0; i<4; i++) {
    if (guess->at(i) == truth->at(i)) {
      rw[0] += 1;
      aused[i] = true;
      bused[i] = true;
    }
  }

  for (int i=0; i<4; i++) {
    if (aused[i]) continue;
    for (int j=0; j<4; j++) {
      if (bused[j]) continue;
      if (guess->at(i) == truth->at(j)) {
        rw[1] += 1;
        bused[j] = true;
      }
    }
  }

  if (rw[0] == 4) return true;
  return false;
}



