// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 22:19:09; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// collision test for random number generators
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
void collision(RNG_type &r, long runs, long n, long m, 
	       long count_min, long count_max) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n, m: n balls fall into m urns
  // count_min, long count_max: lower and upper bound
  vector<double> prob, T;
  vector<bool> urn;
  long i, j, k, l, t, collisions, num_fail;
  cout.setf(ios::fixed);
  n=1l<<n;
  urn.resize(1l<<m);
  for (l=0l; l<31l; ++l) {
#ifdef LaTeX
    cout << l << ". bit: " << " & ";
#else
    cout << l << ". bit: " << "\t";
#endif
    num_fail=0l;
    for (k=0l; k<runs; ++k) {
      collisions=0l;
      for (i=0l; i<(1l<<m); i++)
	urn[i]=false;
      for (i=0l; i<n; ++i) {
	t=0l;
	for (j=0l; j<m; ++j)
	  t|=(r.rand()&(1l<<l)) ? (1l<<j) : 0l;
	if (urn[t])
	  ++collisions;
	else
	  urn[t]=true;
      }
      if (collisions<=count_min || collisions>count_max) {
#ifdef LaTeX
	cout << "\\textbf{";
#endif
	++num_fail;
      }
      cout << setfill(' ') << collisions;
#ifdef LaTeX
      if (collisions<=count_min || collisions>count_max)
	cout << "}";
      cout << " & ";
#else
      cout << '\t';
#endif
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
}


template<class RNG_type>
void collision_main(RNG_type &r, long runs, long sub_max, long n, long m) {
  vector<double> prob, T;
  vector<char> urn;
  long i, j, j0, j1, t, t_max, n1, count_min, count_max;
  double q, p;
  cout.setf(ios::fixed);
  t_max=14l;
  n1=1l<<n;
  prob.resize(n1+1);
  T.resize(t_max);
  urn.resize(1l<<m);
  T[0]=0.01;   T[1]=0.02;
  T[2]=0.03;   T[3]=0.05;
  T[4]=0.10;   T[5]=0.25;
  T[6]=0.50;   T[7]=0.75;
  T[8]=0.90;   T[9]=0.95;
  T[10]=0.97;  T[11]=0.98;
  T[12]=0.99;  T[13]=1.00;
  for (j=0l; j<=n1; ++j)
    prob[j]=0.0;
  prob[1]=1.0;
  j0=j1=1l;
  for (i=1l; i<n1; ++i) {
    ++j1;
    for (j=j1; j>=j0; --j) {
      q=static_cast<double>(j)/static_cast<double>(1l<<m);
      prob[j]=q*prob[j]+((1.0+1.0/static_cast<double>(1l<<m))-q)*prob[j-1];
    }
    if (prob[j]<1e-24) {
      prob[j]=0.0;
      if (j==j1)
	--j1;
      if (j==j0)
	++j0;
    }
  }
  p=0.0;
  t=1l;
  j=j0-1l;
#ifdef LaTeX
  cout << "Kollisionen $\\le$ & mit Wkt. \\\\" << endl;
#else
  cout << "Collisions <=\twith prob." << endl;
#endif
  do {
    ++j;
    p+=prob[j];
    if (p>T[t-1]) {
      if (n1-j-1l>=0l) 
#ifdef LaTeX
	cout << n1-j-1l << " & " << 1.0-p << "\\\\" << endl;
#else      
        cout << n1-j-1l << "\t\t" << 1.0-p << endl;
#endif
      if (t==1l)
	count_max=n1-j-1l;
      if (t==t_max-1l)
	count_min=n1-j-1l;
      while (!(p<=T[t-1]) && !(t==t_max))
	++t;
    }
  } while (!(t==t_max));
#ifdef LaTeX
  cout << "\\multiciol{" << runs+2l <<"}{@{}l@{}}{Generator " << r.name() 
       << "}\\\\" << endl << "\\midrule" << endl;
#else
  cout << "Generator " << r.name() << endl;
#endif
  for (j=1l; j<=sub_max; ++j) {
#ifdef LaTeX
    cout << "\\multiciol{" << runs+2l <<"}{@{}l@{}}{Splitting level " << j 
	 << "}\\\\" << endl;
#else
    cout << "Splitting level " << j << endl;
#endif
    r.reset();
    r.split(j, 0l);
    collision(r, runs, n, m, count_min, count_max);
  }
}


int main(int argc, char *argv[]) {
  if (argc!=5) {
    cerr << 
      "Tina's Random Number Generator Library\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Collision test\n";
    cerr << 
      "--------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " runs sub_max n m\n\n";
    cerr <<
      "usefull arguments: runs=5; sub_max=128; n=14; m=20; m<32; n<m\n\n";
    cerr << 
      "The Programm throws 2^n balls into 2^m urns and counts how often\n";
    cerr << 
      "a ball is thown into a urn that already contains a ball.\n\n";
    cerr << 
      "To decide in which urn a ball is thrown, the programm takes the i-th\n";
    cerr <<
      "bit from m random 31-bit integer and forms a m-bit integer.\n\n";
    cerr 
      << "Four further informations see Donald E. Knuth\n";
    cerr << "\"The Art of Computer Programming\", 1981\n";
  } else {
    try {
      long runs, sub_max, n, m;
      runs=atol(argv[1]);
      sub_max=atol(argv[2]);
      n=atol(argv[3]);
      m=atol(argv[4]);
      cout << "2^" << n << " balls" << endl;
      cout << "2^" << m << " urns" << endl;

#define FOR_ALL_GENERATORS \
RNG r; collision_main(r, runs, sub_max, n, m);
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
