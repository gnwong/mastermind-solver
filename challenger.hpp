/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include <vector>
#include <random>

#ifndef GUESS
 #define GUESS
 #include "guess.hpp"
#endif

class Challenger {

  public:
    Challenger (int t);
    ~Challenger ();

    bool reset ();

    bool change_type (int t);
    int get_type ();

    void get_truth (Guess &guess);

    bool query (Guess &guess, int rw[]);
    bool get_correlation (Guess &guess, Guess &truth, int rw[]);

  private:
    int type;
    int range[2];
    Guess truth;
};

