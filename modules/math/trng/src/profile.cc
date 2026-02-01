// ---------------------------------------------------------------------
// Time-stamp: <Sonntag, 02.03.2003, 15:14:35; edited by bauke>
// 
// Tina's random number generators TRNG
//
// profiling Tina's random number generators
//
// Copyright (C) 2001, 2002 Heiko Bauke
//
// heiko.bauke@physik.uni-magdeburg.de
//
// TRNG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation. This program
// is distributed WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// ---------------------------------------------------------------------


#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#include <trng.h>

using namespace TRNG;
using namespace std;


template<class T>
void profl(TRNG::RNG<T> &r, long j) {
  clock_t my_clock;
  long i;
  cout.setf(ios::fixed);
  long sum=0;
  my_clock=clock();
  for (i=0l; i<j; ++i)
    sum+=r.rand();
  my_clock=clock()-my_clock;
  if (sum==0)  // do something with sum
    sum++;
#ifdef LaTeX
  cout << " & ";
#else
  cout << '\t';
#endif
  cout << setfill(' ') << setprecision(2) 
       << static_cast<double>(my_clock)/static_cast<double>(CLOCKS_PER_SEC);
  my_clock=clock();
}


template<class T>
void profd(RNG<T> &r, long j) {
  clock_t my_clock;
  long i;
  cout.setf(ios::fixed);
  double sum=0.0;
  my_clock=clock();
  for (i=0l; i<j; ++i)
    sum+=r.uniform();
  my_clock=clock()-my_clock;
  if (sum==0)  // do something with sum
    sum++;
#ifdef LaTeX
  cout << " & ";
#else
  cout << '\t';
#endif
  cout << setfill(' ') << setprecision(2) 
       << static_cast<double>(my_clock)/static_cast<double>(CLOCKS_PER_SEC);
}


template<class T>
void prof_main(RNG<T> &r,long from, long to) {
  long i;
  std::string str(r.name());
  str+=string( (16-str.length()>0) ? 16-str.length() : 0, ' ');
  cout << str;
#ifdef LaTeX
  cout << " & long";
#else
  cout << "long";
#endif
  for (i=(1l<<from); i<=(1l<<to); i*=4l)
    profl(r, i);
#ifdef LaTeX
  cout << "\\\\" << endl;
#else
  cout << endl;
#endif
#ifdef LaTeX
   cout << " & double ";
#else
   cout << "\t\tdouble ";
#endif
   for (i=(1l<<from); i<=(1l<<to); i*=4l)
     profd(r, i);
#ifdef LaTeX
  cout << "\\\\" << endl;
#else
  cout << endl;
#endif
}


int main(void) {
  const long from=22;
  const long to=26;
  long i;
#ifdef LaTeX
  cout << " & ";
#else
  cout << "\t\t";
#endif
  for (i=from; i<=to; i+=2l)
#ifdef LaTeX
    cout << " & $2^{" << i << "}$";
#else
    cout << "\t2^" << i;
#endif
#ifdef LaTeX
  cout << "\\\\" << endl;
#else
  cout << endl;
#endif
  try {

#define FOR_ALL_GENERATORS \
RNG r; prof_main(r, from, to);
#include "for_all_generators.h"

  } 
  catch (exception &err) {
    cerr << err.what() << endl;
  }
  catch (...) {
    cerr << "something went wrong" << endl;
  }
  return EXIT_SUCCESS;
}
