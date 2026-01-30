#include <cstdlib>
#include <iostream>
#include <trng.h>

using namespace std;
using namespace TRNG;

template<class RNG_type>
void foo(RNG_type &R) {
  cout << R.rand() << " is a random number from generator " 
       << R.name() << endl;
}

int main(void) {
  ParkMiller R1;
  LCG64 R2;
  foo(R1);
  foo(R2);  
  return EXIT_SUCCESS;
}
