/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include <iostream>

#ifndef CHALLENGER
 #define CHALLENGER
 #include "challenger.hpp"
#endif
#ifndef GUESS
 #define GUESS
 #include "guess.hpp"
#endif

class Adversary {

  public:
    Adversary (Challenger *C);
    ~Adversary ();

    int dumb ();
    int naive ();
    int efficient ();
    int smart ();

  private:
    Challenger *C;
    int range[2];
                      // The angry observer might protest this should be a preprocessor
    int verbosity;    // variable. I disagree, citing the fact it might be pleasant to
                      // specify at runtime.
};

