// ---------------------------------------------------------------------
// Time-stamp: <Samstag, 03.05.2003, 22:22:59; edited by bauke>
// 
// Tina's random number generators TRNG
//
// lagged Fibonacci generator
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

#ifndef FIBONACCI_H
#define FIBONACCI_H

#include <trng.h>
#include <sstream>


// This is an example for extending Tina's random number generators.
// We implement a exclusive or lagged Fibonacci generator. 
//
//   r_i=r_(i-q) xor r_(i-p); p>q
//
// Sequence splitting and leapfrog method are implemented in a very stupid 
// way. Unused values are just thrown away. Don't use this generator in
// your applications.

// the template parametr determine the lags
template<long p1, long q1>
class Fibonacci : public TRNG::RNG<Fibonacci<p1, q1> > {
private:
  std::vector<long> r;
  long p, q, pointer, steps;
  TRNG::ParkMiller R_init;
  std::ostringstream name_str;
public:
  // TRNG::user1_t for a user implemeneted rng
  static const TRNG::RNG_type type=TRNG::user1_t;

  const char * name(void) {
    return (name_str.str()).c_str();
  }

  void reset(void) {
    steps=1l;
    max_val=0x7fffffffl;
    max_val2=max_val/2l;
  }

  void seed(long s) {
    R_init.seed(s);
    for (int i=0; i<p; ++i)
      r[i]=R_init.rand();
    pointer=0;
  }
  
  // calculate the next random number
  long rand(void) {
    long t;
    ++pointer;
    if (pointer==p)
      pointer=0l;
    r[pointer]=r[pointer]^r[(pointer+q)<p ? (pointer+q) : (pointer+q-p)];
    t=r[pointer];
    for (long i=1l; i<steps; ++i) {
      ++pointer;
      if (pointer==p)
	pointer=0l;
      r[pointer]=r[pointer]^r[(pointer+q)<p ? (pointer+q) : (pointer+q-p)];
    }
    return t;
  }
  
  void split(long s, long n) {
    if (s<1l || n>s || n<0l)
      throw TRNG::error("invalid arguments for Fibonacci::split");
    if (s>1l) {
      for (long i=0l; i<n; ++i)
	rand();
      steps*=s;
    }
  }

  void jump2(long s) {
    if (s<0l || s>63l)
      throw TRNG::error("invalid argument for Fibonacci::split");
    unsigned long long to=1ull<<s;
    for (unsigned long long i=0ull; i<to; ++i)
      rand();
  }

  void save_status(std::vector<long> &s) {
    s.resize(p+3);
    s[0]=type;
    s[1]=pointer;
    s[2]=steps;
    for (int i=0; i<p; ++i)
      s[i+3]=r[i];
  }

  void load_status(const std::vector<long> &s) {
    if (s[0]!=type)
      throw TRNG::error("Fibonacci::load_status wrong parameter");
    pointer=s[1];
    steps=s[2];
    r.resize(p);
      for (int i=0; i<p; ++i)
	r[i]=s[i+3];
  }

  Fibonacci & Fibonacci::operator=(TRNG::RNG<Fibonacci> &other) {
    if (this!=&other) {
      std::vector<long> s;
      other.save_status(s);
      load_status(s);
    }
    return *this;
  }
  
  Fibonacci(long seed_=0l) : r() {
    if (p1<0l || q1<0l || p1==q1)
      throw TRNG::error("bad arguments for Fibonacci::Fibonacci");
    if (p1>q1) {
      p=p1;
      q=q1;
    } else {
      p=q1;
      q=p1;
    }
    r.resize(p);
    reset();
    seed(seed_);
    name_str << "Fibonacci r_i = " 
	     << "r_{i-" << q << "} xor "
	     << "r_{i-" << p << "} mod 2^31";
  }

  virtual ~Fibonacci() {};
};


#endif
