// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 23:02:46; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// tests method  bool TRNG::RNG::poisson_dist(double)
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

double mu;

template<class RNG_type>
void poisson_dist_test(RNG<RNG_type> &r, long runs, long n, 
		       const vector<double> &borders, 
		       const vector<double> &prob) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n: random numbers per test
  // num_classes: number of classes for the Chi-Square test
  vector<double> observ;
  long i, num_fail;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  long num_classes=borders.size();
  observ.resize(num_classes, 0.0);
  num_fail=0l;
  while (runs>0l) {
    for (i=0l; i<num_classes; ++i)
      observ[i]=0.0;
    for (i=0l; i<n; ++i) {
      long k=r.poisson_dist(mu);
      if (k>borders[num_classes-2])
	++observ[num_classes-1];
      else
	for (long j=0l; j<num_classes-1; ++j)
	  if (k<=borders[j]) {
	    ++observ[j];
	    break;
	  }
    }
    Chi2=chi_square_test(prob, observ);
    Chi2P=chi_square_prob(Chi2, num_classes-1l);
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
void poisson_dist_test_main(RNG<RNG_type> &r, long runs, long sub_max, 
			    long n, 
			    const vector<double> &borders,
			    const vector<double> &prob) {
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
    poisson_dist_test(r, runs, n, borders, prob);
  }
}

void calc_borders(vector<double> &borders, vector<double> &prob) {
  borders.resize(0);
  double x, P1=1.0; 
  long j=0l;
  do {
    x=poisson_dist_pdf(j, mu);
    borders.push_back(j);
    prob.push_back(x);
    P1-=x;
    ++j;
  } while (P1>1.0/32.0);
  prob.push_back(P1);
  borders.push_back(-1l);
  double dp;
  do {
    j=0l;
    dp=2.0;
    for (unsigned int i=0; i<prob.size()-1; ++i) {
      if (prob[i+1]+prob[i]<dp) {
	dp=prob[i+1]+prob[i];
      j=i;
      }
    }
    if (dp<1.0/32.0) {
      prob[j+1]+=prob[j];
      prob.erase(prob.begin()+j);
      borders.erase(borders.begin()+j);
    }
  } while (dp<1.0/32.0);
}

int main(int argc, char *argv[]) {
  if (argc!=5) {
    cerr 
      << "Tina's Random Number Generator Library " << TRNG::version() << endl
      <<"(P) & (C) by Heiko Bauke, Magdeburg 2001, 2002" << endl 
      << endl
      << "testing method  bool TRNG::RNG::poisson_dist(double)" << endl
      << "synopsis:" << endl    
      << "$ " << argv[0] << " runs sub_max n mu" << endl 
      << endl
      << "runs: how often the test is applied" << endl
      << "sub_max: how many substreams are tested" << endl
      << "n: number of random numbers per test" << endl
      << "mu: mean" << endl 
      << endl
      << "just try" << endl
      << "$ " << argv[0] << " 8 128 4096 1.0" << endl;
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
      mu=atof(argv[4]);
      if (mu<=0.0)
	throw error("mu out of range");      
#ifdef LaTeX
      cout << n << " W\\\"urfe je Test\\\\" << endl;
#else
      cout << n << " throws per test" << endl;
#endif
      vector<double> borders;
      vector<double> prob;
      calc_borders(borders, prob);

#define FOR_ALL_GENERATORS \
RNG r; poisson_dist_test_main(r, runs, sub_max, n, borders, prob);
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
