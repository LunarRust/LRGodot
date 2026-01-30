// ---------------------------------------------------------------------
// Time-stamp: <Sonntag, 27.04.2003,  1:48:42; edited by bauke>
// 
// Tina's random number generators TRNG
//
// useful functions for Tina's random number generators 
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

#if !defined TRNGLIB_H

#define TRNGLIB_H

#include <vector>
#include <exception>
#include <string>

namespace TRNG {

  //! class for %error handling.
  /*!
    This class is thrown, if an %error occurs. It is public public inherited by
    std::exception.
  */
  class error : public std::exception {
    std::string _what;
  public:
    virtual const char * what(void) const throw() {
      return _what.c_str();
    };
    explicit error(const std::string &what_arg) : _what(what_arg) {
    };
    virtual ~error() throw() {
    };
    
  };

  //! power
  /*!
    This data structure is used in the YARN random number generator family. 
    It allows the fast calculation of all powers of a given base \f$b\f$ 
    modulo \f$m\f$.
  */
  class power {
    long b, modulus;
    unsigned long b_power0[0x10000];
    unsigned long b_power1[0x08000];
  
    long pow(long n) {
      long long p=1ll, t=b;
      while (n>0) {
	if ((n&0x1)==0x1) 
	  p=(p*t)%modulus;
	t=(t*t)%modulus;
	n/=2;
      }
      return static_cast<long>(p);
    }

    void calc_b_power(void) {
      for (long i=0l; i<0x10000l; ++i)
	b_power0[i]=pow(i);
      for (long i=0l; i<0x08000l; ++i)
	b_power1[i]=pow(i*0x10000l);
    }

  public:
    long operator()(const long n) const {
      if (modulus==2147483647l) {
 	unsigned long long z=
 	  static_cast<unsigned long long>(b_power1[n>>16])*
 	  static_cast<unsigned long long>(b_power0[n&0xffff]);
	z=(z&0x7fffffffull)+(z>>31);
 	return (z>=static_cast<unsigned long>(modulus)) ? (z-modulus) : z;
      } else
	return static_cast<long int>((static_cast<unsigned long long>(b_power1[n>>16])*
				      static_cast<unsigned long long>(b_power0[n&0xffff]))
				     %modulus);
    }
    
    long operator()(void) const {
      return b;
    }

    void set(const long b_, const long modulus_) {
      b=b_;
      modulus=modulus_;
      calc_b_power();
    }
    
    power(const long b_, const long modulus_) : 
      b(b_),
      modulus(modulus_) {
      calc_b_power();
    }

    virtual ~power() {
    }
  };

  const char * version(void);
  long modulo_invers(long, long);
  void gauss(std::vector<long> &, std::vector<long> &, long);
  void matrix_mult(const std::vector<long> &, const std::vector<long> &,
		   std::vector<long> &, long);
  void matrix_vec_mult(const std::vector<long> &, const std::vector<long> &, 
		       std::vector<long> &, long);

  double Gamma(double);
  double ln_Gamma(double);
  double Gamma_P(double, double);
  double Gamma_Q(double, double);
  double incomp_Gamma(double, double);
  double comp_incomp_Gamma(double, double);
  double Gamma_ser(double, double);
  double Gamma_cf(double, double);
  double ln_factorial(long);
  long binomial_coeff(long, long);
  double errf(double);
  double chi_square_test(const std::vector<double> &, 
			 const std::vector<double> &);
  double chi_square_prob(double, long);
  double Stirling_num2(long, long);
  double Student_t(double, long, bool=true);
  long find_interval(const std::vector<double> &, double);

  double uniform_pdf(double);
  double uniform_pdf(double, double, double);
  double uniformco_pdf(double);
  double uniformco_pdf(double, double, double);
  double uniformcc_pdf(double);
  double uniformcc_pdf(double, double, double);
  double uniformoc_pdf(double);
  double uniformoc_pdf(double, double, double);
  double uniformoo_pdf(double);
  double uniformoo_pdf(double, double, double);
  double normal_dist_pdf(double, double, double);
  double exp_dist_pdf(double, double);
  double laplace_dist_pdf(double, double);
  double tent_dist_pdf(double, double);
  double Gamma_dist_pdf(double, double, double);
  double Beta_dist_pdf(double, double, double);
  double chi_square_dist_pdf(double, double);
  double Student_t_dist_pdf(double, double);
  double binomial_dist_pdf(long, long, double);
  double poisson_dist_pdf(long, double);
  double geometric_dist_pdf(long, double);

}
  
#endif


