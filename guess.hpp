/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

struct Guess {

  int a;
  int b;
  int c;
  int d;

  Guess () {
    this->a = 0;
    this->b = 0;
    this->c = 0;
    this->d = 0;
  }
  Guess (int w, int x, int y, int z) {
    this->a = w;
    this->b = x;
    this->c = y;
    this->d = z;
  }

  int at (int n) {
    if (n==0) return this->a;
    if (n==1) return this->b;
    if (n==2) return this->c;
    return this->d;
  }

  void reset (int w, int x, int y, int z) {
    this->a = w;
    this->b = x;
    this->c = y;
    this->d = z;
  }

};