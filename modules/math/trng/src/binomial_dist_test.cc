// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:09:45; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// tests method  bool TRNG::RNG::binomial_dist(long, double)
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
#include <algorithm>
#include <trng.h>

using namespace TRNG;
using namespace std;

double p;
long tr;


template<class RNG_type>
void binomial_dist_test(RNG<RNG_type> &r, long runs, long n) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n: random numbers per test
  // num_classes: number of classes for the Chi-Square test
  vector<double> observ, prob;
  long i, num_fail;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  observ.resize(tr+1l, 0.0);
  prob.resize(tr+1l);
  for (i=0l; i<=tr; ++i)
    prob[i]=binomial_dist_pdf(i, tr, p);
  num_fail=0l;
  while (runs>0l) {
    for (i=0l; i<=tr; ++i)
      observ[i]=0.0;
    for (i=0l; i<n; ++i)
      ++observ[r.binomial_dist(tr, p)];
    Chi2=chi_square_test(prob, observ);
    Chi2P=chi_square_prob(Chi2, tr);
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
void binomial_dist_test_main(RNG<RNG_type> &r, long runs, long sub_max, 
			    long n) {
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
    binomial_dist_test(r, runs, n);
  }
}

int main(int argc, char *argv[]) {
  if (argc!=6) {
    cerr 
      << "Tina's Random Number Generator Library " << TRNG::version() << endl
      <<"(P) & (C) by Heiko Bauke, Magdeburg 2001, 2002" << endl 
      << endl
      << "testing method  bool TRNG::RNG::binomial_dist(long, double)" << endl
      << "synopsis:" << endl    
      << "$ " << argv[0] << " runs sub_max n p tr" << endl 
      << endl
      << "runs: how often the test is applied" << endl
      << "sub_max: how many substreams are tested" << endl
      << "n: number of random numbers per test" << endl
      << "p: probability" << endl 
      << "tr: trails" << endl 
      << endl
      << "just try" << endl
      << "$ " << argv[0] << " 8 128 16384 0.5 8" << endl;
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
      p=atof(argv[4]);
      if (p<=0.0 || p>1.0)
	throw error("parameter p out of range");
      tr=atol(argv[5]);
      if (tr<1)
	throw error("parameter t out of range");
#ifdef LaTeX
      cout << n << " W\\\"urfe je Test\\\\" << endl;
#else
      cout << n << " throws per test" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; binomial_dist_test_main(r, runs, sub_max, n);
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
