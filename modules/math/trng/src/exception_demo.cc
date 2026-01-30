#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <trng.h>

using namespace std;
using namespace TRNG;


int main(void) {
  ParkMiller R;

  cout << R.normal_dist() << endl;  
  try {
    R.jump2(16l);
    cout << "jumped forward" << endl;
    // you can't jump backwards
    R.jump2(-16l);
    cout << "jumped backward" << endl;
  }
  catch (exception &e) {
    cerr << "oops!! " << e.what() << endl;
  }
  catch (...) {
    cerr << "something else went wrong" << endl;
  }
  return EXIT_SUCCESS;
}
