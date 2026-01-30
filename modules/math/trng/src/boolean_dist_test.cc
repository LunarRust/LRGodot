// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:17:47; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// tests methods  bool TRNG::RNG::boolean(void) and
// bool TRNG::RNG::boolean(double)
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
#include <cmath>
#include <iostream>
#include <iomanip>
#include <trng.h>

using namespace TRNG;
using namespace std;

template<class RNG_type>
void boolean_test(RNG<RNG_type> &r, long runs, long n, double p) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n: random numbers per test
  // p: probability
  vector<double> observ, prob;
  long i, num_fail;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  observ.resize(2, 0.0);
  prob.resize(2);
  prob[0]=p;
  prob[1]=1.0-p;
  num_fail=0l;
  while (runs>0l) {
    for (i=0l; i<2; ++i)
      observ[i]=0.0;
    if (p==0.5)
      for (i=0l; i<n; ++i)
	if (r.boolean())
	  ++observ[0];
	else
	  ++observ[1];
    else
      for (i=0l; i<n; ++i)
	if (r.boolean(p))
	  ++observ[0];
	else
	  ++observ[1];
    Chi2=chi_square_test(prob, observ);
    Chi2P=chi_square_prob(Chi2, 1l);
    if (Chi2P<0.01 || 0.99<Chi2P) {
#ifdef LaTeX
      cout << "\\textbf{";
#endif
      ++num_fail;
    }
    cout << setfill(' ') << setprecision(3) << Chi2P;
#ifdef LaTeX
    if (Chi2P<0.01 || 0.99<Chi2P)
      cout << "}";
    cout << " & ";
#else
    cout << '\t';
#endif
    --runs;
  }
  if (num_fail>2l)
#ifdef LaTeX
    cout << "\\textbf{durchgefallen}";
#else
    cout << "failed";
#endif
  else
    if (num_fail>1l) 
#ifdef LaTeX
      cout << "\\textbf{suspekt}";
#else
      cout << "suspect";
#endif
    else
#ifdef LaTeX
      cout << "o.\\,k.";
#else
      cout << "o.k.";
#endif
#ifdef LaTeX
  cout << "\\\\" << endl;
#else
  cout << endl;
#endif
}

template<class RNG_type>
void boolean_test_main(RNG<RNG_type> &r, long runs, long sub_max, 
		       long n, double p) {
  long j;
#ifdef LaTeX
  cout << "\\multicol{" << runs+1l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif
  for (j=1l; j<=sub_max; ++j) {
#ifdef LaTeX
    cout << "\\multicol{" << runs+1l <<"}{@{}l@{}}{Splitting level " << j 
	 << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << endl;
#endif
    r.reset();
    r.split(j, 0l);
    boolean_test(r, runs, n, p);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=5) {
    cerr 
      << "Tina's Random Number Generator Library " << TRNG::version() << endl
      << "(P) & (C) by Heiko Bauke, Magdeburg 2001, 2002" << endl 
      << endl
      << "esting methods  bool TRNG::RNG::boolean(void)" << endl
      << "and  bool TRNG::RNG::boolean(double)" << endl 
      << endl
      << "synopsis:" << endl
      << "$ " << argv[0] << " runs sub_max n prob" << endl
      << endl
      << "runs: how often the test is applied" << endl
      << "sub_max: how many substreams are tested" << endl
      << "n: number of random numbers per test" << endl
      << "prob: probability; if prob=0.5 boolean(void) is tested else boolean(double)" << endl 
      << endl
      << "just try" << endl
      << "$ " << argv[0] << " 8 128 4096 0.125" << endl;
    return EXIT_FAILURE;
  } else {
    try {
      long runs=atol(argv[1]);
      if (runs<1l)
	throw error("runs out of range");
      long sub_max=atol(argv[2]);
      if (sub_max<1l)
	throw error("sub_max out of range");
      long n=atol(argv[3]);
      if (n<1l)
	throw error("n out of range");
      double p=atof(argv[4]);
      if (!(0.0<p && p<1.0))
	throw error("probability out of range");      
#ifdef LaTeX
      cout << n << " W\\\"urfe je Test\\\\" << endl
	   << "Wahrscheinlichkeit " << p << "\\\\" << endl;
#else
      cout << n << " throws per test" << endl
	   << "probability " << p << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; boolean_test_main(r, runs, sub_max, n, p);
#include "for_all_generators.h"

    }
    catch (exception &err) {
      cerr << err.what() << endl;
    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
