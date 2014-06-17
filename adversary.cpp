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

  // Always reset the Challenger
  C.reset();

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

  // Always reset the Challenger
  C.reset();

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

  // Always reset the Challenger
  C.reset();

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

  while (true) {

    // Pick from valid using optimization
    this->optimize(g, valid_guesses);

    // Check guess
    c++;
    C.query(g, rw);

    // Win if we've won
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

// Updates the guess object with the optimal guess in "valid_guesses", with
// the assumption that each of the entries of valid_guesses is equally likely.
void Adversary::optimize (Guess &g, std::vector<Guess> &valid_guesses) {
  this->optimize(g, valid_guesses, 4);
}

// Allows us to define different optimization techniques.
void Adversary::optimize (Guess &g, std::vector<Guess> &valid_guesses, int type) {

  // Different types:
  // -1 ?
  // 1  : min of maxima  ~  1500
  // 2  : min of minima  ~  1
  // 3  : min of stddev  ~  489.755
  // 4  : min of minstd  ~  489.755

  int rw[2];
  std::vector< std::vector<int> > rws;
  std::vector<int> rws_count;
  std::vector<double> characteristic;
  double returnValue = -1;

  // We can do some pre-guess optimization here. Depending on the type of
  // optimization we're using, we can effectively store the "best guesses"
  // for the first run (i.e. with valid_guesse.size() == range^n)...
  if (valid_guesses.size() == 6561) {
    if (type == 1) {
      returnValue = 1500;
    } else if (type == 2) {
      returnValue = 1;
    } else if (type == 3) {
      returnValue = 489.755;
    } else if (type == 4) {
      returnValue = 489.755;
    }
  }
  
  // Assume each element of valid_guesses has equal probability of being
  // the truth. How does making each choice split the rws?
  for (int i=0; i<valid_guesses.size(); i++) {

    rws.clear();
    rws_count.clear();

    for (int j=0; j<valid_guesses.size(); j++) {
      C.get_correlation(valid_guesses.at(i),valid_guesses.at(j),rw);
      std::vector<int> w(rw, rw + 2);

      if (std::find(rws.begin(), rws.end(), w) == rws.end()) {
        rws.push_back(w);
        rws_count.push_back(1);
      } else {
        rws_count[std::find(rws.begin(), rws.end(), w) - rws.begin()]++;
      }
    }

    if (type == 1) {
      characteristic.push_back(*std::max_element(rws_count.begin(), rws_count.end()));
    } else if (type == 2) {
      characteristic.push_back(*std::min_element(rws_count.begin(), rws_count.end()));
    } else if (type == 3) {
      double mean, sum=0;
      mean = std::accumulate(rws_count.begin(), rws_count.end(), 0) / rws_count.size();
      for (int k=0; k<rws_count.size(); k++) {
        sum += (rws_count.at(k) - mean)*(rws_count.at(k) - mean);
      }
      characteristic.push_back(sqrt(sum/rws_count.size()));
    } else if (type == 4) {
      int n = 4; // This number changes based on our guess format
      int count = n*(n+3)/2;
      double mean, sum=0;
      mean = std::accumulate(rws_count.begin(), rws_count.end(), 0) / count;
      for (int k=0; k<rws_count.size(); k++) {
        sum += (rws_count.at(k) - mean)*(rws_count.at(k) - mean);
      }
      sum += (mean*mean)*(count - rws_count.size());
      characteristic.push_back(sqrt(sum/count));
    } else {
      characteristic.push_back(rws_count.at(0));
    }

    if (returnValue != -1) {
      if (fabs(characteristic.back() - returnValue) < 0.01) {
        i = valid_guesses.size();
      }
    }
  }

  // Minimize the characteristic
  double min = characteristic.at(0);
  for (int i=1; i<characteristic.size(); i++) {
    if (min > characteristic.at(i)) min = characteristic.at(i);
  }

  // Find an element with corresponding characteristic
  int gidx = std::find(characteristic.begin(), characteristic.end(), min) - characteristic.begin();
  
  // This is debug code that is intentionally left in. Running it allows the 
  // programmer to determine the list of all valid "first guesses" for a defined
  // characteristic. Modify as desired.
  /*
  if (valid_guesses.size() == 6561) {
    auto it = characteristic.begin();
    while (it < characteristic.end()) {
      it = std::find(it, characteristic.end(), min);
      if (it == characteristic.end()) break;
      // valid_guesses.at(it - characteristic.begin()).print();
      std::advance(it,1);
    }
    std::cout << "Minimized characteristic takes value: " << min << std::endl;
  }
  // */

  // Update the guess
  g.a = valid_guesses.at(gidx).a;
  g.b = valid_guesses.at(gidx).b;
  g.c = valid_guesses.at(gidx).c;
  g.d = valid_guesses.at(gidx).d;
}


