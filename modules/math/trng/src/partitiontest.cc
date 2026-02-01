// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:56:41; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// partition test for random number generators
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
void partition(RNG_type &r, long runs, long d, long k, long n) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // d, k, n: examine n k-tuples
  //          (0..d-1) |
  //          (0..d-1) |
  //          (0..d-1)  > k rows
  //           ..      |
  //          (0..d-1) |
  vector<double> observ, prob;
  vector<long> u;
  long i, j, num_fail, q, p, k2;
  double Chi2, Chi2P, t1, t2;
  cout.setf(ios::fixed);
  u.resize(k+1);
  t1=t2=static_cast<double>(d);
  prob.resize(k, pow(t1, static_cast<double>(-k)));
  num_fail=0l;
  for (i=k-1l; i>=0; --i) {
    prob[i]*=t2*Stirling_num2(k, k-i);
    --t1;
    t2*=t1;
  }
  i=k-1l;
  while (prob[i]<0.001) {
    prob[i-1]+=prob[i];
    prob.pop_back();
    --i;
  }
  k2=i+1l;
  observ.resize(k2);
  while (runs>0l) {
    for (i=0l; i<k2; ++i)
      observ[i]=0.0;
    for (i=0l; i<n; ++i) {
      for (j=0; j<=k; ++j)
	u[j]=-1l;
      for (j=0l; j<k; ++j) {
	q=static_cast<long>(r.uniform()*d);
	p=0l;
	while (q!=u[p] && u[p]!=-1l)
	  ++p;
	u[p]=q;
      }
      p=0l;
      while (u[p]!=-1l)
	++p;
      ++observ[(k-p)<k2 ? k-p : k2-1];
    }
    Chi2P=0.0;
    try {
      Chi2=chi_square_test(prob, observ);
      Chi2P=chi_square_prob(Chi2, k2-1l);
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
void partition_main(RNG_type &r,long runs, long sub_max, long d, long k, long n) {
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
    partition(r, runs, d, k, n);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=6) {
    cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Partition test\n";
    cerr << 
      "--------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max d k n\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; d=52; k=5; n=5000\n\n";
    cerr << 
      "The Programm forms integer k-tuples,\n";
    cerr << "  (0..d-1) |\n";
    cerr << "  (0..d-1) |\n";
    cerr << "  (0..d-1)  > k rows\n";
    cerr << "   ..      |\n";
    cerr << "  (0..d-1) |\n";
    cerr << 
      "counts how many distrinct values were found and makes a Chi^2-test.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, d, k, n;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      d=atol(argv[3]);
      k=atol(argv[4]);
      n=atol(argv[5]);
#ifdef LaTeX
      cout << k << "-Tupel mit \\\\" << endl;
      cout << d << " verschiedenen Werten\\\\" << endl;
      cout << n << " Tupel je Test\\\\" << endl;
#else
      cout << k << "-tuple with" << endl;
      cout << d << " values" << endl;
      cout << n << " tuple per Test" << endl;
#endif

#define FOR_ALL_GENERATORS \
RNG r; partition_main(r, runs, sub_max, d, k, n);
#include "for_all_generators.h"

    }
    catch (...) {
      cerr << "something went wrong" << endl;
    }
  }
  return EXIT_SUCCESS;
}
