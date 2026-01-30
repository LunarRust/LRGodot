// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 21:46:58; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// tests method  bool TRNG::RNG::Student_t_dist(double)
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

double nu;

double p(double x) {
  return Student_t_dist_pdf(x, nu);
}

double integrate(double (*f)(double), double I, double x0, double dx) {
  double y1, y2, y3=f(x0);
  while (I>0.0) {
    y1=y3;
    y2=f(x0+dx/2.0);
    x0+=dx;
    y3=f(x0);
    I-=fabs(dx)*(y1+4.0*y2+y3)/6.0;
  }
  return x0;
}

template<class RNG_type>
void Student_t_dist_test(RNG<RNG_type> &r, long runs, long n, long num_classes, 
			 const vector<double> &borders) {
  // r: random number generator
  // runs: number of Chi-Square tests
  // n: random numbers per test
  // num_classes: number of classes for the Chi-Square test
  vector<double> observ, prob;
  long i, num_fail;
  double Chi2, Chi2P;
  cout.setf(ios::fixed);
  observ.resize(num_classes, 0.0);
  prob.resize(num_classes, 1.0/num_classes);
  num_fail=0l;
  while (runs>0l) {
    for (i=0l; i<num_classes; ++i)
      observ[i]=0.0;
    for (i=0l; i<n; ++i)
      ++observ[find_interval(borders, r.Student_t_dist(nu))];
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
void Student_t_dist_test_main(RNG<RNG_type> &r, long runs, long sub_max, 
			    long n, long num_classes,
			    const vector<double> &borders) {
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
    Student_t_dist_test(r, runs, n, num_classes, borders);
  }
}

void calc_borders(vector<double> &borders, const long num_classes) {
  borders.resize(num_classes-1);
  if (num_classes%2==0) {
    double x=0.0;
    borders[num_classes/2-1]=0.0;
    for (int j=1; j<num_classes/2; ++j) {
      x=integrate(p, 1.0/num_classes, x, 1.0/1024.0/num_classes);
      borders[num_classes/2-1+j]=x;
    }
    x=0.0;
    for (int j=1; j<num_classes/2; ++j) {
      x=integrate(p, 1.0/num_classes, x, -1.0/1024.0/num_classes);
      borders[num_classes/2-1-j]=x;
    }
  } else {
    double x=0.0;
    x=integrate(p, 1.0/2.0/num_classes, x, 1.0/1024.0/num_classes);
    borders[num_classes/2]=x;
    for (int j=1; j<num_classes/2; ++j) {
      x=integrate(p, 1.0/num_classes, x, 1.0/1024.0/num_classes);
      borders[num_classes/2+j]=x;
    }
    x=0.0;
    x=integrate(p, 1.0/2.0/num_classes, x, -1.0/1024.0/num_classes);
    borders[num_classes/2-1]=x;
    for (int j=1; j<num_classes/2; ++j) {
      x=integrate(p, 1.0/num_classes, x, -1.0/1024.0/num_classes);
      borders[num_classes/2-1-j]=x;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc!=6) {
    cerr 
      << "Tina's Random Number Generator Library " << TRNG::version() << endl
      <<"(P) & (C) by Heiko Bauke, Magdeburg 2001, 2002" << endl 
      << endl
      << "testing method  bool TRNG::RNG::Student_t_dist(double)" << endl
      << "synopsis:" << endl    
      << "$ " << argv[0] << " runs sub_max n num_classes nu" << endl 
      << endl
      << "runs: how often the test is applied" << endl
      << "sub_max: how many substreams are tested" << endl
      << "n: number of random numbers per test" << endl
      << "num_classes: number of classes" << endl
      << "nu: parameter nu, degrees of freedom" << endl 
      << endl
      << "just try" << endl
      << "$ " << argv[0] << " 8 128 4096 64 1.0" << endl;
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
      long num_classes=atol(argv[4]);
      if (num_classes<2l)
	throw error("num_classes out of range");      
      nu=atof(argv[5]);
      if (nu<=0.0)
	throw error("parameter nu out of range");
#ifdef LaTeX
      cout << n << " W\\\"urfe je Test\\\\" << endl
	   << num_classes << " Klassen\\\\" << endl;
#else
      cout << n << " throws per test" << endl
	   << num_classes << " classes" << endl;
#endif
      vector<double> borders;
      calc_borders(borders, num_classes);

#define FOR_ALL_GENERATORS \
RNG r; Student_t_dist_test_main(r, runs, sub_max, n, num_classes, borders);
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
