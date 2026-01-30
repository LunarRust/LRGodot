// ---------------------------------------------------------------------
// Time-stamp: <Montag, 21.04.2003, 16:13:34; edited by bauke>
// 
// Tina's random number generators TRNG
//
// testing random number generators 
// parallel Monte Carlo simmulation for the 2d Ising model
// using a stack
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
#include <stack>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <mpi.h>
#include <trng.h>

using namespace std;
using namespace TRNG;

int rank, size;

typedef struct {
  int x;
  int y;
} koord;

class lattice {
private:
  vector<int> s;
  int L, L2;
  int pos(int);
public:
  int size(void);
  void resize(int, int);
  void fill(int);
  void flipp(koord);
  int get(koord);
  void set(koord, int);
  double energy(void);
  double magnet(void);
  void print(void);
  lattice(int);
  lattice();
};

inline int lattice::pos(int x) {
  while (x<0)
    x+=L;
  while (x>=L)
    x-=L;
  return x;
}

int lattice::size(void) {
  return L;
}

void lattice::resize(int newL, int w=-1) {
  if (newL>0) {
    L=newL;
    L2=L*L;
    s.resize(L2);
  } else {
    cerr << "negative size\n";
    exit(EXIT_FAILURE);
  }
}

inline void lattice::flipp(koord r) {
   s[pos(r.x)+pos(r.y)*L]*=-1;;
}

void lattice::fill(int w) {
  for (int i=0; i<L2; ++i)
    s[i]=w;
}

inline int lattice::get(koord r) {
  return s[pos(r.x)+pos(r.y)*L];
}

void lattice::set(koord r, int w) {
  s[pos(r.x)+pos(r.y)*L]=w;
}

double lattice::energy(void) {
  double e=0.0;
  for (int i=0; i<L; ++i)
    for (int j=0; j<L; ++j)
      e-=s[i+j*L]*(s[pos(i+1)+j*L]+s[i+pos(j+1)*L]);
  return e/L2;
}

double lattice::magnet(void) {
  double m=0.0;
  for (int i=0; i<L; ++i)
    for (int j=0; j<L; ++j)
      if (s[i+j*L]<0)
	--m;
      else
	++m;
  return fabs(m)/L2;
}

void lattice::print(void) {
  int i, j;
  for (int i=0; i<L; ++i) {
    for (int j=0; j<L; ++j)
      if (s[i+j*L]<0)
	cout << '.';
      else
	cout << '#';
    cout << endl;
  }
  cout << endl;
}

lattice::lattice(int newL) {
  lattice::resize(newL);
}

lattice::lattice() {
  L=0;
  L2=0;
}

template<class RNG_type>
void wolffstep(RNG_type &R, lattice &s, double T) {
  stack<koord> buffer;
  double padd;
  int oldspin;
  koord r;
  
  padd=1.0-exp(-2.0/T);
  r.x=static_cast<int>(s.size()*R.uniform());
  r.y=static_cast<int>(s.size()*R.uniform());
  oldspin=s.get(r);
  s.flipp(r);
  buffer.push(r);
  while (!buffer.empty()) {
    r=buffer.top();
    buffer.pop();
    --r.x;
    if (s.get(r)==oldspin)
      if (R.boolean(padd)) {
	buffer.push(r);
	s.flipp(r);
      }
    ++r.x;
    ++r.x;
    if (s.get(r)==oldspin)
      if (R.boolean(padd)) {
	buffer.push(r);
	s.flipp(r);
      }
    --r.x;
    --r.y;
    if (s.get(r)==oldspin)
      if (R.boolean(padd)) {
	buffer.push(r);
	s.flipp(r);
      }
    ++r.y;
    ++r.y;
    if (s.get(r)==oldspin)
      if (R.boolean(padd)) {
	buffer.push(r);
	s.flipp(r);
      }
    --r.y;
  }
}

void output(vector<double> &Ea, vector<double> &ca, int simulations, 
	    double E_exact, double c_exact) {
  int j;
  Ea[simulations]=0.0;
  ca[simulations]=0.0;
  Ea[simulations+1]=0.0;
  ca[simulations+1]=0.0;
  for (j=0; j<simulations; ++j) {
    Ea[simulations]+=Ea[j]/simulations;
    ca[simulations]+=ca[j]/simulations;
  }
  for (j=0; j<simulations; ++j) {
    Ea[simulations+1]+=(Ea[j]-Ea[simulations])*(Ea[j]-Ea[simulations]);
    ca[simulations+1]+=(ca[j]-ca[simulations])*(ca[j]-ca[simulations]);
  }
  Ea[simulations+1]/=(simulations-1.0)*simulations;
  Ea[simulations+1]=sqrt(Ea[simulations+1])*Student_t(0.99, simulations-1l);
  ca[simulations+1]/=(simulations-1.0)*simulations;
  ca[simulations+1]=sqrt(ca[simulations+1])*Student_t(0.99, simulations-1l);
  cout << endl << "\t E\t\t c" << endl;
  for (j=0; j<simulations; ++j)
    cout << '\t' << setprecision(8) << Ea[j] << '\t' 
	 << setprecision(8) << ca[j] << endl;
  cout << "\t--------------\t--------------" << endl;
  cout << "mean\t" << setprecision(8) << Ea[simulations] << '\t' 
       << setprecision(8) << ca[simulations] << endl;
  cout << "Del\t" 
       << setprecision(8) << Ea[simulations]-E_exact << '\t' 
       << setprecision(8) << ca[simulations]-c_exact << endl;
  cout << "sig\t" << setprecision(8) << Ea[simulations+1] << '\t' 
       << setprecision(8) << ca[simulations+1] << endl;
  cout << "Del/sig\t" 
       << setprecision(8) 
       << fabs(Ea[simulations]-E_exact)/Ea[simulations+1] << '\t' 
       << setprecision(8) 
       << fabs(ca[simulations]-c_exact)/ca[simulations+1] << endl;
}


template<class RNG_type>
void wolff_main(RNG_type &R, int runs) {
  const int L=12;
//    const double E_exact=-1.453064854; // 16x16
//    const double c_exact= 1.498704952; // 16x16
  const double E_exact=-1.465960817; // 12x12
  const double c_exact= 1.352950680; // 12x12
//    const double E_exact=-1.491589107;  // 8x8
//    const double c_exact= 1.145559238;  // 8x8
  const int simulations=10;
  int i, j;
  double T, E, E2, q, c;
  vector<double> Ea(simulations+2);
  vector<double> ca(simulations+2);
  vector<double> Eaa(size+2);
  vector<double> caa(size+2);
  MPI::Status status;

  R.split(size, rank);
  cout.setf(ios::fixed);
  cout.setf(ios::showpoint);
  T=2.0/log(1.0+sqrt(2.0));
  lattice s(L);
  s.fill(-1);
  if (rank==0) {
    cout << R.name() << endl;
    cout << "T = " << T << endl;
  }
  for (j=0; j<simulations; ++j) { 
    E=0.0;
    E2=0.0;
    for (i=0; i<runs; ++i) {
      wolffstep(R, s, T);
      q=s.energy();
      E+=q;
      E2+=q*q;
    }
    E/=runs;
    E2/=runs;
    c=(L*L)/(T*T)*(E2-E*E);
    Ea[j]=E;
    ca[j]=c;
  }
  if (rank==0) {
    output(Ea, ca, simulations, E_exact, c_exact);
    if (size>1) {
      Eaa[0]=Ea[simulations];
      caa[0]=ca[simulations];
      for (i=1; i<size; ++i) {
	for (j=0; j<simulations; ++j) {
	  MPI::COMM_WORLD.Recv(&Ea[j], 1, MPI::DOUBLE, i, 0, status);
	  MPI::COMM_WORLD.Recv(&ca[j], 1, MPI::DOUBLE, i, 0, status);
	}
	output(Ea, ca, simulations, E_exact, c_exact);
	Eaa[i]=Ea[simulations];
	caa[i]=ca[simulations];
      }
      output(Eaa, caa, size, E_exact, c_exact);
    }
    cout << endl 
	 << "--------------------------------------" << endl
	 << endl;
  } else
    for (j=0; j<simulations; ++j) {
      MPI::COMM_WORLD.Send(&Ea[j], 1, MPI::DOUBLE, 0, 0);
      MPI::COMM_WORLD.Send(&ca[j], 1, MPI::DOUBLE, 0, 0);
    }
}

int main (int argc, char *argv[]) {
  MPI::Init(argc, argv);
  rank=MPI::COMM_WORLD.Get_rank();
  size=MPI::COMM_WORLD.Get_size();
  if (argc!=2) {
     cerr << 
      "Tina's Random Number Generator Library " << TRNG::version() << "\n";
    cerr << 
      "(P) & (C) by Heiko Bauke, Magdeburg 2001\n\n";
    cerr << 
      "ising 2d wolf test\n";
    cerr << 
      "------------------\n\n";
    cerr << 
      "synopsis:\n";
    cerr << 
      "$ mpirun -np n_proc " << argv[0] << " runs\n";
    cerr <<
      "usefull arguments: n_proc=8; runs=100000\n";
  } else {
    int runs=atoi(argv[1]);

#define FOR_ALL_GENERATORS \
RNG r; wolff_main(r, runs);
#include "for_all_generators.h"

  }
  return EXIT_SUCCESS;
}

