// ---------------------------------------------------------------------
// Time-stamp: <Samstag, 20.09.2003, 23:00:40; edited by bauke>
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

#if !defined TRNG_H

#define TRNG_H

#include <iostream>
#include <cmath>
#include <climits>
#include <vector>
#include <trnglib.h>

#define TRNG_VERSION "3"
#define TRNG_REVISION "1"


//! Tina's random number generators namespace
/*!
  All function and classes are encapsulated by the namespace TRNG.
*/
namespace TRNG {
  
  //! Pseudo random number generator types
  /*!
    Every pseudo random number generator's type can be identified by a class
    member type which has anny value from this enumeration type.
  */
  enum RNG_type {
    RNG_t,          //!< not specialized random number generator 
    generic_MLCG_t, //!< generic multiplicative linear congruential random number generator
    ParkMiller_t,   //!< random number generator class ParkMiller
    LCG32_t,        //!< random number generator class LCG32
    LCG64_t,        //!< random number generator class LCG64
    MRG2_t,         //!< random number generator class MRG2
    MRG3_t,         //!< random number generator class MRG3
    MRG4_t,         //!< random number generator class MRG4
    MRG5_t,         //!< random number generator class MRG5
    YARNLCG64_t,    //!< random number generator class YARNLCG64
    YARN2_t,        //!< random number generator class YARN2
    YARN3_t,        //!< random number generator class YARN3
    YARN4_t,        //!< random number generator class YARN4
    YARN5_t,        //!< random number generator class YARN5
    CLCG2_t,        //!< random number generator class CLCG2
    CLCG3_t,        //!< random number generator class CLCG3
    CLCG4_t,        //!< random number generator class CLCG4
    EINV_t,         //!< random number generator class EINV
    EINVLCG64_t,    //!< random number generator class EINVLCG64
    trng_gsl_t,     //!< random number generator class trng_gsl
    user1_t=1001,   //!< user defined random number generator class nr 1
    user2_t=1002,   //!< user defined random number generator class nr 2
    user3_t=1003,   //!< user defined random number generator class nr 3
    user4_t=1004,   //!< user defined random number generator class nr 4
    user5_t=1005    //!< user defined random number generator class nr 5
  };   
 
  //! Two dimensional vector structure
  struct vector2d_struct {
    //! 1st element
    double x1;
    //! 2nd element
    double x2;
  };

  //! Two dimensional vector
  /*! 
    This structure is for storing two dimensional vectors. The method
    TRNG::RNG::spherical2d(void) returns this structure.
   */
  typedef struct vector2d_struct vector2d;
  
  //! Three dimensional vector structure
  struct vector3d_struct {
    //! 1st element
    double x1;
    //! 2nd element
    double x2;
    //! 3rd element
    double x3;
  };

  //! Three dimensional vector
  /*! 
    This structure is for storing three dimensional vectors. The method
    TRNG::RNG::spherical3d(void) returns this structure.
  */
  typedef struct vector3d_struct vector3d;

  //! Four dimensional vector structure
  struct vector4d_struct {
    //! 1st element
    double x1;
    //! 2nd element
    double x2;
    //! 3rd element
    double x3;
    //! 4th element
    double x4;
  };

  //! Four dimensional vector
  /*! 
    This structure is for storing four dimensional vectors. The method
    TRNG::RNG::spherical4d(void) returns this structure.
   */
  typedef struct vector4d_struct vector4d;

  // -------------------------------------------------------------------

  //! Pseudo random number generator template
  /*! 
    All the pseudo random number generators are derived form this base
    class. Routines for generation of uniform and nonuniform variartes 
    are implemented in this base class. Generator specific tasks like 
    sequence splitting or leapfrog method are implementetd by the derived 
    classes. The method rand(), the generator's core method, has of 
    course also to be reimplemented for every new generator. 
    \author Heiko Bauke
  */
  template<class RNG_type> class RNG {
  private:
    RNG_type & derived(void) {
      return static_cast<RNG_type &>(*this);
    }
    
  protected:
    //! Maximum value that the random number gernerator's method rand() can return 
    long max_val;
    //! Half of the maximum value that method rand() of the random number gernerator can return 
    long max_val2;
  public:
    //! Pseudo random number generator type
    /*!	
      This numerical value determins the random number generator type.
    */
    static const TRNG::RNG_type type=RNG_t;
    
    //! Name of Pseudo random number generator
    /*! 
      Returns a pointer to a zero terminated string containing the name of
      pseudo random number generator.
      \return pointer to a zero termiated string
    */
    const char * name(void) {
      return derived().name(); 
    }
    
    //! Reset the pseudo random number generator
    /*! 
      The parameters of the pseudo random number generator are set to some 
      default values.
    */
    void reset(void)  { 
      derived().reset(); 
    }
    
    //! Seed the pseudo random number generator
    /*! 
      The state of the pseudo random number generator is set. Calling reset()
      and seed() with the seed value used by the initialisation will
      set the generator in the same state as after initialisation.
      \param s new seed
      \exception error if \f$s<0\f$
      \sa TRNG::error
    */
    void seed(long s=0l) { 
      derived().seed(s);
    }
    
    //! Next pseudo random number
    /*! 
      This is core method of the generator. It calculates the next pseudo
      random number. It is an integer number \f$r\f$ with 
      \f$0\le r\le\mathrm{max}\f$. You can determine the upper bound 
      \f$\mathrm{max}\f$ by calling the method max().	
      \return next integer pseudo random number
    */
    long rand(void) { 
      return derived().rand(); 
    }

    //! Maximal pseudo random number
    /*!
      \return maximal pseudo random number returned by rand()
    */
    long max(void) {
      return max_val;
    };
    
    //! Pseudo random boolean
    /*!
      \return pseudo random boolean with probability \f$\frac{1}{2}\f$ for 
      return value true
    */
    bool boolean(void) {
      return (rand()<=max_val2) ? true : false;
    }

    //! Pseudo random boolean
    /*!
      \return pseudo random boolean with probability \f$p\f$ for return value 
      true
    */
    bool boolean(const double p) {
      return (rand()<static_cast<long>(p*max())+1.0) ? true : false;
    }
    
    //! Pseudo random number
    /*!
      \return a in \f$[0,1)\f$ uniform distributed random number
    */
    double uniform(void) {
      return uniformco();
    }

    //! Pseudo random number
    /*!
      \return a in \f$[a,b)\f$ uniform distributed random number
      \param a lower bound
      \param b upper bound
    */
    double uniform(const double a, const double b) {
      return uniformco(a, b);
    }

    //! Pseudo random number
    /*!
      \return a in \f$[0,1)\f$ uniform distributed random number
    */
    double uniformco(void) {
      return static_cast<double>(rand())/
	(static_cast<double>(max())+1.0);
    }

    //! Pseudo random number
    /*!
      \return a in \f$[a,b)\f$ uniform distributed random number
      \param a lower bound
      \param b upper bound
    */
    double uniformco(const double a, const double b) {
      return a+(b-a)*static_cast<double>(rand())/
	(static_cast<double>(max())+1.0);
    }

    //! Pseudo random number
    /*!
      \return a in \f$[0,1]\f$ uniform distributed random number
    */
    double uniformcc(void) {
      return static_cast<double>(rand())/
	static_cast<double>(max());
    }
    
    //! Pseudo random number
    /*!
      \return a in \f$[a,b]\f$ uniform distributed random number
      \param a lower bound
      \param b upper bound
    */
    double uniformcc(const double a, const double b) {
      return a+(b-a)*static_cast<double>(rand())/
	static_cast<double>(max());
    }

    //! Pseudo random number
    /*!
      \return a in \f$(0,1]\f$ uniform distributed random number
    */
    double uniformoc(void) {
      return (static_cast<double>(rand())+1.0)/
	(static_cast<double>(max())+1.0);
    }
    
    //! Pseudo random number
    /*!
      \return a in \f$(a,b]\f$ uniform distributed random number
      \param a lower bound
      \param b upper bound
    */
    double uniformoc(const double a, const double b) {
      return a+(b-a)*(static_cast<double>(rand())+1.0)/
	(static_cast<double>(max())+1.0);
    }
    
    //! Pseudo random number
    /*!
      \return a in \f$(0,1)\f$ uniform distributed random number
    */
    double uniformoo(void) {
      return (static_cast<double>(rand())+1.0)/
	(static_cast<double>(max())+2.0);
    }

    //! Pseudo random number
    /*!
      \return a in \f$(a,b)\f$ uniform distributed random number
      \param a lower bound
      \param b upper bound
    */
    double uniformoo(const double a, const double b) {
      return a+(b-a)*(static_cast<double>(rand())+1.0)/
	(static_cast<double>(max())+2.0);
    }
    
    //! Pseudo random number
    /*!
      \return If \f$b>0\f$ a in \f$[0, b)\f$ uniform distributed natural random number is returned, if \f$b<0\f$ return value is in \f$(b, 0]\f$.
      \param b upper bound
    */
    long uniforml(const long b) {
      return static_cast<long>(uniform(0.0, static_cast<double>(b)));
    }
    
    //! Pseudo random number
    /*!
      \return a in \f$[a, b)\f$ uniform distributed natural random number
      \param a lower bound
      \param b upper bound
    */
    long uniforml(const long a, const long b) {
      return static_cast<long>(uniform(static_cast<double>(a),
				       static_cast<double>(b)));
    }
    
    //! Pseudo random number
    /*!
      A normal distributed random variate has a probability density
      \f[
      p(x)=\dfrac{1}{\sqrt {2\pi}\sigma}\mathrm{e}^{-\frac{x^2}{2\sigma^2}}\,.
      \f]
      This method uses the polar (Box-Mueller) method\latexonly, see 
      \cite{Knuth1981} for details\endlatexonly.
      \return a normal distributed random number with mean \f$\mu\f$ and variance
      \f$\sigma\f$ 
      \param sigma variance \f$\sigma\f$
      \param mu mean \f$\mu\f$
      \exception error if \f$\sigma\le 0\f$
      \sa TRNG::error
    */
    double normal_dist(const double sigma=1.0, const double mu=0.0) {
      double s, t1, t2;
      if (sigma<=0.0)
	throw error("negative or zero standard deviation in TRNG::RNG::normal_dist");
      do {
	t1=uniformoo(-1.0, 1.0);
	t2=uniformoo(-1.0, 1.0);
	s=t1*t1+t2*t2;
      } while (s>=1.0);
      s=std::sqrt(-2.0*std::log(s)/s);
      return t1*s*sigma+mu;
    }

    //! Pseudo random number
    /*!
      A exponential distributed random variate has a probability density
      \f[
      p(x)=
      \begin{cases}
      \frac{1}{\mu}\mathrm{e}^{-\frac{x}{\mu}} & x\ge 0\,,\qquad \mu>0 \\
      0 & \text{else}
      \end{cases}\,.
      \f]
      \latexonly
      The transformation method is discribed in \cite{Knuth1981}. 
      \endlatexonly
      One random number is used to get one random number with exponential 
      distribution.
      \return a exponential distributed random number with mean \f$\mu\f$
      \param mu mean \f$\mu\f$
      \exception error if \f$\mu\le 0\f$
      \sa TRNG::error
    */
    double exp_dist(const double mu=1.0) {
      if (mu<=0.0)
	throw error("negative or zero parameter in TRNG::RNG::exp_dist");
      double t=uniformoc();
      return -mu*std::log(t);
    }

    //! Pseudo random number
    /*!
      The two-sided exponential probability distribution is
      \f[
      p(x)=\dfrac{1}{2a}\mathrm{e}^{-|x|/a}\,,\qquad a>0\,.
      \f]
      It is also known as the Laplace distribution. The is implementation 
      adopted from the GNU scientific library and uses a simple 
      transformation method. One random number is used to get one random
      number with Laplace distribution.
      \return a pseudo random number with probability density \f$p(x)\f$
      \param a parameter \f$a\f$, \f$a>0\f$
      \exception error if \f$a\le0\f$
      \sa TRNG::error
    */
    double laplace_dist(const double a=1.0) {
      if (a<=0.0)
	throw error("parameter less or equal zero in TRNG::RNG::laplace_dist");
      double u;
      do {
	u=uniformoo(-1.0, 1.0);
      } while (u==0.0);
      return u<0.0 ? a*std::log(-u) : -a*std::log(u);
    }

    //! Pseudo random number
    /*!
      This method generates a pseudo randeom number with a tent shaped
      probability distribution.
      \f[
      p(x)=
      \begin{cases}
      \dfrac{x+a}{a^2} & -a\le x\le 0 \\[1.5ex]
      \dfrac{a-x}{a^2} & 0\le x\le a \\
      0 & \text{else}
      \end{cases}\,,
      \qquad\text{with }a>0
      \f]
      The implementation uses a simple transformation method. One random 
      number is used to get one random number with a tent shaped
      distribution.
      \return a pseudo random number with probability density \f$p(x)\f$
      \param a parameter \f$a\f$, \f$a>0\f$
      \exception error if \f$a\le0\f$
      \sa TRNG::error
    */
    double tent_dist(const double a=1.0) {
      if (a<=0.0)
	throw error("parameter less or equal zero in TRNG::RNG::tent_dist");
      double y=uniformoo();
      if (y<0.5)
	return (-1.0+std::sqrt(2*y))*a;
      else
	return (1.0-std::sqrt(2.0-2.0*y))*a;
    }
    
    //! Pseudo random number
    /*!
      A gamma distributed random number has a probability density
      \f[
      p(x)=
      \begin{cases}
      \dfrac{1}{\Gamma(a)b^a} x^{a-1} \mathrm{e}^{-\frac{x}{b}} & x>0 \\
      0 & \text{else}
      \end{cases}\,.
      \f]
      The is implemetation adopted from the GNU scientific library.
      \latexonly
      See also \cite{Knuth1981}. 
      \endlatexonly
      \return a gamma distributed random number 
      \param a parameter \f$a\f$
      \param b parameter \f$b\f$
      \exception error if \f$a\le 0\f$ or \f$b\le 0\f$
      \sa TRNG::error
    */
    double Gamma_dist(const double a, const double b) {
      if (a<=0.0 || b<=0.0)
	throw error("parameter less or equal zero in TRNG::RNG::Gamma_dist");
      double a_int=std::floor(a);
      if (a_int==a) {
	// if a is an integer
	if (a<12) {
	  double prod=1.0;
	  for (int i=0; i<a; ++i)
	    prod*=uniformoo();
	  // Note: for 12 iterations we are safe against underflow, since
	  // the smallest positive random number is O(2^-32). This means
	  // the smallest possible product is 2^(-12*32) = 10^-116 which
	  // is within the range of double precision.
	  return -b*std::log(prod);
	} else {
	  // Works only if a>1, and is most efficient if a is large
	  // This algorithm, reported in Knuth, is attributed to Ahrens.  A
	  // faster one, we are told, can be found in: J. H. Ahrens and
	  // U. Dieter, Computing 12 (1974) 223-246.
	  double sqa, x, y, v;
	  sqa=std::sqrt(2.0*a-1.0);
	  do {
	    do {
	      const double pi=3.14159265358979324;
	      y=std::tan(pi*uniformco());
	      x=sqa*y+a-1.0;
	    }
	    while (x<=0.0);
	    v=uniformco();
	  }
	  while (v>(1.0+y*y)*std::exp((a-1.0)*log(x/(a-1))-sqa*y));
	  return b*x;
	}
      }
      if (a_int==0.0) {
	// if a<1
	// This is exercise 16 from Knuth; see page 135, and the solution is
	// on page 551.
	const double e=2.71828182844;
	double p, q, x, u, v;
	p=e/(a+e);
	do {
	  u=uniformco();
	  v=uniformoo();
	  if (u<p) {
	    x=std::exp((1.0/a)*std::log(v));
	    q=std::exp(-x);
	  } else {
	    x=1.0-std::log(v);
	    q=std::exp((a-1.0)*std::log(x));
	  }
	} while (uniformco()>=q);
	return b*x;
      }
      return Gamma_dist(a_int, b)+Gamma_dist(a-a_int, b); 
    }

    //! Pseudo random number
    /*!
      A Beta distributed random number has a probability density
      \f[
      p(x)=
      \begin{cases}
      \dfrac{\Gamma(a+b)}{\Gamma(a)\Gamma(b)}x^{a-1}(1-x)^{b-1} &
      0\le x\le 1 \\
      0 & \text{else}
      \end{cases}
      \f]
      \latexonly
      The method is discribed in \cite{Knuth1981} pp.~129-130.
      \endlatexonly
      \return a Beta distributed random number 
      \param a parameter \f$a\f$, \f$a>0\f$
      \param b parameter \f$b\f$, \f$b>0\f$
      \exception error if \f$a\le 0\f$ or \f$b\le 0\f$
      \sa TRNG::error
    */
    double Beta_dist(const double a, const double b) {
      if (a<=0.0 || b<=0.0)
	throw error("negative or zero parameter in TRNG::RNG::Beta_dist");
      double t1=Gamma_dist(a, 1.0);
      double t2=Gamma_dist(b, 1.0);
      return t1/(t1+t2);
    }

    //! pseudo random number
    /*!
      The \f$\chi^2\f$-distribution is just a special case of the 
      \f$\Gamma\f$-distribution with \f$a=\nu/2\f$ and \f$b=1\f$.
      \f[
      p(x)=\frac{\left(\frac{x}{2}\right)^{\frac{\nu}{2}-1}}
      {2\Gamma(\frac{\nu}{2})}
      \mathrm{e}^{-\frac{x}{2}}\,,\qquad x\le0
      \f]
      \latexonly
      The method is discribed in \cite{Knuth1981} p.~130.
      \endlatexonly
      \return a chi square distributed random number
      \param nu degrees of freedom \f$\nu\f$
      \exception error if \f$\nu<1\f$
      \sa TRNG::error
    */
    double chi_square_dist(const double nu) {
      if (nu<1.0)
	throw error("parameter less than one in TRNG::RNG::chi_square_dist");
      return 2.0*Gamma_dist(0.5*nu, 1.0);
    }

    //! Pseudo random number
    /*!
      The binomial distribution is a discrete distribution with
      \f[
      p(k)=\frac{n!}{k!(n-k)!}p^k(1-p)^{n-k}\,,\qquad 0\le k\le n\,.
      \f]
      \latexonly
      The method is discribed in \cite{Knuth1981} p.~131. 
      \endlatexonly 
      The implemetation is adopted from the GNU scientific library.
      \return a binomial distributed pseudo random number
      \param n number of trails \f$n\f$
      \param p probability \f$p\f$ in each trail
      \exception error if \f$p\le 0\f$ or \f$p>1\f$ or \f$n\le 0\f$
      \sa TRNG::error
    */
    long binomial_dist(long n, double p=0.5) {
      if (p<=0.0 || p>1.0)
	throw error("probability <=0.0 or >1.0 in TRNG::RNG::binomial_dist");
      if (n<1l)
	throw error("less than one trail TRNG::RNG::binomial_dist");
      long i, a, b, k=0;
      while (n>12l) {  // This parameter is tunable
	double X;
	a=1l+(n/2l);
	b=1l+n-a;
	X=Beta_dist(static_cast<double>(a), static_cast<double>(b));
	if (X>=p) {
	  n=a-1l;
	  p/=X;
	} else {
	  k+=a;
	  n=b-1l;
	  p=(p-X)/(1l-X);
	}
      }
      for (i=0l; i<n; i++)
	if (uniformco()<p)
	  k++;
      return k;
    }

    //! Pseudo random number
    /*!
      The binomial distribution is a discrete distribution with
      \f[
      p(k)=\frac{n!}{k!(n-k)!}p^k(1-p)^{n-k}\,,\qquad 0\le k\le n\,.
      \f]
      This method's implementation uses a lookup table and is very fast
      if this method is called often with the same parameter set.
      \return a binomial distributed pseudo random number
      \param n number of trails \f$n\f$
      \param p probability \f$p\f$ in each trail
      \exception error if \f$p\le 0\f$ or \f$p>1\f$ or \f$n\le 0\f$
      \sa TRNG::error
    */
    long binomial_dist_tab(long n, double p=0.5) {
      if (p<=0.0 || p>1.0)
	throw error("probability <=0.0 or >1.0 in TRNG::RNG::binomial_dist_tab");
      if (n<1l)
	throw error("less than one trail TRNG::RNG::binomial_dist_tab");
      static long n_=0l;
      static double p_=0.0;
      static std::vector<double> prob;
      if (n!=n_ || p!=p_) {
	n_=n;
	p_=p;
	prob.resize(n_);
	// use
	// ( n )            (  n  )   n-k ( n )
	// (   ) = 1  and   (     ) = --- (   )
	// ( 0 )            ( k+1 )   k+1 ( k )
	double b=1.0;
	for (int k=0l; k<n_; ++k) {
	  prob[k]=b*std::pow(p_, static_cast<double>(k))*
	    std::pow(1.0-p_, static_cast<double>(n_-k));
	  if (k>0l)
	    prob[k]+=prob[k-1l];
	  b=std::floor(b*static_cast<double>(n_-k)/
		       static_cast<double>(k+1l)+0.5); 
	}
      }
      return find_interval(prob, uniformco());
    }

    //! Pseudo random number
    /*!
      Student's \f$t\f$-distribution with \f$\nu\f$ degrees of freedom is 
      defined as 
      \f[
      p(x)=\frac{\Gamma\left(\frac{\nu-1}{2}\right)}
      {\sqrt{\nu\pi}\Gamma\left(\frac{\nu}{2}\right)}
      \left(1+\frac{x^2}{\nu}\right)^{-\frac{\nu+1}{2}}\,.
      \f]
      This method's implementation is adopted from the GNU scientific 
      library\latexonly, see also \cite{Knuth1981}\endlatexonly.
      \return a random number with Student's-\f$t\f$ distribution with 
      \f$\nu\f$ degrees of freedom
      \param nu degrees of freedom, \f$\nu\,>\,0\f$
      \exception error if \f$\nu\le 0\f$
      \sa TRNG::error
    */
    double Student_t_dist(const double nu) {
      if (nu<=0.0)
	throw error("parameter less than or equal zero in TRNG::RNG::Student_t_dist");
      if (nu<=2.0) 
	return normal_dist()/std::sqrt(chi_square_dist(nu)/nu);
      else {
	double y1, y2, z;
	do {
	  y1=normal_dist();
	  y2=exp_dist(1.0/(0.5*nu-1.0));
	  z=y1*y1/(nu-2.0);
	} while (1.0-z<0.0 || std::exp(-y2-z)>(1.0-z));
	// Note that there is a typo in Knuth's formula, the line below
	// is taken from the original paper of Marsaglia, Mathematics of
	// Computation, 34 (1980), p 234-256 
	return y1/std::sqrt((1.0-2.0/nu)*(1.0-z));
      }
    }

    //! Pseudo random number
    /*!
      The probability distribution for Poisson variates is
      \f[
      p(k)=\frac{\mu^k}{k!}\mathrm{e}^{-\mu}\,,\qquad k\ge 0\,.
      \f]
      This method's implementation is adopted from the GNU scientific 
      library\latexonly, see also \cite{Knuth1981}\endlatexonly.
      \return a poisson distributed pseudo random number
      \param mu mean \f$\mu\f$
      \exception error if \f$\mu \le 0\f$
      \sa TRNG::error
    */
    long poisson_dist(double mu=1.0) {
      if (mu<=0)
	throw error("parameter less than 0 in TRNG::RNG::poisson_dist");
      double emu;
      double prod=1.0;
      long k=0;  
      while (mu>10.0) {
	long m=static_cast<long>(mu*(7.0/8.0));
	double X=Gamma_dist(static_cast<long>(m), 1.0);
	if (X>=mu) {
	  return k+binomial_dist(m-1l, mu/X);
	} else {
	  k+=m;
	  mu-=X; 
	}
      }
      // This following method works well when mu is small
      emu=std::exp(-mu);
      do {
	prod*=uniformco();
	k++;
      } while (prod>emu);
      return k-1l;
    }

    //! Pseudo random number
    /*!
      The geometric probability distribution is
      \f[
      p(k)=q(1-q)^{k-1}\,,\qquad k\ge 1\,.
      \f]
      This method's implementation is adopted from the GNU scientific 
      library\latexonly, see also \cite{Knuth1981}\endlatexonly.
      \return a geometric distributed pseudo random number
      \param q probability \f$q\f$
      \exception error if \f$q\le 0\f$ or \f$q>1\f$
      \sa TRNG::error
    */
    long geometric_dist(double q) {
      if (q<=0.0 ||q>1.0)
	throw error("parameter out of range in TRNG::RNG::geometric_dist");
      if (q==1.0)
	return 1l;
      return static_cast<long>
	(std::log(uniformoo())/std::log(1.0-q))+1l;
    }

    //! Pseudo random number
    /*!
      Returns a random number calculated by the rejection method. Assume
      your desired probability distribution is \f$p(x)=\frac{3}{4}(1-x^2)\f$
      for \f$x\in [-1, 1]\f$. Write a class that calculates this probability 
      distribution
\verbatim
class p {
public:
  double operator()(double x) {
    return 0.75*(1.0-x*x);
  }
};
\endverbatim
      and use
\verbatim
my_rng.rejection(p(), -1.0, 1.0, 0.75)
\endverbatim
      to generate a pseudo random number with probability distribution 
      \f$p(x)\f$.
      \return random number
      \param p function object, a function describing the probability density in the range between \f$a_1\f$ and \f$a_2\f$
      \param a1 lower bound \f$a_1\f$
      \param a2 upper bound \f$a_2\f$
      \param p_max maximum of the probability function \f$p(x)\f$ for 
      \f$x\in [a_1, a_2]\f$
    */
    template<class t_function>
      double rejection(t_function p, double a1, double a2, double p_max)  {
      double t1, t2;
      do {
	t1=uniformco(a1, a2);
	t2=uniformco(0.0, p_max);
      } while (p(t1)<t2);
      return t1;
    }

    //! Pseudo random number
    /*!
      This method can be used to generate discrete random variates with
      an arbitrary probability distribution. The algorithm is \f$O(\ln n)\f$.
      \latexonly
      If you need a faster algorithm see \cite{Walker1977}.
      \endlatexonly
      \return random number \f$k\f$, \f$0\le k \le n-1\f$
      \param p \f$n\f$ dimensional commulative probability vector \f$p_0\f$, 
      \f$p_1,\ldots\f$ \f$p_{n-1}\f$, with \f$p_i\,<\,p_{i+1}\f$ and \f$p_{n-1}=1\f$
      \exception error if vector empty
      \sa TRNG::error
    */
    long discrete_dist(const std::vector<double> p) {
      if (p.empty())
	throw error("empty vector in TRNG::discrete_dist");
      double x=uniformco();
      if (x<p[0])
	return 0l;
      long i1=0l;
      long i2=p.size()-1l;
      while (i2-i1>1l) {
	long i3=(i2+i1)/2;
	if (x<=p[i3])
	  i2=i3;
	else
	  i1=i3;
      }
      return i2;
    }

    //! Pseudo random vector
    /*!
      This method calculates a unit vector with a uniform distributed 
      direction in two dimensions. For a vector uniform distributed inside
      the unit citcle multiply the vector with \f$\sqrt{u}\f$ where \f$u\f$ is
      uniforum distributed in \f$[0,1)\f$. The vector is stored in a structure
      #vector2d.
      \return a two dimensional unit vector 
    */
    vector2d spherical2d(void) {
      double r2, t1, t2, t12, t22;
      vector2d vec;
      do {
	t1=2.0*uniformco()-1.0;
	t2=uniformco();
	t12=t1*t1;
	t22=t2*t2;
	r2=t12+t22;
      } while (r2>1.0);
      vec.x1=2.0*t1*t2/r2;
      vec.x2=(t12-t22)/r2;
      return vec;
    }

    //! Pseudo random vector
    /*!
      This method calculates a unit vector with a uniform distributed 
      direction in three dimensions. For a vector uniform distributed 
      inside the unit shere multiply the vector with \f$\sqrt[3]{u}\f$ where 
      \f$u\f$ is uniforum distributed in \f$[0,1)\f$. The vector is stored in 
      a structure #vector3d.
      \return a three dimensional unit vector 
    */
    vector3d spherical3d(void)  {
      double q, r2, t1, t2;
      vector3d vec;
      do {
	t1=2.0*uniformco()-1.0;
	t2=2.0*uniformco()-1.0;
	r2=t1*t1+t2*t2;
      } while (r2>1.0);
      q=2.0*sqrt(1.0-r2);
      vec.x1=t1*q;
      vec.x2=t2*q;
      vec.x3=1-2*r2;
      return vec;
    }

    //! Pseudo random vector
    /*!
      This method calculates a unit vector with a uniform distributed 
      direction in four dimensions. For a vector uniform distributed inside 
      the unit hyper-shere multiply the vector with \f$\sqrt[4]{u}\f$ where 
      \f$u\f$ is uniforum distributed in \f$[0,1)\f$. The vector is stored in 
      a structure #vector4d.
      \return a four dimensional unit vector 
    */
    vector4d spherical4d(void)  {
      double q, r21, r22, t1, t2, t3, t4;
      vector4d vec;
      do {
	t1=2.0*uniformco()-1.0;
	t2=2.0*uniformco()-1.0;
	r21=t1*t1+t2*t2;
      } while (r21>1.0);
      do {
	t3=2.0*uniformco()-1.0;
	t4=2.0*uniformco()-1.0;
	r22=t3*t3+t4*t4;
      } while (r22>1.0);
      q=sqrt((1.0-r21)/r22);
      vec.x1=t1;
      vec.x2=t2;
      vec.x3=t3*q;
      vec.x4=t4*q;
      return(vec);
    }

    //! Sequence splitting
    /*!
      The pseudo random number generator's sequence is splitted into \f$s\f$ 
      sequences using the leapfrog method. Sequence number \f$n\f$ is selected.
      \f$0\le n <s\f$
      \param s number of sequences
      \param n selected sequence
      \exception error if \f$s<1\f$ or \f$n\ge s\f$ or \f$n<0\f$
      \sa TRNG::error
    */
    void split(long s, long n) {
      derived().split(s, n);
    }

    //! Sequence splitting
    /*!
      The pseudo random number generator jumps \f$s\f$ steps ahead.
      \param s determines the jump size
      \exception error if \f$s<0\f$
      \sa TRNG::error
    */
    void jump(long long s) {
      if (s<0ll)
	throw error("invalid argument for TRNG::RNG::jump");
      long i=0l;
      while (s>0ll) {
	if (s%2==1)
	  jump2(i);
	++i;
	s>>=1;
      }
    }

    //! Sequence splitting
    /*!
      The pseudo random number generator jumps \f$n\cdot s\f$ steps ahead.
      \param s determines the jump size
      \exception error if \f$s<0\f$ or \f$n<0\f$ 
      \param s paremeter \f$s\f$
      \param n paremeter \f$n\f$
      \sa TRNG::error
    */
    void jump(long long s, long n) {
      if (s<0ll || n<0l)
	throw error("invalid argument for TRNG::RNG::jump");
      while (n>0l) {
	jump(s);
	--n;
      }
    }

    //! Sequence splitting
    /*!
      The pseudo random number generator jumps \f$2^s\f$ steps ahead.
      \param s determines the jump size
      \exception error if \f$s<0\f$
      \sa TRNG::error
    */
    void jump2(long s) {
      derived().jump2(s);
    }

    //! Sequence splitting
    /*!
      The pseudo random number generator jumps \f$n\cdot 2^s\f$ steps ahead.
      \param s determines the jump size
      \param n determines the jump size
      \exception if \f$s<0\f$ or \f$n<0\f$
      \sa TRNG::error
    */
    void jump2(long s, long n) {
      if (s<0l || n<0l)
	throw error("invalid argument for TRNG::RNG::jump2");
      while (n>0l) {
	if (n%2l==1l)
	  jump2(s);
	s+=1l;
	n>>=1;
      }
    }

    //! Status saving
    /*!
      The status of the pseudo random number generator is saved into a
      vector.
      \param s reference to a vector of long
    */
    void save_status(std::vector<long> &s) {
      derived().save_status(s);
    }

    //! Status restoring
    /*!
      The status of the pseudo random number generator is restored from a
      vector.
      \param s reference to a vector of long
    */
    void load_status(const std::vector<long> &s) {
      derived().load_status(s);
    }

//     //! generic copy constructor
//     /*!
//       This is the random number generator copy constructor. Useful if
//       some generators with the same sequence of random numbers are needed.
//      */
//     RNG<RNG_type> & RNG<RNG_type>::operator=(RNG<RNG_type> &other) {
//       if (this!=&other) {
// 	std::vector<long> s;
// 	other.save_status(s);
// 	load_status(s);
//       }
//       return *this;
//     }
    
  };

  // --------------------------------------------------------------------

  class generic_MLCG; // forward declaration
  //! Linear congruential generator
  /*!
    This class implements a simple linear congruential pseudo random number 
    generator with a prime modulus. The pseudo random number\f$r_i\f$ is 
    calculated by
    \f[
    \begin{split}
    r_{i} & =a\cdot r_{i-1}\mod m \,.
    \end{split}
    \f]
    The modulus \f$m\f$ has to be a prime smaller than \f$2^{31}\f$ and \f$a\f$ 
    a generating element of the multiplicative group modulo \f$m\f$ to generate 
    a maxmal length period \f$m-1\f$. The behaviour of method rand() is different 
    from other generators, it never returns 0. For this reason this generator is for 
    internal use only. 
    \author Heiko Bauke
  */
  class generic_MLCG : public RNG<generic_MLCG> {
    // generic class for multiplicative linear congruential 
    // random number generators
  private:
    long a, a_save;
    long modulus, modulus_save;
    long r;
    void backward(void);
  public:
    static const RNG_type type=generic_MLCG_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);    
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<generic_MLCG> & operator=(RNG<generic_MLCG> &other);
    generic_MLCG(long a_=16807l, long modulus_=2147483647l, long seed_=0l);
  };

  inline long generic_MLCG::rand(void) {
    if (modulus==2147483647l) {
      unsigned long long t=
	static_cast<unsigned long long>(a)*static_cast<unsigned long long>(r);
      t=(t&0x7fffffffull)+(t>>31);
      r=(t>=2147483647ul) ?  (t-2147483647ul) : t;
      return r;
    } else {
      r=static_cast<long>
	((static_cast<long long>(a)*static_cast<long long>(r))%modulus);
      return r;
    }
  }

  // -------------------------------------------------------------------

  //! Parameter set for linear congruential generators
  /*!
    This class implements the parameter set for linear congruential 
    generators with prime modulus and is used by class ParkMiller. 
    See also ParkMiller_param_sets.
    \f[
    r_i=a\cdot r_{i-1} \mod m
    \f]
   */
  class ParkMiller_param {
  public:
    const long a;  //!< multiplier
    const long modulus;  //!< prime modulus
    /*!
      The parameter of the constructor specify the multiplier and the 
      prime modulus.
      \param a_ multiplier \f$a\f$ 
      \param modulus_ prime modulus\f$m\f$ 
     */
    ParkMiller_param(long a_, long modulus_) :
      a(a_), modulus(modulus_) { };
  };

  //! Different parameter sets for random number generators of ParkMiller type.
  namespace ParkMiller_param_sets {
    //! random number generator by Park and Miller \latexonly\cite{ParkMiller1988}\endlatexonly
    const ParkMiller_param mindstd=ParkMiller_param(16807l, 2147483647l);
    //! random number generator by Fishman \latexonly\cite{FishmanMoore1986}\endlatexonly
    const ParkMiller_param Fishman1=ParkMiller_param(742938285l, 2147483647l);
    //! random number generator by Fishman \latexonly\cite{FishmanMoore1986}\endlatexonly
    const ParkMiller_param Fishman2=ParkMiller_param(950706376l, 2147483647l);
    //! random number generator by Fishman \latexonly\cite{FishmanMoore1986}\endlatexonly
    const ParkMiller_param Fishman3=ParkMiller_param(1226874159l, 2147483647l);
    //! random number generator by Fishman \latexonly\cite{FishmanMoore1986}\endlatexonly
    const ParkMiller_param Fishman4=ParkMiller_param(62089911l, 2147483647l);
    //! random number generator by Fishman \latexonly\cite{FishmanMoore1986}\endlatexonly
    const ParkMiller_param Fishman5=ParkMiller_param(1343714438l, 2147483647l);
  }

  class ParkMiller;  // forward declaration
  //! Linear congruential generator
  /*!
    This class implements a simple multiplicative linear congruential pseudo 
    random number generator with a prime modulus in the form
    \f[
    \begin{split}
    q_{i} & =a\cdot q_{i-1}\mod m \\
    r_{i} & = q_{i}-1\,. \\
    \end{split}
    \f]
    \f$r_i\f$ is the actual pseudo random number. The modulus \f$m\f$ has 
    to be a prime smaller than \f$2^{31}\f$ and \f$a\f$ a generating element 
    of the multiplicative group modulo \f$m\f$ to generate a maxmal length 
    period.
    \author Heiko Bauke
  */
  class ParkMiller : public RNG<ParkMiller> {
  private:
    generic_MLCG R;
  public:
    static const RNG_type type=ParkMiller_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<ParkMiller> & operator=(RNG<ParkMiller> &other); 
    //! constructor
    /*! 
      The default values of the constructor implement a modified random 
      number generator proposed by Park and Miller. This generator has a 
      period of
      \f$2^{31}-2\approx 2^{31}\approx 2.15\cdot 10^9\f$. 
      \latexonly See also \cite{ParkMiller1988}. \endlatexonly
      \param param parameter set 
      \param seed_ default seed
    */
    ParkMiller(ParkMiller_param param=ParkMiller_param_sets::mindstd,
	       long seed_=0l);
  };

  inline long ParkMiller::rand(void) {
    return R.rand()-1l;
  }
  
  // -------------------------------------------------------------------

  //! Parameter set for linear congruential generators
  /*!
    This class implements the parameter set for linear congruential 
    generators with modulus \f$2^{32}\f$ and is used by class LCG32. 
    See also LCG32_param_sets.
    \f[
    r_i=a\cdot r_{i-1} + b \mod 2^{32}
    \f]
   */
  class LCG32_param {
  public:
    const unsigned long a;  //!< multiplier
    const unsigned long b;  //!< additive constant
    /*!
      The parameter of the constructor specify the multiplier and the 
      additive constant.
      \param a_ multiplier \f$a\f$ 
      \param b_ additive constant \f$b\f$ 
     */
    LCG32_param(unsigned long a_, unsigned long b_) :
      a(a_), b(b_) { };
  };

  //! Different parameter sets for random number generators of LCG32 type.
  namespace LCG32_param_sets {
    //! random number generator used by VAX
    const LCG32_param VAX=LCG32_param(69069ul, 1ul);
    //! Marsaglia's Super-Duper
    const LCG32_param SuperDuper=LCG32_param(69069ul, 0ul);
    //! random number generator used in Derive
    const LCG32_param Derive=LCG32_param(3141592653ul, 1ul);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const LCG32_param LEcuyer1=LCG32_param(2891336453ul, 1ul);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const LCG32_param LEcuyer2=LCG32_param(29943829ul, 1ul);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const LCG32_param LEcuyer3=LCG32_param(32310901ul, 1ul);
  }

  class LCG32;  // forward declaration;
  //! Linear congruential generator
  /*!
    This class implements a simple linear congruential pseudo random number 
    generator with a %power of two modulus in the form
    \f[
    \begin{split}
    q_{i} & = a\cdot q_{i-1}+b\mod 2^{32} \\
    r_{i} & = \lfloor q_{i}/2 \rfloor \,. \\
    \end{split}
    \f]
    \f$r_i\f$ is the actual pseudo random number. To get a full period of 
    \f$2^{32}\f$ \f$a\f$ and \f$b\f$ have to be choosen that 
    \f$a\equiv 1\mod 4\f$ and \f$b\f$ is odd. If \f$b=0\f$ the maximal 
    period is \f$2^{30}\f$. A generator with \f$b=0\f$ has a full period if
    \f$\pm3=a\mod 8\f$.
    \author Heiko Bauke
  */
  class LCG32 : public RNG<LCG32> {
  private:
    unsigned long a, a_save;
    unsigned long b, b_save;
    unsigned long r;
    void backward(void);
  public:
    static const RNG_type type=LCG32_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);    
    long rand(void);    
    void split(long s, long n);    
    void jump2(long s);    
    void save_status(std::vector<long> &s);    
    void load_status(const std::vector<long> &s);
    RNG<LCG32> & operator=(RNG<LCG32> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator used on VAX. This generator has a period of 
      \f$2^{32}\approx 4.29\cdot 10^9\f$. This generator is just a toy.
      Its period is too short. See also LCG32_param_sets.
      \param param parameter set 
      \param seed_ default seed
    */
    LCG32(LCG32_param param=LCG32_param_sets::VAX, long seed_=0l);
  };

  inline long LCG32::rand(void) {
    r=a*r+b;
#if ULONG_MAX == 0xfffffffful
#else
    r&=0xfffffffful;
#endif
    return static_cast<long>(r>>1);
  }

  // -------------------------------------------------------------------

  //! Parameter set for linear congruential generators
  /*!
    This class implements the parameter set for linear congruential 
    generators with modulus \f$2^{64}\f$ and is used by class LCG64. 
    See also LCG64_param_sets.
    \f[
    r_i=a\cdot r_{i-1} + b \mod 2^{64}
    \f]
   */
  class LCG64_param {
  public:
    const unsigned long long a;  //!< multiplier
    const unsigned long long b;  //!< additive constant
    /*!
      The parameter of the constructor specify the multiplier and the 
      additive constant.
      \param a_ multiplier \f$a\f$ 
      \param b_ additive constant \f$b\f$ 
     */
    LCG64_param(unsigned long long a_, unsigned long long b_) :
      a(a_), b(b_) { };
  };

  //! Different parameter sets for random number generators of LCG64 type.
  namespace LCG64_param_sets {
    //! %TRNG default
    const LCG64_param TRNG=LCG64_param(18145460002477866997ull, 1ull);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const LCG64_param LEcuyer1=LCG64_param(2862933555777941757ull, 1ull);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const LCG64_param LEcuyer2=LCG64_param(3202034522624059733ull, 1ull);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const LCG64_param LEcuyer3=LCG64_param(3935559000370003845ull, 1ull);
  }

  class LCG64;  // forward declaration
  //! linear congruential generator
  /*!
    This class implements a simple linear congruential pseudo random number 
    generator with a %power of two modulus in the form
    \f[
    \begin{split}
    q_{i} & = a\cdot q_{i-1}+b\mod 2^{64} \\
    r_{i} & = \lfloor q_{i}/2^{33} \rfloor \,. \\
    \end{split}
    \f]
    \f$r_i\f$ is the actual pseudo random number. To get a full period of 
    \f$2^{64}\f$ \f$a\f$ and \f$b\f$ have to be choosen that \f$a\equiv 1\mod 4\f$ and 
    \f$b\f$ is odd.
    \author Heiko Bauke
   */
  class LCG64 : public RNG<LCG64> {
  private:
    unsigned long long a, a_save;
    unsigned long long b, b_save;
    unsigned long long r;
    void backward(void);
  public:
    static const RNG_type type=LCG64_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);    
    void save_status(std::vector<long> &s);    
    void load_status(const std::vector<long> &s);
    RNG<LCG64> & operator=(RNG<LCG64> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a=18\,145\,460\,002\,477\,866\,997\f$ and \f$b=1\f$.
      This generator has a period of \f$2^{64}\approx 1.84\cdot 10^{19}\f$.
      LCG64 is the quick and dirty generator in TRNG.
      \param param parameter set 
      \param seed_ default seed
    */
    LCG64(LCG64_param param=LCG64_param_sets::TRNG, long seed_=0l);
  };
  
  inline long LCG64::rand(void) {
    r=a*r+b;
    return static_cast<long>(r>>33);
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for multiple recursive
    generators with two coefficients and prime modulus and is used by 
    class MRG2. See also MRG2_param_sets.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}\mod m
    \f]
   */
  class MRG2_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long modulus;  //!< modulus \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param modulus_ modulus \f$m\f$ 
     */
    MRG2_param(long a1_, long a2_, long modulus_) :
      a1(a1_), a2(a2_), modulus(modulus_) { };
  };

  //! Different parameter sets for random number generators of MRG2 type.
  namespace MRG2_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG2_param LEcuyer1=MRG2_param(1498809829l, 1160990996l, 2147483647l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG2_param LEcuyer2=MRG2_param(46325l, 1084587l, 2147483647l);
  }

  class MRG2;  // forward declaration;
  //! multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    two with a prime modulus.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}\mod m
    \f]
    \author Heiko Bauke
  */
  class MRG2 : public RNG<MRG2> {
  private:
    long a1, a2, a1_save, a2_save;
    long r1, r2;
    long modulus, modulus_save;
    void backward(void);
  public:
    static const RNG_type type=MRG2_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);    
    RNG<MRG2> & operator=(RNG<MRG2> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=1\,498\,809\,829\f$, \f$a_2=1\,160\,990\,996\f$ 
      and \f$m=2^{31}-1\f$ 
      \latexonly as proposed in \cite{lEcuyer1993}\endlatexonly. 
      This generator has a period of 
      \f${2^{31}-1}^2-1\approx 2^{62}\approx 4.61\cdot 10^{18}\f$.
      \param param parameter set
      \param seed_ default seed
    */
    MRG2(MRG2_param param=MRG2_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long MRG2::rand(void) {
    unsigned long long t=
      static_cast<unsigned long long>(a1)*static_cast<unsigned long long>(r1)+
      static_cast<unsigned long long>(a2)*static_cast<unsigned long long>(r2);
    if (modulus==2147483647l) {
      t=(t&0x7fffffffull)+(t>>31);
      if (t>=2ull*2147483647ull)
	t-=2ull*2147483647ull;
      if (t>=2147483647ull)
	t-=2147483647ull;
    } else
      t%=modulus;
    r2=r1;  r1=t;
    return r1;
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for multiple recursive
    generators with three coefficients and prime modulus and is used by 
    class MRG3. See also MRG3_param_sets.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}+a_3\cdot r_{i-3}\mod m
    \f]
   */
  class MRG3_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long a3;       //!< multiplier \f$a_3\f$
    const long modulus;  //!< modulus \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param a3_ multiplier \f$a_3\f$ 
      \param modulus_ modulus \f$m\f$ 
     */
    MRG3_param(long a1_, long a2_, long a3_, long modulus_) :
      a1(a1_), a2(a2_), a3(a3_), modulus(modulus_) { };
  };

  //! Different parameter sets for random number generators of MRG3 type.
  namespace MRG3_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG3_param LEcuyer1=MRG3_param(2021422057l, 1826992351l, 1977753457l, 2147483647l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG3_param LEcuyer2=MRG3_param(1476728729l, 0l, 1155643113l, 2147483647l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG3_param LEcuyer3=MRG3_param(65338l, 0l, 64636l, 2147483647l);
  }

  class MRG3;  // forward declaration;
  //! multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    three with a prime modulus.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}+a_3\cdot r_{i-3}\mod m
    \f]
    \author Heiko Bauke
  */
  class MRG3 : public RNG<MRG3> {
  private:
    long a1, a2, a3, a1_save, a2_save, a3_save;
    long r1, r2, r3;
    long modulus, modulus_save;
    void backward(void);
  public:
    static const RNG_type type=MRG3_t;
    const char * name(void);
    void reset(void);    
    void seed(long s=0l);
    long rand(void);   
    void split(long s, long n);
    void jump2(long s);    
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<MRG3> & operator=(RNG<MRG3> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=2\,021\,422\,057\f$, \f$a_2=1\,826\,992\,351\f$,
      \f$a_3=1\,977\,753\,457\f$ and \f$m=2^{31}-1\f$ 
      \latexonly as proposed in \cite{lEcuyer1993}\endlatexonly. 
      This generator has a period of 
      \f${2^{31}-1}^3-1\approx 2^{93}\approx 9.90\cdot 10^{27}\f$.
      \param param parameter set
      \param seed_ default seed
    */
    MRG3(MRG3_param param=MRG3_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long MRG3::rand(void) {
    unsigned long long t=
      static_cast<unsigned long long>(a1)*static_cast<unsigned long long>(r1)+
      static_cast<unsigned long long>(a2)*static_cast<unsigned long long>(r2)+
      static_cast<unsigned long long>(a3)*static_cast<unsigned long long>(r3);
    if (modulus==2147483647l) {
      t=(t&0x7fffffffull)+(t>>31);
      if (t>=2ull*2147483647ull)
	t-=2ull*2147483647ull;
      if (t>=2147483647ull)
	t-=2147483647ull;
    } else
      t%=modulus;
    r3=r2;  r2=r1;  r1=t;
    return r1;
  }
    
  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for multiple recursive
    generators with four coefficients and prime modulus and is used by 
    class MRG4. See also MRG4_param_sets.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}+a_3\cdot r_{i-3}+a_4\cdot r_{i-4}\mod m
    \f]
   */
  class MRG4_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long a3;       //!< multiplier \f$a_3\f$
    const long a4;       //!< multiplier \f$a_4\f$
    const long modulus;  //!< modulus \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param a3_ multiplier \f$a_3\f$ 
      \param a4_ multiplier \f$a_4\f$ 
      \param modulus_ modulus \f$m\f$ 
     */
    MRG4_param(long a1_, long a2_, long a3_, long a4_, long modulus_) :
      a1(a1_), a2(a2_), a3(a3_), a4(a4_), modulus(modulus_) { };
  };

  //! Different parameter sets for random number generators of MRG4 type.
  namespace MRG4_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG4_param LEcuyer1=MRG4_param(2001982722l, 1412284257l, 1155380217l, 1668339922l, 2147483647l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG4_param LEcuyer2=MRG4_param(64886l, 0l, 0l, 64322l, 2147483647l);
  }

  class MRG4;  // forward declaration;
  //! multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    four with a prime modulus.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}+
    a_3\cdot r_{i-3}+a_4\cdot r_{i-4}\mod m
    \f]
    \author Heiko Bauke
  */
  class MRG4 : public RNG<MRG4> {
  private:
    long a1, a2, a3, a4, a1_save, a2_save, a3_save, a4_save;
    long r1, r2, r3, r4;
    long modulus, modulus_save;
    void backward(void);
  public:
    static const RNG_type type=MRG4_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<MRG4> & operator=(RNG<MRG4> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=2\,001\,982\,722\f$, \f$a_2=1\,412\,284\,257\f$,
      \f$a_3=1\,155\,380\,217\f$, \f$a_4=1\,668\,339\,922\f$ and 
      \f$m=2^{31}-1\f$ 
      \latexonly as proposed by \cite{lEcuyer1993}\endlatexonly.
      This generator has a period of 
      \f${2^{31}-1}^4-1\approx 2^{124}\approx 2.13\cdot 10^{37}\f$.
      \param param parameter set
      \param seed_ default seed
    */
    MRG4(MRG4_param param=MRG4_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long MRG4::rand(void) {
    unsigned long long t=
      static_cast<unsigned long long>(a1)*static_cast<unsigned long long>(r1)+
      static_cast<unsigned long long>(a2)*static_cast<unsigned long long>(r2)+
      static_cast<unsigned long long>(a3)*static_cast<unsigned long long>(r3)+
      static_cast<unsigned long long>(a4)*static_cast<unsigned long long>(r4);
    if (modulus==2147483647l) {
      t=(t&0x7fffffffull)+(t>>31);
      if (t>=4ull*2147483647ull)
	t-=4ull*2147483647ull;
      if (t>=2ull*2147483647ull)
	t-=2ull*2147483647ull;
      if (t>=2147483647ull)
	t-=2147483647ull;
    } else
      t%=modulus;
    r4=r3;  r3=r2;  r2=r1;  r1=t;
    return r1;
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for multiple recursive
    generators with five coefficients and prime modulus and is used by 
    class MRG5. See also MRG5_param_sets.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}+
    a_3\cdot r_{i-3}+a_4\cdot r_{i-4}+a_5\cdot r_{i-5}\mod m
    \f]
   */
  class MRG5_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long a3;       //!< multiplier \f$a_3\f$
    const long a4;       //!< multiplier \f$a_4\f$
    const long a5;       //!< multiplier \f$a_5\f$
    const long modulus;  //!< modulus \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param a3_ multiplier \f$a_3\f$ 
      \param a4_ multiplier \f$a_4\f$ 
      \param a5_ multiplier \f$a_5\f$ 
      \param modulus_ modulus \f$m\f$ 
     */
    MRG5_param(long a1_, long a2_, long a3_, long a4_, long a5_, long modulus_) :
      a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_), modulus(modulus_) { };
  };

  //! Different parameter sets for random number generators of MRG5 type.
  namespace MRG5_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const MRG5_param LEcuyer1=MRG5_param(177786l, 0l, 0l, 0l, 64654l, 2147483647l);
  }

  class MRG5;  // forward declaration;
  //! multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    four with a prime modulus.
    \f[
    r_{i} = a_1\cdot r_{i-1}+a_2\cdot r_{i-2}+
    a_3\cdot r_{i-3}+a_4\cdot r_{i-4}+a_5\cdot r_{i-5}\mod m
    \f]
    \author Heiko Bauke
  */
  class MRG5 : public RNG<MRG5> {
  private:
    long a1, a2, a3, a4, a5, a1_save, a2_save, a3_save, a4_save, a5_save;
    long r1, r2, r3, r4, r5;
    long modulus, modulus_save;
    unsigned long long m2_2;
    void backward(void);
  public:
    static const RNG_type type=MRG5_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<MRG5> & operator=(RNG<MRG5> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=107\,374\,182\f$, \f$a_2=0\f$,
      \f$a_3=0\f$, \f$a_4=0\f$, \f$a_5=104\,480\f$ and 
      \f$m=2^{31}-1\f$ 
      \latexonly as proposed by \cite{lEcuyer1993}\endlatexonly.
      This generator has a period of 
      \f${2^{31}-1}^5-1\approx 2^{155}\approx 4.57\cdot 10^{46}\f$.
      \param param parameter set
      \param seed_ default seed
    */
    MRG5(MRG5_param param=MRG5_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long MRG5::rand(void) {
    unsigned long long t=
      static_cast<unsigned long long>(a1)*static_cast<unsigned long long>(r1)+
      static_cast<unsigned long long>(a2)*static_cast<unsigned long long>(r2)+
      static_cast<unsigned long long>(a3)*static_cast<unsigned long long>(r3)+
      static_cast<unsigned long long>(a4)*static_cast<unsigned long long>(r4);
    if (t>=m2_2)
      t-=m2_2;
    t+=static_cast<unsigned long long>(a5)*static_cast<unsigned long long>(r5);
    if (modulus==2147483647l) {
      t=(t&0x7fffffffull)+(t>>31);
      if (t>=2ull*2147483647ull)
	t-=2ull*2147483647ull;
      if (t>=2147483647ull)
	t-=2147483647ull;
    } else
      t%=modulus;
    r5=r4;  r4=r3;  r3=r2;  r2=r1;  r1=t;
    return r1;
  }

  // -------------------------------------------------------------------

  //! Parameter set for linear congruential generators
  /*!
    This class implements the parameter set for linear congruential 
    generators with modulus \f$2^{64}\f$ and is used by class YARNLCG64. 
    See also YARNLCG64_param_sets.
    \f[
    r_i=a\cdot r_{i-1} + b \mod 2^{64}
    \f]
   */
  class YARNLCG64_param {
  public:
    const unsigned long long a;  //!< multiplier
    const unsigned long long b;  //!< additive constant
    const long g;                //!< generating element modulo \f$2^{31}-1\f$
    /*!
      The parameter of the constructor specify the multiplier, the 
      additive constant and the generating element modulo \f$2^{31}-1\f$.
      \param a_ multiplier \f$a\f$ 
      \param b_ additive constant \f$b\f$ 
      \param g_ generating element \f$g\f$ modulo \f$2^{31}-1\f$
     */
    YARNLCG64_param(unsigned long long a_, unsigned long long b_, long g_) :
      a(a_), b(b_), g(g_) { };
  };

  //! Different parameter sets for random number generators of YARNLCG64 type.
  namespace YARNLCG64_param_sets {
    //! %TRNG default
    const YARNLCG64_param TRNG=YARNLCG64_param(18145460002477866997ull, 1ull, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const YARNLCG64_param LEcuyer1=YARNLCG64_param(2862933555777941757ull, 1ull, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const YARNLCG64_param LEcuyer2=YARNLCG64_param(3202034522624059733ull, 1ull, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{LEcuyer1999b}\endlatexonly
    const YARNLCG64_param LEcuyer3=YARNLCG64_param(3935559000370003845ull, 1ull, 123567893l);
  }

  class YARNLCG64;  // forward declaration;

  //! YARN -- a modified linear recursive generator
  /*!
    This class implements a modified linear congruential pseudo random number 
    generator with a %power of two modulus of the form
    \f[
    \begin{split}
    s_{i} & = a\cdot s_{i-1}+b\mod 2^{64} \\
    q_{i} & = \lfloor s_{i}/2^{33} \rfloor \,. \\
    \end{split}
    \f]
    To get a full period of \f$2^{64}\f$ \f$a\f$ and \f$b\f$ have to be choosen
    that \f$a\equiv 1\mod 4\f$ and \f$b\f$ is odd.
    Linear structures are destroyed by the nonlinear bijective mapping
    \f[
    r_{i} = 
    \begin{cases}
    g^{q_{i}}\mod 2^{31}-1 & \text{if }q_{i}\ne 0 \wedge q_{i}\ne 2^{31}-1\\
    q_{i}                  & \text{else}  \\
    \end{cases}\,.
    \f]
    The parameter \f$g\f$ has to be a generating element of the multiplicative 
    group modulo \f$2^{31}-1\f$. 
    \author Heiko Bauke
   */
  class YARNLCG64 : public RNG<YARNLCG64> {
  private:
    LCG64 R;
    power g;
    long g_save;
  public:
    static const RNG_type type=YARNLCG64_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<YARNLCG64> & operator=(RNG<YARNLCG64> &other); 
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a=18\,145\,460\,002\,477\,866\,997\f$, \f$b=1\f$ and
      \f$g=123\,567\,893\f$.
      This generator has a period of \f$2^{64}\approx 1.84\cdot 10^{19}\f$.
      \param param parameter set
      \param seed_ default seed
    */
    YARNLCG64(YARNLCG64_param param=YARNLCG64_param_sets::TRNG, long seed_=0l);
  };

  inline long YARNLCG64::rand(void) {
    long t=R.rand();
    return (t==0 || t==2147483647l) ? t : g(t);
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for modified multiple recursive
    generators with two coefficients and prime modulus and is used by 
    class YARN2. See also YARN2_param_sets.
    \f[
    \begin{split}
    q_{i} & = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}\mod m \\
    r_{i} & = 
    \begin{cases}
    g^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases} \\
    \end{split}
    \f]
  */
  class YARN2_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long modulus;  //!< modulus \f$m\f$
    const long g;        //!< generating element \f$g\f$ modulo \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param modulus_ modulus \f$m\f$ 
      \param g_ generating element \f$g\f$ modulo \f$m\f$
    */
    YARN2_param(long a1_, long a2_, long modulus_, long g_) :
      a1(a1_), a2(a2_), modulus(modulus_), g(g_) { };
  };

  //! Different parameter sets for random number generators of YARN2 type.
  namespace YARN2_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN2_param LEcuyer1=YARN2_param(1498809829l, 1160990996l, 2147483647l, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN2_param LEcuyer2=YARN2_param(46325l, 1084587l, 2147483647l, 123567893l);
  }
  
  class YARN2;  // forward declaration;
  //! YARN -- a modified multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    two with a prime modulus.     
    \f[
    q_{i} = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}\mod m
    \f]
    Linear structures are destroyed by the nonlinear bijective mapping
    \f[r_{i} = 
    \begin{cases}
    g^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases}\,.
    \f]
    The parameter \f$g\f$ has to be a generating element of the 
    multiplicative group modulo \f$m\f$.
    \author Heiko Bauke
   */
  class YARN2 : public RNG<YARN2> {
  private:
    MRG2 R;
    power g;
    long g_save;
  public:
    static const RNG_type type=YARN2_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<YARN2> & operator=(RNG<YARN2> &other); 
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=1\,498\,809\,829\f$, \f$a_2=1\,160\,990\,996\f$ 
      and \f$m=2^{31}-1\f$ 
      \latexonly as proposed in \cite{lEcuyer1993}\endlatexonly. 
      This generator has a period of 
      \f$(2^{31}-1)^2-1\approx 2^{62}\approx 4.61\cdot 10^{18}\f$.
      The parameter \f$g\f$ is chosen to be \f$123\,567\,893\f$.
      \param param parameter set
      \param seed_ default seed
    */
    YARN2(YARN2_param param=YARN2_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long YARN2::rand(void) {
    long t=R.rand();
    return (t==0) ? 0 : g(t);
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for modified multiple recursive
    generators with three coefficients and prime modulus and is used by 
    class YARN3. See also YARN3_param_sets.
    \f[
    \begin{split}
    q_{i} & = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}+a_3\cdot q_{i-3}\mod m \\
    r_{i} & = 
    \begin{cases}
    g^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases} \\
    \end{split}
    \f]
  */
  class YARN3_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long a3;       //!< multiplier \f$a_3\f$
    const long modulus;  //!< modulus \f$m\f$
    const long g;        //!< generating element \f$g\f$ modulo \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param a3_ multiplier \f$a_3\f$ 
      \param modulus_ modulus \f$m\f$ 
      \param g_ generating element \f$g\f$ modulo \f$m\f$
    */
    YARN3_param(long a1_, long a2_, long a3_, long modulus_, long g_) :
      a1(a1_), a2(a2_), a3(a3_), modulus(modulus_), g(g_) { };
  };

  //! Different parameter sets for random number generators of YARN3 type.
  namespace YARN3_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN3_param LEcuyer1=YARN3_param(2021422057l, 1826992351l, 1977753457l, 2147483647l, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN3_param LEcuyer2=YARN3_param(1476728729l, 0l, 1155643113l, 2147483647l, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN3_param LEcuyer3=YARN3_param(65338l, 0l, 64636l, 2147483647l, 123567893l);
  }

  class YARN3;  // forward declaration;
  //! YARN -- a modified multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    three with a prime modulus.     
    \f[
    q_{i} = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}+a_3\cdot q_{i-3}\mod m
    \f]
    Linear structures are destroyed by the nonlinear bijective mapping
    \f$r_{i} = 
    \begin{cases}
    b^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0         & \text{if }q_{i} =0 \\
    \end{cases}\,.
    \f$
    The parameter \f$b\f$ has to be a generating element of the 
    multiplicative group modulo \f$m\f$.
    \author Heiko Bauke
   */
  class YARN3 : public RNG<YARN3> {
  private:
    MRG3 R;
    power g;
    long g_save;
  public:
    static const RNG_type type=YARN3_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<YARN3> & operator=(RNG<YARN3> &other); 
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=2\,021\,422\,057\f$, \f$a_2=1\,826\,992\,351\f$,
      \f$a_3=1\,977\,753\,457\f$ and \f$m=2^{31}-1\f$ 
      \latexonly as proposed in \cite{lEcuyer1993}\endlatexonly. 
      This generator has a period of 
      \f$(2^{31}-1)^3-1\approx 2^{93}\approx 9.90\cdot 10^{27}\f$.
      The parameter \f$g\f$ is chosen to be \f$123\,567\,893\f$.
      \param param parameter set
      \param seed_ default seed
    */
    YARN3(YARN3_param param=YARN3_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long YARN3::rand(void) {
    long t=R.rand();
    return (t==0) ? 0 : g(t);
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for modified multiple recursive
    generators with four coefficients and prime modulus and is used by 
    class YARN4. See also YARN4_param_sets.
    \f[
    \begin{split}
    q_{i} & = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}+a_3\cdot q_{i-3}+
    a_4\cdot q_{i-4}\mod m \\
    r_{i} & = 
    \begin{cases}
    g^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases} \\
    \end{split}
    \f]
  */
  class YARN4_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long a3;       //!< multiplier \f$a_3\f$
    const long a4;       //!< multiplier \f$a_4\f$
    const long modulus;  //!< modulus \f$m\f$
    const long g;        //!< generating element \f$g\f$ modulo \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param a3_ multiplier \f$a_3\f$ 
      \param a4_ multiplier \f$a_4\f$ 
      \param modulus_ modulus \f$m\f$ 
      \param g_ generating element \f$g\f$ modulo \f$m\f$
    */
    YARN4_param(long a1_, long a2_, long a3_, long a4_, long modulus_, long g_) :
      a1(a1_), a2(a2_), a3(a3_), a4(a4_), modulus(modulus_), g(g_) { };
  };

  //! Different parameter sets for random number generators of YARN4 type.
  namespace YARN4_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN4_param LEcuyer1=YARN4_param(2001982722l, 1412284257l, 1155380217l, 1668339922l, 2147483647l, 123567893l);
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN4_param LEcuyer2=YARN4_param(64886l, 0l, 0l, 64322l, 2147483647l, 123567893l);
  }

  class YARN4;  // forward declaration;
  //! YARN -- a modified multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    four with a prime modulus.     
    \f[
    q_{i} = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}+
    a_3\cdot q_{i-3}+a_4\cdot q_{i-4}\mod m
    \f]
    Linear structures are destroyed by the nonlinear bijective mapping 
    \f$r_{i} = 
    \begin{cases}
    b^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases}\,.
    \f$
    The parameter \f$b\f$ has to be a generating element of the 
    multiplicative group modulo \f$m\f$.
    \author Heiko Bauke
   */
  class YARN4 : public RNG<YARN4> {
  private:
    MRG4 R;
    power g;
    long g_save;
  public:
    static const RNG_type type=YARN4_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<YARN4> & operator=(RNG<YARN4> &other); 
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=2\,001\,982\,722\f$, \f$a_2=1\,412\,284\,257\f$,
      \f$a_3=1\,155\,380\,217\f$, \f$a_4=1\,668\,339\,922\f$ and 
      \f$m=2^{31}-1\f$ 
      \latexonly as proposed by \cite{lEcuyer1993}\endlatexonly.
      This generator has a period of 
      \f$(2^{31}-1)^4-1\approx 2^{124}\approx 2.13\cdot 10^{37}\f$.
      The parameter \f$g\f$ is chosen to be \f$123\,567\,893\f$.
      \param param parameter set
      \param seed_ default seed
    */
    YARN4(YARN4_param param=YARN4_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long YARN4::rand(void) {
    long t=R.rand();
    return (t==0) ? 0 : g(t);
  }

  // -------------------------------------------------------------------

  //! Parameter set for multiple recursive generator
  /*!
    This class implements the parameter set for modified multiple recursive
    generators with five coefficients and prime modulus and is used by 
    class YARN5. See also YARN5_param_sets.
    \f[
    \begin{split}
    q_{i} & = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}+a_3\cdot q_{i-3}+
    a_4\cdot q_{i-4}+a_5\cdot q_{i-5}\mod m \\
    r_{i} & = 
    \begin{cases}
    g^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases} \\
    \end{split}
    \f]
  */
  class YARN5_param {
  public:
    const long a1;       //!< multiplier \f$a_1\f$
    const long a2;       //!< multiplier \f$a_2\f$
    const long a3;       //!< multiplier \f$a_3\f$
    const long a4;       //!< multiplier \f$a_4\f$
    const long a5;       //!< multiplier \f$a_5\f$
    const long modulus;  //!< modulus \f$m\f$
    const long g;        //!< generating element \f$g\f$ modulo \f$m\f$
    /*!
      The parameter of the constructor specify the multipliers and the 
      modulus.
      \param a1_ multiplier \f$a_1\f$ 
      \param a2_ multiplier \f$a_2\f$ 
      \param a3_ multiplier \f$a_3\f$ 
      \param a4_ multiplier \f$a_4\f$ 
      \param a5_ multiplier \f$a_5\f$ 
      \param modulus_ modulus \f$m\f$ 
      \param g_ generating element \f$g\f$ modulo \f$m\f$
    */
    YARN5_param(long a1_, long a2_, long a3_, long a4_, long a5_, long modulus_, long g_) :
      a1(a1_), a2(a2_), a3(a3_), a4(a4_), a5(a5_), modulus(modulus_), g(g_) { };
  };

  //! Different parameter sets for random number generators of YARN5 type.
  namespace YARN5_param_sets {
    //! random number generator proposed by L'Ecuyer \latexonly\cite{lEcuyer1993}\endlatexonly
    const YARN5_param LEcuyer1=YARN5_param(177786l, 0l, 0l, 0l, 64654l, 2147483647l, 123567893l);
  }

  class YARN5;  // forward declaration;
  //! YARN -- a modified multiple recursive generator
  /*!
    This multiple recursive generator uses a linear recurrence of order 
    five with a prime modulus.     
    \f[
    q_{i} = a_1\cdot q_{i-1}+a_2\cdot q_{i-2}+
    a_3\cdot q_{i-3}+a_4\cdot q_{i-4}+a_5\cdot q_{i-5}\mod m
    \f]
    Linear structures are destroyed by the nonlinear bijective mapping
    \f]r_{i} = 
    \begin{cases}
    b^{q_{i}}\mod m & \text{if }q_{i}\ne 0 \\
    0               & \text{if }q_{i} =0 \\
    \end{cases}\,.
    \f]
    The parameter \f$b\f$ has to be a generating element of the 
    multiplicative group modulo \f$m\f$.
    \author Heiko Bauke
   */
  class YARN5 : public RNG<YARN5> {
  private:
    MRG5 R;
    power g;
    long g_save;
  public:
    static const RNG_type type=YARN4_t;
    const char * name(void);    
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<YARN5> & operator=(RNG<YARN5> &other); 
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with \f$a_1=107\,374\,182\f$, \f$a_2=0\f$,
      \f$a_3=0\f$, \f$a_4=0\f$, \f$a_5=65\,322\f$ and 
      \f$m=2^{31}-1\f$ 
      \latexonly as proposed by \cite{lEcuyer1993}\endlatexonly.
      This generator has a period of 
      \f$(2^{31}-1)^5-1\approx 2^{155}\approx 4.57\cdot 10^{46}\f$.
      The parameter \f$g\f$ is chosen to be \f$123\,567\,893\f$.
      \param param parameter set
      \param seed_ default seed
    */
    YARN5(YARN5_param param=YARN5_param_sets::LEcuyer1, long seed_=0l);
  };

  inline long YARN5::rand(void) {
    long t=R.rand();
    return (t==0) ? 0 : g(t);
  }

  // -------------------------------------------------------------------

  //! Parameter set for combined linear congruential generators
  /*!
    This class implements the parameter set for combined linear congruential 
    generators with prime moduli and is used by class CLCG2. 
    See also CLCG2_param_sets.
   */
  class CLCG2_param {
  public:
    const long a1;        //!< multiplier of 1st generator
    const long modulus1;  //!< prime modulus of 1st generator
    const long a2;        //!< multiplier of 2nd generator
    const long modulus2;  //!< prime modulus of 2nd generator
    /*!
      The parameter of the constructor specify the multipliers and the 
      prime moduli.
      \param a1_ multiplier of 1st generator
      \param modulus1_ prime modulus of 1st generator
      \param a2_ multiplier of 2nd generator
      \param modulus2_ prime modulus of 2nd generator
     */
    CLCG2_param(long a1_, long modulus1_, long a2_, long modulus2_) :
      a1(a1_), modulus1(modulus1_),
      a2(a2_), modulus2(modulus2_) { };
  };

  //! Different parameter sets for random number generators of CLCG2 type.
  namespace CLCG2_param_sets {
    //! %TRNG default
    const CLCG2_param TRNG=CLCG2_param(376555083l, 2147482951l, 1028879659l, 2147482949l);
  }

  class CLCG2;  // forward declaration;
  //! combined generator
  /*!
    This is a combined linear congruential random number generator 
    with two generators. 
    \f[
    \begin{split}
    q_{1,i} & = a_1\cdot q_{1,i-1} \mod m_1 \\
    q_{2,i} & = a_2\cdot q_{2,i-1} \mod m_2 \\
    r_i     & = q_{1,i}+q_{2,i} \mod m_1-1
    \end{split}
    \f]
    \latexonly See also \cite{lEcuyer1988}.\endlatexonly
    \author Heiko Bauke
  */
  class CLCG2 : public RNG<CLCG2> {
  private:
    generic_MLCG R1;
    generic_MLCG R2;
    long modulus;
  public:
    static const RNG_type type=CLCG2_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);    
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);    
    void load_status(const std::vector<long> &s);    
    RNG<CLCG2> & operator=(RNG<CLCG2> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with
      \f[
      \begin{split}
      q_{1,i} & =    376\,555\,083\cdot q_{1,i-1} \mod 2\,147\,482\,951 \\
      q_{2,i} & = 1\,028\,879\,659\cdot q_{2,i-1} \mod 2\,147\,482\,949 \\
      r_i     & = q_{1,i}+q_{2,i} \mod 2\,147\,482\,950\,.
      \end{split}
      \f]
      This generator has a period of 
      \f$\frac{(2\,147\,482\,951-1)(2\,147\,482\,949-1)}{2}\approx 2^{61} 
      \approx 2.31\cdot 10^{18}\f$. The multipliers were found by an exhausive 
      search applying the spectral test in up to eight dimensions.
      \param param parameter set
      \param seed_ default seed
    */
    CLCG2(CLCG2_param param=CLCG2_param_sets::TRNG, long seed_=0l);
  };

  inline long CLCG2::rand(void) {
    unsigned long t=
      static_cast<unsigned long>(R1.rand())+
      static_cast<unsigned long>(R2.rand());
    return (t>=static_cast<unsigned long>(modulus)) ? (t-modulus) : t;
  }
  
  // -------------------------------------------------------------------

  //! Parameter set for combined linear congruential generators
  /*!
    This class implements the parameter set for combined linear congruential 
    generators with prime moduli and is used by class CLCG3. 
    See also CLCG3_param_sets.
   */
  class CLCG3_param {
  public:
    const long a1;        //!< multiplier of 1st generator
    const long modulus1;  //!< prime modulus of 1st generator
    const long a2;        //!< multiplier of 2nd generator
    const long modulus2;  //!< prime modulus of 2nd generator
    const long a3;        //!< multiplier of 3rd generator
    const long modulus3;  //!< prime modulus of 3rd generator
    /*!
      The parameter of the constructor specify the multipliers and the 
      prime moduli.
      \param a1_ multiplier of 1st generator
      \param modulus1_ prime modulus of 1st generator
      \param a2_ multiplier of 2nd generator
      \param modulus2_ prime modulus of 2nd generator
      \param a3_ multiplier of 3rd generator
      \param modulus3_ prime modulus of 3rd generator
     */
    CLCG3_param(long a1_, long modulus1_, long a2_, long modulus2_, long a3_, long modulus3_) :
      a1(a1_), modulus1(modulus1_),
      a2(a2_), modulus2(modulus2_),
      a3(a3_), modulus3(modulus3_) { };
  };

  //! Different parameter sets for random number generators of CLCG3 type.
  namespace CLCG3_param_sets {
    //! %TRNG default
    const CLCG3_param TRNG=CLCG3_param(376555083l, 2147482951l,1028879659l, 2147482949l, 225802979l, 2147482943l);
  }

  class CLCG3;  // forward declaration;
  //! combined generator
  /*!
    This is a combined linear congruential random number generator 
    with three generators. 
    \f[
    \begin{split}
    q_{1,i} & =  a_1\cdot q_{1,i-1} \mod m_1 \\
    q_{2,i} & =  a_2\cdot q_{2,i-1} \mod m_2 \\
    q_{3,i} & =  a_3\cdot q_{3,i-1} \mod m_3 \\
    r_i     & = q_{1,i}+q_{2,i}+q_{3,i} \mod m_1-1
    \end{split}
    \f]
    \latexonly See also \cite{lEcuyer1988}.\endlatexonly
    \author Heiko Bauke
  */
  class CLCG3 : public RNG<CLCG3> {
  private:
    generic_MLCG R1;
    generic_MLCG R2;
    generic_MLCG R3;
    long modulus;
  public:
    static const RNG_type type=CLCG3_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<CLCG3> & operator=(RNG<CLCG3> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with
      \f[
      \begin{split}
      q_{1,i} & =    376\,555\,083\cdot q_{1,i-1} \mod 2\,147\,482\,951 \\
      q_{2,i} & = 1\,028\,879\,659\cdot q_{2,i-1} \mod 2\,147\,482\,949 \\
      q_{3,i} & =    225\,802\,979\cdot q_{3,i-1} \mod 2\,147\,482\,943 \\
      r_i     & = q_{1,i}+q_{2,i}+q_{3,i} \mod 2\,147\,482\,950\,.
      \end{split}
      \f]
      This generator has a period of 
      \f$\frac{(2\,147\,482\,951-1)(2\,147\,482\,949-1)
      (2\,147\,482\,943-1)}{4}\approx 2^{91} \approx 2.48\cdot 10^{27}\f$.
      The multipliers were found by an exhausive 
      search applying the spectral test in up to eight dimensions.
      \param param parameter set
      \param seed_ default seed
    */
    CLCG3(CLCG3_param param=CLCG3_param_sets::TRNG, long seed_=0l);
  };

  inline long CLCG3::rand(void) {
    unsigned long t=
      static_cast<unsigned long>(R1.rand())+
      static_cast<unsigned long>(R2.rand());
    if (t>=static_cast<unsigned long>(modulus))
      t-=modulus;
    t+=static_cast<unsigned long>(R3.rand());
    return (t>=static_cast<unsigned long>(modulus)) ? (t-modulus) : t;
  }
  
  // -------------------------------------------------------------------

  //! Parameter set for combined linear congruential generators
  /*!
    This class implements the parameter set for combined linear congruential 
    generators with prime moduli and is used by class CLCG4. 
    See also CLCG4_param_sets.
   */
  class CLCG4_param {
  public:
    const long a1;        //!< multiplier of 1st generator
    const long modulus1;  //!< prime modulus of 1st generator
    const long a2;        //!< multiplier of 2nd generator
    const long modulus2;  //!< prime modulus of 2nd generator
    const long a3;        //!< multiplier of 3rd generator
    const long modulus3;  //!< prime modulus of 3rd generator
    const long a4;        //!< multiplier of 4th generator
    const long modulus4;  //!< prime modulus of 4th generator
    /*!
      The parameter of the constructor specify the multipliers and the 
      prime moduli.
      \param a1_ multiplier of 1st generator
      \param modulus1_ prime modulus of 1st generator
      \param a2_ multiplier of 2nd generator
      \param modulus2_ prime modulus of 2nd generator
      \param a3_ multiplier of 3rd generator
      \param modulus3_ prime modulus of 3rd generator
      \param a4_ multiplier of 4rd generator
      \param modulus4_ prime modulus of 4rd generator
     */
    CLCG4_param(long a1_, long modulus1_, long a2_, long modulus2_, long a3_, long modulus3_, long a4_, long modulus4_) :
      a1(a1_), modulus1(modulus1_),
      a2(a2_), modulus2(modulus2_),
      a3(a3_), modulus3(modulus3_),
      a4(a4_), modulus4(modulus4_) { };
  };

  //! Different parameter sets for random number generators of CLCG4 type.
  namespace CLCG4_param_sets {
    //! %TRNG default
    const CLCG4_param TRNG=CLCG4_param(376555083l, 2147482951l, 1028879659l, 2147482949l, 225802979l, 2147482943l,2028073966l, 2147482859l);
  }

  class CLCG4;  // forward declaration;
  //! combined generator
  /*!
    This is a combined linear congruential random number generator 
    with four generators. 
    \f[
    \begin{split}
    q_{1,i} & =  a_1\cdot q_{1,i-1} \mod m_1 \\
    q_{2,i} & =  a_2\cdot q_{2,i-1} \mod m_2 \\
    q_{3,i} & =  a_3\cdot q_{3,i-1} \mod m_3 \\
    q_{4,i} & =  a_3\cdot q_{4,i-1} \mod m_4 \\
    r_i     & = q_{1,i}+q_{2,i}+q_{3,i}+q_{4,i} \mod m_1-1
    \end{split}
    \f]
    \latexonly See also \cite{lEcuyer1988}.\endlatexonly
    \author Heiko Bauke
  */
  class CLCG4 : public RNG<CLCG4> {
  private:
    generic_MLCG R1;
    generic_MLCG R2;
    generic_MLCG R3;
    generic_MLCG R4;
    long modulus;
  public:
    static const RNG_type type=CLCG4_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);  
    void load_status(const std::vector<long> &s);
    RNG<CLCG4> & operator=(RNG<CLCG4> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with
      \f[
      \begin{split}
      q_{1,i} & =    376\,555\,083\cdot q_{1,i-1} \mod 2\,147\,482\,951 \\
      q_{2,i} & = 1\,028\,879\,659\cdot q_{2,i-1} \mod 2\,147\,482\,949 \\
      q_{3,i} & =    225\,802\,979\cdot q_{3,i-1} \mod 2\,147\,482\,943 \\
      q_{4,i} & = 2\,028\,073\,966\cdot q_{3,i-1} \mod 2\,147\,482\,859 \\
      r_i     & = q_{1,i}+q_{2,i}+q_{3,i}+q_{4,i} \mod 2\,147\,482\,950\,.
      \end{split}
      \f]
      This generator has a period of 
      \f$\frac{(2\,147\,482\,951-1)(2\,147\,482\,949-1)
      (2\,147\,482\,943-1)(2\,147\,482\,859-1)}{8}\approx 2^{121}
      \approx 2.66\cdot 10^{36}\f$.
      The multipliers were found by an exhausive 
      search applying the spectral test in up to eight dimensions.
      \param param parameter set
      \param seed_ default seed
    */
    CLCG4(CLCG4_param param=CLCG4_param_sets::TRNG, long seed_=0l);
  };

  inline long CLCG4::rand(void) {
    unsigned long t=
      static_cast<unsigned long>(R1.rand())+
      static_cast<unsigned long>(R2.rand());
    if (t>=static_cast<unsigned long>(modulus))
      t-=modulus;
    t+=static_cast<unsigned long>(R3.rand());
    if (t>=static_cast<unsigned long>(modulus))
      t-=modulus;
    t+=static_cast<unsigned long>(R4.rand());
    return (t>=static_cast<unsigned long>(modulus)) ? (t-modulus) : t;
  }
    
  // -------------------------------------------------------------------

  //! Parameter set for explicit inversive congruential generator
  /*!
    This class implements the parameter set for explicit inversive 
    congruential with prime modulus and is used by class EINV. 
    See also EINV_param_sets.
    \f[
    r_i=\overline{a\cdot i+b}\mod m
    \f]
   */
  class EINV_param {
  public:
    const long a;  //!< multiplier
    const long b;  //!< additive constant
    const long modulus;  //!< prime modulus
    /*!
      The parameters of the constructor specify the multiplier and the 
      prime modulus.
      \param a_ multiplier \f$a\f$ 
      \param b_ additive constant \f$b\f$ 
      \param modulus_ prime modulus\f$m\f$ 
     */
    EINV_param(long a_, long b_, long modulus_) :
      a(a_), b(b_), modulus(modulus_) { };
  };

  //! Different parameter sets for random number generators of EINV type.
  namespace EINV_param_sets {
    //! %TRNG default
    const EINV_param TRNG=EINV_param(1073741831l, 0l, 1342177283l);
  }

  class EINV;  // forward declaration;
  //! explicit inversive congruential generator
  /*!
    This is an explicit inversive congruential generator with prime modulus.
    \f[
    r_i=\overline{a\cdot i+b}\mod m
    \f]
    This generator has a period of \f$m\f$. This generator type has 
    excelent statistical properties but its period is too short for 
    large applications. You may combine this genrator with another 
    one.
    \latexonly See also \cite{Eichenauer1993}.\endlatexonly
    \author Heiko Bauke
  */
  class EINV : public RNG<EINV> {
  private:
    long a, b, i, di, modulus, a_save, b_save, modulus_save;
  public:
    static const RNG_type type=EINV_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);
    RNG<EINV> & operator=(RNG<EINV> &other);
    //! constructor
    /*!
      The default multiplier is \f$a=1\,073\,741\,831\f$, additive constant
      \f$b=0\f$ and modulus \f$m=1\,342\,177\,283\f$.
      \param param parameter set
      \param seed_ seed
    */
    EINV(EINV_param param=EINV_param_sets::TRNG, long seed_=0l);    
  };

  inline long EINV::rand(void) {
    i-=modulus;
    i+=di;
    if (i<0)
      i+=modulus;
    long x=static_cast<long>((static_cast<long long>(a)*
			      static_cast<long long>(i))%modulus)-modulus;
    x+=b;
    if (x<0)
      x+=modulus;
    // invert x
    long temp, q, flast=0, f=1, m=modulus;
    while (x>1) {
      temp=m%x; 
      q=m/x;
      m=x;  
      x=temp;  
      temp=f; 
      f=flast-q*f; 
      flast=temp;
    }
    return (a==0) ? 0 : ( (f<0) ? (f+modulus) : f );
  }
  
  // -------------------------------------------------------------------

  //! Parameter set for combined generator
  /*!
    This class implements the parameter set for conbined generator and 
    is used by class EINVLCG64. See also EINVLCG64_param_sets.
    \f[
    \begin{split}
    q_{1,i} & = \overline{a\cdot i+b}\mod m \\
    q_{2,i} & = c\cdot q_{2,i-1}+d\mod 2^{64} \\
    r_i     & = q_{1,i} + \lfloor q_{2,i}/2^{33} \rfloor \mod 2^{31} \\
    \end{split}
    \f]
  */
  class EINVLCG64_param {
  public:
    const long a;  //!< multiplier for explicit inversive congruential generator
    const long b;  //!< additive constant for explicit inversive congruential generator
    const long modulus;  //!< prime modulus for explicit inversive congruential generator
    const unsigned long long c;  //! multiplier for linear congruential generator
    const unsigned long long d;  //! multiplier for linear congruential generator
    
    /*!
      The parameters of the constructor specify the multiplier, the additive 
      constant and the prime modulus for the explicit inversive generator 
      and the multiplier and the additive constant for the linear 
      congruential generator.
      \param a_ multiplier \f$a\f$ 
      \param b_ additive constant \f$b\f$ 
      \param modulus_ prime modulus\f$m\f$ 
      \param c_ multiplier \f$c\f$ 
      \param d_ additive constant \f$d\f$ 
    */
    EINVLCG64_param(long a_, long b_, long modulus_, unsigned long long c_, unsigned long long d_) :
      a(a_), b(b_), modulus(modulus_), c(c_), d(d_) { };
  };

  //! Different parameter sets for random number generators of EINV type.
  namespace EINVLCG64_param_sets {
    //! %TRNG default
    const EINVLCG64_param TRNG=EINVLCG64_param(1073741831l, 0l, 1342177283l, 18145460002477866997ull, 1ull);
  }

  class EINVLCG64;  // forward declaration;
  //! combined generator
  /*!
    This is a combined generator.
    \f[
    \begin{split}
    q_{1,i} & = \overline{a\cdot i+b}\mod m \\
    q_{2,i} & = c\cdot q_{2,i-1}+d \mod 2^{64} \\
    r_i     & = q_{1,i} + \lfloor q_{2,i}/2^{33} \rfloor \mod 2^{31} \\
    \end{split}
    \f]
    \author Heiko Bauke
  */
  class EINVLCG64 : public RNG<EINVLCG64> {
  private:
    LCG64 R1;
    EINV R2;
  public:
    static const RNG_type type=EINVLCG64_t;
    const char * name(void);
    void reset(void);
    void seed(long s=0l);    
    long rand(void);
    void split(long s, long n);
    void jump2(long s);
    void save_status(std::vector<long> &s);
    void load_status(const std::vector<long> &s);    
    RNG<EINVLCG64> & operator=(RNG<EINVLCG64> &other);
    //! constructor
    /*!
      The constructor's default values implement a pseudo random number 
      generator with
      \f[
      \begin{split}
      q_{1,i}&=\overline{1073741831\cdot i}\mod (2^{30}+2^{28}+3) \\
      q_{2,i}&=18\,145\,460\,002\,477\,866\,997\cdot q_{2,i-1}+1\mod 2^{64} \\
      r_{i}&=q_{1,i}+\lfloor q_{2,i}/2^{33}\rfloor \mod 2^{31}
      \end{split}\,.
      \f]
      \param param param set
      \param seed_ seed	
    */
    EINVLCG64(EINVLCG64_param param=EINVLCG64_param_sets::TRNG, long seed_=0l);    
  };

  inline long EINVLCG64::rand(void) {
    return (R1.rand()+R2.rand())&0x7fffffffl;
  }

}


#endif
