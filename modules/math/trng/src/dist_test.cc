// ---------------------------------------------------------------------
// Time-stamp: <Monday, 09.12.2002, 22:48:47; edited by bauke>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// tests jumping, splitting, storing and loading generator's status
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

#include <iostream>
#include <vector>
#include <trng.h>
#include "fibonacci.h"

using namespace TRNG;
using namespace std;


template<class RNG_type>
void plausibility_main(RNG<RNG_type> &ra, RNG<RNG_type> &rb) {
  long i, j, k, n;
  bool err, genok;
  vector<long> status;
  RNG_type rc, rd;
  genok=true;
  cout << "testing " << ra.name() << endl;
  // test jump
  n=1l;
  ra.reset();
  rb.reset();
  ra.seed();
  rb.seed();
  for (i=0l; i<20l; ++i) {
    ra.jump2(i);
    for (j=0l; j<n; ++j)
      rb.rand();
    err=false;
    if (ra.rand()!=rb.rand())
      err=true;
    else
      if (ra.rand()!=rb.rand())
	err=true;
      else
	if (ra.rand()!=rb.rand())
	  err=true;
    if (err) {
      cout << ra.name() << ": error in Trng::jump" << endl;
      genok=false;
      break;
    }
    n<<=1;
  }
  // test split
  for (i=2l; i<=2l; ++i) {
    for (j=0l; j<i; ++j) {
      ra.reset();
      rb.reset();
      ra.seed();
      rb.seed();
      for (k=0l; k<j; ++k)
	ra.rand();
      rb.split(i, j);
      err=false;
      if (ra.rand()!=rb.rand())
	err=true;
      else {
	for (k=0l; k<i-1l; ++k)
	  ra.rand();
	if (ra.rand()!=rb.rand())
	  err=true;
	else {
	  for (k=0l; k<i-1l; ++k)
	    ra.rand();
	  if (ra.rand()!=rb.rand())
	    err=true;
	}
      }
      if (err) {
	cout << ra.name() << ": error in Trng::split" << endl;
	genok=false;
	break;
      }
    }
  }
  // test savestatus / loadstatus 
  ra.reset();
  rb.reset();
  ra.seed();
  rb.seed();
  for (i=0l; i<271828l; ++i) {
    ra.rand();
    rb.rand();
  }
  ra.save_status(status);
  for (i=0l; i<314159l; ++i)
    ra.rand();
  ra.load_status(status);
  err=false;
  if (ra.rand()!=rb.rand())
    err=true;
  else
    if (ra.rand()!=rb.rand())
      err=true;
    else
      if (ra.rand()!=rb.rand())
	err=true;
  if (err) {
    cout << ra.name() 
	 << ": error in Trng::savestatus or Trng::loadstatus" << endl;
    genok=false;
  }
  // copy
  ra.reset();
  ra.seed();
  rc=ra;
  rd=ra;
  rc.split(2l, 0l);
  rd.split(2l, 1l);
  err=false;
  for (i=0l; i<16l; ++i)
    if (i%2l==0l) {
      if (ra.rand()!=rc.rand()) {
	err=true;
	break;
      }
    } else {
      if (ra.rand()!=rd.rand()) {
	err=true;
	break;
      }
    }
  if (err) {
    cout << ra.name()
	 << ": error in Trng::copy" << endl; 
    genok=false;
  }
  //
  cout << ra.name() << ": test ";
  if (genok)
    cout << "passed" << endl;
  else
    cout << "failed" << endl;
}

int main(void) {
  try {
    cout << TRNG::TRNG::version() << endl;
    ParkMiller r1a, r1b;
    LCG32 r2a, r2b;
    LCG64 r3a, r3b;
    MRG2 r4a, r4b;
    MRG3 r5a, r5b;
    MRG4 r6a, r6b;
    CLCG2 r7a, r7b;
    CLCG3 r8a, r8b;
    CLCG4 r9a, r9b;
    EINV r10a, r10b;
    EINVLCG64 r11a, r11b;
    Fibonacci<103, 250> r12a, r12b;
    plausibility_main(r1a, r1b);
    plausibility_main(r2a, r2b);
    plausibility_main(r3a, r3b);
    plausibility_main(r4a, r4b);
    plausibility_main(r5a, r5b);
    plausibility_main(r6a, r6b);
    plausibility_main(r7a, r7b);
    plausibility_main(r8a, r8b);
    plausibility_main(r9a, r9b);
    plausibility_main(r10a, r10b);
    plausibility_main(r11a, r11b);
    plausibility_main(r12a, r12b);
  } 
  catch (exception &err) {
    cerr << err.what() << endl;
  }
  catch (...) {
    cerr << "something went wrong" << endl;
  }
  return(EXIT_SUCCESS);
}


