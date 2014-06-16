/*
 *  `mastermind-solver'
 *    ..
 *
 *  Copyright (C)  2014  George Wong
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see http://www.gnu.org/licenses/.
 *
 */


#include <iostream>

#ifndef CHALLENGER
 #define CHALLENGER
 #include "challenger.hpp"
#endif
#ifndef ADVERSARY
 #define ADVERSARY
 #include "adversary.hpp"
#endif
#ifndef GUESS
 #define GUESS
 #include "guess.hpp"
#endif


void human (Challenger &C);
void test1 (Challenger &C);

int main (int argc, char **argv) {

  // In the future, we should probably implement one global PRG to 
  // ensure that we don't double seed
  //std::default_random_engine PRG(clock());

  Challenger C = Challenger(0);
  Adversary A  = Adversary(C);

  // Some tests below
  //test1(C);

  // Start a human game as below
  //human(C);

  // Call the naive solver as below
  //std::cout << "Naive guess count: " << A.naive() << std::endl;
  
  // Run an efficient test
  //std::cout << "Efficient guessing scheme count: " << A.efficient() << std::endl;
  
  // Run the smartest test we know how
  std::cout << "Smart guessing scheme count: " << A.smart() << std::endl;

  return 0;
}


// Runs a game with a human guesser
void human (Challenger &C) {

  int a,b,c,d,rw[2];
  bool solved = false;
  Guess guess;
  C.reset();

  std::cout << "Guess: a b c d\n";
  
  while (!solved) {
    std::cout << "Guess: ";
    scanf("%d%d%d%d", &a, &b, &c, &d);
    guess.reset(a,b,c,d);
    solved = C.query(guess,rw);
    std::cout << rw[0] << " " << rw[1] << std::endl;
  }
}

// Runs some simple tests to see if the Challenger has "get_correlation"
// implemented correctly.
void test1 (Challenger &C) {
  Guess guess;
  Guess truth;
  int rw[2];

  // 2 2
  guess.reset(1,2,3,4);
  truth.reset(1,2,4,3);
  C.get_correlation(guess,truth,rw);
  std::cout << rw[0] << "," << rw[1] << std::endl;

  // 2 0
  guess.reset(1,1,1,1);
  truth.reset(1,1,7,7);
  C.get_correlation(guess,truth,rw);
  std::cout << rw[0] << "," << rw[1] << std::endl;

  // 0 4
  guess.reset(1,2,3,4);
  truth.reset(4,3,2,1);
  C.get_correlation(guess,truth,rw);
  std::cout << rw[0] << "," << rw[1] << std::endl;

  // 1 2
  guess.reset(2,1,1,4);
  truth.reset(1,1,2,5);
  C.get_correlation(guess,truth,rw);
  std::cout << rw[0] << "," << rw[1] << std::endl;
}


