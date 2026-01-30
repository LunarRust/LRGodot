// ---------------------------------------------------------------------
// Time-stamp: <Freitag, 24.01.2003, 23:30:34; edited by heiko>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// spectral test for ramdom number generators
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

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>

using namespace std;

const long np_max=31l;
const long max_dim_spec_test=8l;
double d[max_dim_spec_test-1], S[max_dim_spec_test-1], e[max_dim_spec_test-1];


long power_modulo(long a, long e, long m) {
  long long p, al;
  al=(long long)a;
  p=1;
  while (e>0) {
    if ((e&1)==1) {
      p*=al;
      p%=m;
    }
    al*=al;
    al%=m;
    e>>=1;
  }
  return((long)p);
}


long long scalar_prod(long long a[], long long b[], long n) {
  //returns scalarproduct of two vectors a and b with dimension n 
  long long q;
  long i;  
  q=0ll;
  for (i=0; i<n; i++) {
    q+=a[i]*b[i];
  }
  return(q);
}


long long new_min(long long s, long long a[], long n) {
  // returns min(s, (a*a))
  // a is a vector with dimension n
  long long q;
  long i;
  q=0ll;
  for (i=0; i<n; i++) {
    if ((a[i]>0) && (a[i]>s))
      return(s);
    if ((a[i]<0) && ((-a[i])>s))
      return(s);
    q+=a[i]*a[i];
    if (q>s)
      return(s);
  }
  return(q);
}


long long div_round(long long a, long long b) {
  // returns round(a/b); round rounds to the nearest integer
  return(((a>0 && b>0) || (a<0 && b<0)) ? (a+b/2)/b : (a-b/2)/b);
}


int spectral_test(long long a, long long m, 
		  double d[], double S[], double e[], double threshold) {
  // performs the spectral test for linear congruential random number 
  // generators r[i+1]=(a*r[i]+b)%m
  // algorithm is described in Donald Knuth's
  // The Art of Computer Programming, Volume 2, 2nd Ed., pp. 89-113
  // 2nd edition, 1981
  // a: the multiplier
  // m: the modulus
  // spectral test may be applied if
  // the sequence has maximal period, or
  // m is prime and b=0 and the period length is m-1, or
  // m=2^n and a%8=5 or a%8=3
  // spectral_test returns in 
  // d[]: the maximal distance beween adjacent parallel hyperplanes
  // S[]: d[] normalized to upperbounds as in Donald Knuth's
  // The Art of Computer Programming, Volume 2, 2nd Ed., p. 105
  // e[]: m independent measure as suggested by Knuth
  long i, ii, j, k, t, T;
  long long h, hp, p, pp, q, r, s, t1, t2, t3, u, v;
  long long U[max_dim_spec_test][max_dim_spec_test], 
    V[max_dim_spec_test][max_dim_spec_test], 
    X[max_dim_spec_test], Y[max_dim_spec_test], Z[max_dim_spec_test];
  // Initialize
  T=max_dim_spec_test;
  t=2l;
  h=a;    hp=m;  
  p=1ll;  pp=0ll;  
  r=a;    s=1ll+a*a;
  for (i=0; i<T; i++)
    for (ii=0l; ii<T; ii++) {
      U[i][ii]=0ll;
      V[i][ii]=0ll;
    }
  e[0]=3.1415926535897932385/static_cast<double>(m);
  S[0]=.93060485910209959893/pow(static_cast<double>(m), 1.0/2.0);
  e[1]=4.1887902047863909846/static_cast<double>(m);
  S[1]=.89089871814033930475/pow(static_cast<double>(m), 1.0/3.0);
  e[2]=4.9348022005446793096/static_cast<double>(m);
  S[2]=.84089641525371454305/pow(static_cast<double>(m), 1.0/4.0);
  e[3]=5.2637890139143245968/static_cast<double>(m);
  S[3]=.81225239635623552260/pow(static_cast<double>(m), 1.0/5.0);
  e[4]=5.1677127800499700296/static_cast<double>(m);
  S[4]=.77489901121888222692/pow(static_cast<double>(m), 1.0/6.0);
  e[5]=4.7247659703314011698/static_cast<double>(m);
  S[5]=.74299714456847421240/pow(static_cast<double>(m), 1.0/7.0);
  e[6]=4.0587121264167682184/static_cast<double>(m);
  S[6]=.70710678118654752440/pow(static_cast<double>(m), 1.0/8.0);
  // Euclidian step
  while (1) {
    q=hp/h; 
    u=hp-q*h;  v=pp-q*p;
    t1=u*u+v*v;
    if (t1<s) {
      s=t1;
      hp=h;  h=u;
      pp=p;  p=v;
    } else
      break;
  }
  // compute d[2]
  u-=h;  v-=p;
  t1=u*u+v*v;
  if (t1<s) {
    s=t1; 
    hp=u;  pp=v;
  }
  d[t-2l]=sqrt(static_cast<double>(s));
  S[t-2l]*=d[t-2l];
  e[t-2l]*=pow(d[t-2l], static_cast<double>(t));
  if (S[t-2]<threshold)
    return(EXIT_FAILURE);
  U[0][0]=-h;   U[0][1]=p;
  U[1][0]=-hp;  U[1][1]=pp;
  if (pp>0) {
    V[0][0]=-pp;  V[0][1]=-hp;
    V[1][0]=p;    V[1][1]=h;
  } else {
    V[0][0]=pp;   V[0][1]=hp;
    V[1][0]=-p;   V[1][1]=-h;
  }
  while (t<T) {
    // Advance t
    t++;
    r=(a*r)%m;
    k=t-1l;
    j=0;
    U[k][0]=-r;
    U[k][k]=1ll;
    V[k][k]=m;
    for (i=0l; i<k; i++) {
      q=div_round(V[i][0]*r, m);
      V[i][k]=V[i][0]*r-q*m;
      for (ii=0l; ii<t; ii++) 
	U[k][ii]+=q*U[i][ii];
    }
    s=new_min(s, U[k], t);
    do {
      // Transform 
      for (i=0l; i<t; i++) {
	if (i!=j) {
	  t1=scalar_prod(V[i], V[j], t);
	  if (t1<0l)
	    t3=-2l*t1;
	  else
	    t3=2l*t1;
	  t2=scalar_prod(V[j], V[j], t);
	  if (t3>t2) {
	    q=div_round(t1, t2);
	    for (ii=0l; ii<t; ii++) {
	      V[i][ii]-=q*V[j][ii];
	      U[j][ii]+=q*U[i][ii];
	    }
	    k=j;
	  }
	}
      }
      // Examine new bound
      if (k==j)
	s=new_min(s, U[j], t);
      // Advance j
      if (j==(t-1))
	j=0l;
      else
	j++;
    } while (j!=k);
    // Prepare for search
    for (i=0l; i<t; i++) {
      X[i]=0l;
      Y[i]=0l;
      Z[i]=static_cast<long long>
        (sqrt(floor(static_cast<double>
                    (scalar_prod(V[i], V[i], t))*
                    static_cast<double>(s)/static_cast<double>(m*m))));
    }
    k=t-1l;
    do {
      while (1) {
	// Advance X[k]
	if (X[k]==Z[k]) 
	  break;
	X[k]++;
	for (i=0l; i<t; i++)
	  Y[i]+=U[k][i];
	// Advance k
	k++;
	while (k<t) {
	  X[k]=-Z[k];
	  for (i=0l; i<t; i++)
	    Y[i]-=2l*Z[k]*U[k][i];
	  k++;
	}
	s=new_min(s, Y, t);
	break;
      }
      // Decrease k
      k--;
    } while (k>=0l);
    d[t-2l]=sqrt(static_cast<double>(s));
    S[t-2l]*=d[t-2l];
    e[t-2l]*=pow(d[t-2l], static_cast<double>(t));
    if (S[t-2l]<threshold)
      return(EXIT_FAILURE);
  }
  return(EXIT_SUCCESS);
}


int main(int argn, char *argv[]) {
  long m, i, j, np, npp, q, z, von, bis, p[np_max];
  double mean, threshold;
  
  cout.setf(ios::fixed);
  if (argn!=5) {
    cerr << 
      "Tina's Random Number Generator Library\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "Spectral test\n";
    cerr << 
      "-------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ " << argv[0] << " modulus from to threshold\n\n";
    cerr <<
      "usefull arguments: modulus=2147483647; from=2; to=2147483647; threshold=0.6; from<to\n\n";
    cerr << 
      "The Programm applicates the spectral test and prints the best multiplicators\n";
    cerr << 
      "and the scaled results of the test for dimensions 2-8.\n\n";
    cerr << 
      "Four further informations see Donald E. Knuth\n";
    cerr << 
      "\"The Art of Computer Programming\", 1981\n";
    return(EXIT_FAILURE);
  }
  // initialize
  sscanf(argv[1], "%li", &m);
  sscanf(argv[2], "%li", &von);
  sscanf(argv[3], "%li", &bis);
  sscanf(argv[4], "%lf", &threshold);

  z=m-1l;
  q=z;
  i=2l;
  np=0l;
  npp=0l;
  // search for prime factors
  while (z%i==0l) {
    if (np==np_max) {
      cerr << "array p[] too small\n";
      exit(EXIT_FAILURE);
    }
    z/=i;
    p[np]=i;
    np++;
  }
  i++;
  while (z>1l) {
    if (z%i==0l) {
      if (np==np_max) {
        cerr << "array p[] too small\n";
	exit(EXIT_FAILURE);
      }
      z/=i;
      p[np]=i;
      np++;
    } else {
      i++;
      i++;
    }
    if (static_cast<long long>(i)*static_cast<long long>(i)>
	static_cast<long long>(z))
      i=z;
  }
  // search for all relevant divisors
  for (i=0l; i<np; i++)
    p[i]=q/p[i];
  // check all multipliers
  for (i=von; i<bis; i++) {
    j=np-1l;
    while ( (j>=0l) && (power_modulo(i, p[j], m)!=1l) )
      j--;
    if (j<0) {
      if (spectral_test(static_cast<long long>(i), 
			static_cast<long long>(m), d, S, e, threshold)
	  ==EXIT_SUCCESS) {
#ifdef LaTeX
  	cout << i << " & ";
#else
  	cout << i << '\t';
#endif
	mean=0.0;
	for (j=0; j<max_dim_spec_test-1; j++) {
#ifdef LaTeX
  	  cout << setfill(' ') << setprecision(3) << S[j] << " & ";
#else
  	  cout << setfill(' ') << setprecision(3) << S[j] << '\t';
#endif
	  mean+=S[j];
	}
	mean/=max_dim_spec_test;
#ifdef LaTeX
	cout << setfill(' ') << setprecision(3) << mean << " \\\\" << endl;
#else
	cout << setfill(' ') << setprecision(3) << mean << endl;
#endif
      }
    }
  }
  return EXIT_SUCCESS;
}
