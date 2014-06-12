/*  
 *  Copyright (C)  2014  George Wong
 *  GNU General Public License
 *
 */

#include <vector>
#include <random>

class Challenger {

  public:
    Challenger (int t);
    ~Challenger ();

    bool reset ();
    
    bool change_type (int t);
    int get_type ();

    void get_truth (std::vector <int> *vec);

    bool query (std::vector<int> *guess, int *rw);
    bool get_correlation (std::vector<int> *a, std::vector<int> *b, int *rw);

  private:
    int type;
    int range[2];
    std::vector<int> my_numbers;
};

