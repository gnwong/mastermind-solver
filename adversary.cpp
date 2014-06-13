/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include "adversary.hpp"

// Constructor
Adversary::Adversary (Challenger *C) {
  this->C = C;
  this->verbosity = 0;
  this->range[0] = 1;
  this->range[1] = 9;
}

// Destroys necessary components
Adversary::~Adversary () {

}

// Randomly search with replacement
int Adversary::naive () {

  int queries = 0;
  int rw[2];

  std::default_random_engine gen(clock());
  std::uniform_int_distribution<int> distribution(this->range[0],this->range[1]);

  Guess guess;

  while (true) {
    queries++;

    guess.reset(distribution(gen),distribution(gen),distribution(gen),distribution(gen));

    if (this->verbosity > 2) 
      std::cout << "guess: " << guess.a << " " << guess.b 
      << " " << guess.c << " " << guess.d << std::endl;
    if (C->query(&guess, rw)) break;
  }

  return queries;
}

// Only guess things that are consistent with 
// responses to previous guesses
int Adversary::efficient () {

  int c = 0;

  // Populate the allowed vectors
  for (int i=1; i<10; i++) {
    for (int j=1; j<10; j++) {
      for (int k=1; k<10; k++) {
        for (int l=1; l<10; l++) {
          // TODO 
          printf("%d %d %d %d\n", i,j,k,l);
        }
      }
    }
  }


  return 0;
}

// TODO
int Adversary::smart () {

  return 0;
}

