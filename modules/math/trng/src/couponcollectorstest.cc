// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:22:26; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// coupon collector's test for random number generators
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
void couponcollector(RNG_type &r, long runs, long n, long d, long t) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n: collections per test
  // d: collect d coupons
  // t: stop collecting after t steps
  vector<long> occurs;
  vector<double> observ, prob;
  long i, s, q, p, y, num_fail;
  double fac, Chi2, Chi2P;
  cout.setf(ios::fixed);
  observ.resize(t-d+1);
  prob.resize(t-d+1);
  occurs.resize(d);
  num_fail=0l;
  fac=static_cast<double>(d);
  for (i=2l; i<d; ++i)
    fac*=static_cast<double>(i);
  for (i=d; i<t; ++i)
    prob[i-d]=fac/pow(static_cast<double>(d), static_cast<double>(i))*
      Stirling_num2(i-1ll, d-1ll);
  prob[t-d]=1.0-fac/pow(static_cast<double>(d), static_cast<double>(t-1l))*
    Stirling_num2(t-1ll, d);
  while (runs>0l) {
    for (i=0l; i<t-d+1l; ++i)
      observ[i]=0.0;
    for (s=0l; s<n; ++s) {
      q=p=0l;
      for (i=0l; i<d; ++i)
	occurs[i]=0l;
      do {
	do {
	  ++p;
	  y=static_cast<long>(r.uniform()*d);
      } while (occurs[y]!=0l);
	occurs[y]=1l;
	++q;
      } while (q<d);
      ++observ[(p>=t ? t : p)-d];
    }
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, t-d); 
    }
    catch (exception &err) {
      cerr << err.what() << endl;
    }
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
void couponcollector_main(RNG_type &r, long runs, long sub_max, long n, long d, long t) {
  long j;
#ifdef LaTeX
  cout << "\\multiciol{" << runs+1l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif
  for (j=1l; j<=sub_max; ++j) {
#ifdef LaTeX
    cout << "\\multiciol{" << runs+1l <<"}{@{}l@{}}{Splitting level " << j 
	 << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << endl;
#endif
    r.reset();
    r.split(j, 0l);
    couponcollector(r, runs, n, d, t);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=6) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Coupon collector's test\n";
    cerr << 
      "-----------------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max n d t\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; n=2500; d=7; t=35\n\n";
    cerr << 
      "The Programm counts the lengths of a sequence of random numbers\n";
    cerr << 
      "0, 1, ..., d-1 requied to get a complete set of numbers 0 to d-1\n";
    cerr << 
      "and does a Chi^2-test. Lengths>=t (t>d) are lumped together for Chi^2-test.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, n, d, t;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      n=atol(argv[3]);
      d=atol(argv[4]);
      t=atol(argv[5]);
      if (!(t>d)) {
	cerr << "bad parameter, t>d\n";
	return(EXIT_FAILURE);
      }
#ifdef LaTeX
      cout << n << " Sammlungen je Test\\\\" << endl;
      cout << d << " zu sammelnde Objekte\\\\" << endl;
      cout << t << " als obere Schrake f\\\"ur Sammeldauer\\\\" << endl;
#else
      cout << n << " collections per test" << endl;
      cout << d << " objects to collect" << endl;
      cout << t << " upper bound" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; couponcollector_main(r, runs, sub_max, n, d, t);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return(EXIT_SUCCESS);
}
