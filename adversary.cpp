/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include "adversary.hpp"

// Constructor
Adversary::Adversary (Challenger &C) : C(C) {
  //this->C = C;
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
    if (C.query(guess, rw)) break;
  }

  return queries;
}

// Only guess things that are consistent with 
// responses to previous guesses
int Adversary::efficient () {

  std::vector<Guess> valid_guesses;
  Guess g;
  int c = 0;
  int rw[2], rwcheck[2];

  std::default_random_engine gen(clock());
  std::uniform_int_distribution<int> distribution(this->range[0],this->range[1]);

  // Populate the allowed vectors
  for (int i=1; i<10; i++) {
    for (int j=1; j<10; j++) {
      for (int k=1; k<10; k++) {
        for (int l=1; l<10; l++) {
          valid_guesses.push_back(Guess(i,j,k,l));
        }
      }
    }
  }

  // Set up a random initial guess. For now we
  // posit that n,n,m,m is a good guess.
  std::vector<int> invalid_numbers;
  bool has_started_elim = false;

  while (true) {

    // Get inital guess(es), if necessary
    if (!has_started_elim) {
      int a,b;
      while (true) {
        a = distribution(gen);
        if (std::find(invalid_numbers.begin(), invalid_numbers.end(), a) == invalid_numbers.end()) {
          invalid_numbers.push_back(a);
          break;
        }
      }
      if (invalid_numbers.size() == 9) {
        g.reset(a,a,a,a);
      } else {
        while (true) {
          b = distribution(gen);
          if (std::find(invalid_numbers.begin(), invalid_numbers.end(), b) == invalid_numbers.end()) {
            invalid_numbers.push_back(b);
            break;
          }
        }
        g.reset(a,a,b,b);  
      }
    }

    // Do the standard "pick from valid"
    else {
      g.reset(valid_guesses.at(0).a, valid_guesses.at(0).b, valid_guesses.at(0).c, valid_guesses.at(0).d);
    }

    // Check guess
    c++;
    C.query(g, rw);
    if (rw[0] != 0 || rw[1] != 0) {
      has_started_elim = true;
    }

    if (rw[0] == 4) {
      return c;
    }

    // Eliminate 
    int i=0;
    while (i < valid_guesses.size()) {
      C.get_correlation(g,valid_guesses.at(i),rwcheck);
      i++;
      if (rwcheck[0] != rw[0] || rwcheck[1] != rw[1]) {
        i--;
        valid_guesses.erase(valid_guesses.begin() + i);
      }
    }
  }
}

// Only responds with guesses that are consistent
// with previous responses and that minimize the
// standard deviation of the resultant sets.
int Adversary::smart () {
  // That's how good we are. Always return 1.
  return 1;
}


