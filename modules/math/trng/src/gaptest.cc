// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:46:38; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// gap test for random number generators
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
#include <climits>
#include <iostream>
#include <iomanip>
#include <trng.h>

using namespace TRNG;
using namespace std;


template<class RNG_type>
void gap_test(RNG_type &r, long runs, double a, double b, long t, long n) {
  // r: random number generator
  // runs: number of gap tests
  // a: lower bound
  // b: upper bound for gap test
  // t: maximal gap length
  // n: number of gaps
  vector<double> observ, prob;
  long j, k, s, num_fail;
  double Chi2, Chi2P, p, u;
  cout.setf(ios::fixed);
  observ.resize(t+1, 0.0);
  num_fail=0;
  prob.resize(t+1, 1.0);
  p=b-a;
  for (j=1l; j<=t; ++j) {
    prob[j]=(1-p)*prob[j-1];
    prob[j-1]*=p;
  }
  while (runs>0l) {
    for (j=0; j<=t; ++j)
      observ[j]=0.0;
    j=-1l;
    s=0l;
    do {
      k=-1l;
      do {
	++k;
	++j;
	if (j==LONG_MAX) {
	  cerr << "realy bad generator" << endl;
	  exit(EXIT_FAILURE);
	}
	u=r.uniform();
      } while (!(a<=u && u<b));
      if (k>=t)
	++observ[t];
      else
	++observ[k];
      s++;
    } while (s<n);
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, t);
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
void gap_test_main(RNG_type &r, long runs, long sub_max, 
		   double a, double b, long t, long n) {
  long j;
#ifdef LaTeX
  cout << "\\multicol{" << runs+1l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif
  for (j=1l; j<=sub_max; ++j) {
#ifdef LaTeX
    cout << "\\multicol{" << runs+2l <<"}{@{}l@{}}{Splitting level " << j 
	 << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << endl;
#endif
    r.reset();
    r.split(j, 0l);
    gap_test(r, runs, a, b, t, n);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=7) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Gap test\n";
    cerr << 
      "--------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max n a b t n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; a=0; b=0.5 t=8; n=5000\n";
    cerr <<
      "0<=a<b<1; n>>t\n\n";
    cerr << 
      "The programm counts the length of sequenzes where all numbers are not in\n";
    cerr << 
      "the intervall [a, b] and counts how often a length of 1..t-1 or >=t\n";
    cerr <<
      "was found and makes a Chi^2-test.\n\n";
    cerr << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, t, n;
      double a, b;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      a=atof(argv[3]);
      b=atof(argv[4]);
      t=atol(argv[5]);
      n=atol(argv[6]);
#ifdef LaTeX
      cout << "Intervall $[" << a << "," << b << "]$ \\\\" << endl;
      cout << "Maximall\\\"ange : " << t << "\\\\" << endl;
      cout << "untersuchte Folgen : " << n << "\\\\" << endl;
#else
      cout << "intervall [" << a << "," << b << "]" << endl;
      cout << "max length : " << t << endl;
      cout << "series : " << n << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; gap_test_main(r, runs, sub_max, a, b, t, n);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
