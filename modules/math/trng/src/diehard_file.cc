// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:36:16; edited by heiko>
// 
// Tina's random number generators TRNG
//
// diehard_file generates a binary input file for the diehard test
// by Marsaglia; http://stat.fsu.edu/pub/diehard/
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
#include <fstream>
#include <new>
#include <string>
#include <trng.h>

using namespace TRNG;
using namespace std;


template<class RNG_type>
void diehard(RNG_type &r) {
  long q, split;
  string name;
  cout << "splitting level (>0): ";
  cin >> split;
  r.split(split, 0l);
  cout << "file name: ";
  cin >> name;
  ofstream file;
  file.open(name.c_str(), ios::out|ios::binary);
  for (int i=0; i<2621440; ++i) {
    q=r.rand()*2l;
    file.write(reinterpret_cast<const char *>(&q), sizeof(q));
  }
  file.close();
}


int main(void) {
  char c;

#define FOR_ALL_GENERATORS \
RNG r; \
cout << "Gernerate file for " << r.name() << " (y/n) "; \
cin >> c; \
if (c=='y') diehard(r); 
#include "for_all_generators.h"

  return EXIT_SUCCESS;
}

