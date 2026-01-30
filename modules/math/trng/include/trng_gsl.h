// ---------------------------------------------------------------------
// Time-stamp: <Sonntag, 27.04.2003,  1:59:27; edited by bauke>
// 
// Tina's random number generators TRNG
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

#ifndef TRNG_GSL_H
#define TRNG_GSL_H

#include <vector>
#include <trng.h>
#include <gsl/gsl_rng.h>


namespace TRNG {
  
  class trng_gsl;  // forward declaration
  //! wrapper class for GSL random number generators 
  /*!
    This class implements a simple wrapper for the random number generators 
    in the GNU Scientific Library. Don't use tese generators in parallel
    applications. Leapfrog and jumping ahead are implemented by throwing
    some numbers away. 
    \see http://sources.redhat.com/gsl/ref/gsl-ref_toc.html
    \author Heiko Bauke
  */
  class trng_gsl : public TRNG::RNG<trng_gsl> {
    gsl_rng *r;
    unsigned long Min_val, Max_val;
    long steps;
    const gsl_rng_type* T;

    long hash_func(const char *key) {
      const long prime=1021;
      long hash=0l;
      long i=0;
      while (key[i]!=0) {
	hash+=key[i];
	++i;
      }
      return hash%prime;
    }
    
  public:
    // TRNG::user1_t for a user implemeneted rng
    static const TRNG::RNG_type type=TRNG::trng_gsl_t;
    
    const char * name(void) {
      return gsl_rng_name(r);
    }
    
    void reset(void) {
      steps=1l;
      Max_val=gsl_rng_max(r);
      Min_val=gsl_rng_min(r);
      if (Max_val>=0x80000000)
	max_val=Max_val/2;
      else
	max_val=Max_val;
      max_val2=max_val/2l;
    }

    void seed(long s=0) {
      gsl_rng_set(r, s);
    }

    long rand(void) {
      unsigned long t;
      t=gsl_rng_get(r);
      for (long i=1l; i<steps; ++i)
	gsl_rng_get(r);
      if (Max_val>=0x80000000)
	return (t-Min_val)/2;
      else
	return t-Min_val;
    }

    void split(long s, long n) {
      if (s<1l || n>s || n<0l)
	throw TRNG::error("invalid arguments for trng_gsl::split");
      if (s>1l) {
	for (long i=0l; i<n; ++i)
	  rand();
	steps*=s;
      }
    }

    void jump2(long s) {
      if (s<0l || s>63l)
	throw TRNG::error("invalid argument for trng_gsl::split");
      unsigned long long to=1ull<<s;
      for (unsigned long long i=0ull; i<to; ++i)
	rand();
    }

    void save_status(std::vector<long> &s) {
      void *state=gsl_rng_state(r);
      size_t n=gsl_rng_size(r);
      s.resize(n+6);
      s[0]=type;
      s[1]=hash_func(gsl_rng_name(r));
      s[2]=steps;
      s[3]=Max_val;
      s[4]=Min_val;
      s[5]=max_val;
      for (size_t i=0; i<n; ++i)
	s[i+6]=*(static_cast<unsigned char *>(state)+i);
    }

    void load_status(const std::vector<long> &s) {
      if (s[0]!=type)
	throw TRNG::error("trng_gsl::load_status wrong parameter");
      gsl_rng_free(r);
      const gsl_rng_type **t0=gsl_rng_types_setup ();
      for (const gsl_rng_type **t=t0; *t!=0; t++)
	if (hash_func((*t)->name)==s[1]) {
	  steps=s[2];
	  Max_val=s[3];
	  Min_val=s[4];
	  max_val=s[5];
	  r=gsl_rng_alloc(*t);
	  if (r==NULL)
	    throw TRNG::error("trng_gsl::load_status not enough memory");
	  void *state=gsl_rng_state(r);
	  size_t n=gsl_rng_size(r);
	  for (size_t i=0; i<n; ++i)
	    *(static_cast<unsigned char *>(state)+i)=
	      static_cast<unsigned char>(s[i+6]);
	  break;
	}
    }

    trng_gsl & operator=(trng_gsl &other) {
      if (this!=&other) {
	std::vector<long> s;
	other.save_status(s);
	load_status(s);
      }
      return *this;
    }
    
    //! constructor
    /*!
      The constructor takes two agruments.
      \param T_ random number generator type, see also GNU Scientific Library
      Reference Manual. 
      \param seed_ default seed
      \see http://sources.redhat.com/gsl/ref/gsl-ref_17.html
    */
    trng_gsl(const gsl_rng_type * T_=gsl_rng_mt19937, long seed_=0l) : T(T_) {
      r=gsl_rng_alloc(T);
      if (r==NULL)
	throw TRNG::error("trng_gsl::trng_gsl not enough memory");
      reset();
      seed(seed_);
    }

    trng_gsl(trng_gsl &other) {
      r=gsl_rng_alloc(gsl_rng_mt19937);
      if (r==NULL)
	throw TRNG::error("trng_gsl::trng_gsl not enough memory");
      std::vector<long> s;
      other.save_status(s);
      load_status(s);
    }
  
    virtual ~trng_gsl() {
      gsl_rng_free(r);
    }
  
  };

}

#endif
