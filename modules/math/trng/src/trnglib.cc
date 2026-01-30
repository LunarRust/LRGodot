// Time-stamp: <Freitag, 28.02.2003, 21:21:05; edited by bauke>
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

#include <trng.h>
#include <trnglib.h>
#include <cmath>

//! TRNG version
/*!
    This function returns a pointer to a zero termiated string with the 
    TRNG version.
    \return pointer to a zero termiated string
*/
const char * TRNG::version(void) {
  return "TRNG V"TRNG_VERSION"."TRNG_REVISION;
}

// ---------------------------------------------------------------------

//! modulo invers
/*!
  Solves the equation \f$a\cdot x=1\mod m\f$.
  \return the inverse of \f$a\f$.
  \param a a positive integer.
  \param m a prime modulus.
  \exception error if \f$a\le 0\f$ or \f$m\le 1\f$ or no inversive exists
  \sa TRNG::error
  \author Heiko Bauke
*/
long TRNG::modulo_invers(long a, long m) {
  if (a<=0l || m<=1)
    throw error("invalid argument in TNRG::modulo_invers");
  long temp, q, flast=0, f=1, m1=m;
  while (a>1) {
    temp=m1%a; 
    q=m1/a;
    m1=a;  a=temp;  temp=f; 
    f=flast-q*f; 
    flast=temp;
  }
  if (a==0) 
    throw error("no inversive in TNRG::modulo_invers");
  return f>=0 ? f : f+m;
}

// ---------------------------------------------------------------------
  
//! linear system solver in modular arithmetic
/*!
    Solves a system of linear equations
    \f[
    \begin{pmatrix}
    a_{1,1} & a_{1,2} & \dots  & a_{1,n} \\
    a_{2,1} & a_{2,2} & \dots  & a_{2,n} \\
    \vdots  & \vdots  & \ddots & \vdots  \\
    a_{n,1} & a_{n,2} & \dots  & a_{n,n}
    \end{pmatrix}
    \begin{pmatrix}
    x_1 \\ x_2 \\ \vdots \\ x_n
    \end{pmatrix}
    =
    \begin{pmatrix}
    b_1 \\ b_2 \\ \vdots \\ b_n
    \end{pmatrix}
    \mod m
    \f]
    in modular arithmetic using Gau&szlig; elimination.
    \param a reference to the coefficient matrix, content is destroyed 
    after function call
    \param b reference to the inhomogenous right side, contains the solution 
    \f$(x_1, x_2, \dots , x_n)^T\f$ after function call
    \param m prime modulus \f$m\f$
    \exception error if coefficient matrix is singular or the matrices 
    have invalid sizes
    \sa TRNG::error
    \author Heiko Bauke
*/
void TRNG::gauss(std::vector<long> &a, std::vector<long> &b, long m) {
  if (a.size()!=b.size()*b.size() || a.size()==0 || b.size()==0) 
    throw TRNG::error("wrong matrix size in TRNG::gauss");
  // initialize indices
  int n=b.size();
  int rank=0;
  std::vector<long> p(n);
  for (int i=0; i<n; ++i)
    p[i]=i;
  // make matrix triangular
  for (int i=0; i<n; ++i) {
    // search for a pivot element
    if (a[n*p[i]+i]==0l) {
      // swap rows
      int j=i+1;
      while (j<n && a[n*p[j]+i]==0l) 
        j++;
      if (j<n) {
	long t=p[i];  p[i]=p[j];  p[j]=t;
      }
    }
    // is rank small?
    if (a[n*p[i]+i]==0l)
      break;
    ++rank;
    long t=TRNG::modulo_invers(a[n*p[i]+i], m);
    for (int j=i; j<n; ++j) 
      a[n*p[i]+j]=static_cast<long>
        ((static_cast<long long>(a[n*p[i]+j])*
          static_cast<long long>(t))%m);
    b[p[i]]=static_cast<long>
      ((static_cast<long long>(b[p[i]])*
        static_cast<long long>(t))%m);
    for (int j=i+1; j<n; ++j) {
      if (a[n*p[j]+i]!=0l) {
        t=TRNG::modulo_invers(a[n*p[j]+i], m);
        for (int k=i; k<n; ++k) {
          a[n*p[j]+k]=
            static_cast<long>
            ((static_cast<long long>(a[n*p[j]+k])*
              static_cast<long long>(t))%m);
          a[n*p[j]+k]-=a[n*p[i]+k];
          if (a[n*p[j]+k]<0l)
            a[n*p[j]+k]+=m;
        }
        b[p[j]]=static_cast<long>
          ((static_cast<long long>(b[p[j]])*
            static_cast<long long>(t))%m);
        b[p[j]]-=b[p[i]];
        if (b[p[j]]<0l)
          b[p[j]]+=m;
      }
    }
  }
  // test if a solution exists
  for (int i=rank; i<n; ++i) 
    if (b[p[i]]!=0l)
      throw TRNG::error("equations system has no solution TRNG::gauss");
  // solve triangular system
  for (int i=n-2; i>=0; --i)
    for (int j=i+1; j<n; ++j) {
      b[p[i]]-=static_cast<long>
        ((static_cast<long long>(a[n*p[i]+j])*
          static_cast<long long>(b[p[j]]))%m);
      if (b[p[i]]<0l)
        b[p[i]]+=m;
    }
  // sort
  for (int i=0; i<n; ++i) 
    p[i]=b[p[i]];
  for (int i=0; i<n; ++i) 
    b[i]=p[i];
}


// ---------------------------------------------------------------------

//! matrix multiplication
/*!
    Multiply two equal sized quadratic matrices \f$a\f$ and \f$b\f$ in modular
    arithmetic, \f$c=a\cdot b \mod m\f$.
    \param a reference to matrix \f$a\f$
    \param b reference to matrix \f$b\f$
    \param c reference to matrix \f$c\f$
    \param m modulus \f$m\f$
    \exception error if the matrices are different sized
    \sa TRNG::error
    \author Heiko Bauke
*/
void TRNG::matrix_mult(const std::vector<long> &a, const std::vector<long> &b,
		       std::vector<long> &c, long m) {
  if (a.size()!=b.size())
    throw TRNG::error("different sized matrices in TRNG::matrix_mult");
  int n=static_cast<int>(std::sqrt(static_cast<double>(b.size())));
  c.resize(n*n);
  for (int i=0; i<n; ++i)
    for (int j=0; j<n; ++j) {
      long long t=0ll;
      for (int k=0; k<n; ++k) {
	t+=(static_cast<long long>(a[j*n+k])*
	    static_cast<long long>(b[k*n+i]))%m;
	if (t>=m)
	  t-=m;
      }
      c[j*n+i]=static_cast<long>(t);
    }
}

// ---------------------------------------------------------------------

//! matrix vector multiplication
/*!
    Multiply a quadratic matrix \f$a\f$ and $a$ vector \f$b\f$ in modular
    arithmetic, 
    \f$c=a\cdot b \mod m\f$.
    \param a reference to matrix \f$a\f$
    \param b reference to vector \f$b\f$
    \param c reference to vector \f$c\f$
    \param m modulus \f$m\f$
    \exception error if the matrices are different sized
    \sa TRNG::error
    \author Heiko Bauke
*/
void TRNG::matrix_vec_mult(const std::vector<long> &a, 
			   const std::vector<long> &b, 
			   std::vector<long> &c, long m) {
  if (a.size()!=b.size()*b.size())
    throw TRNG::error("different sized vectors in TRNG::matrix_vec_mult");
  int n=b.size();
  c.resize(n);
  for (int j=0; j<n; ++j) {
    long long t=0ll;
    for (int k=0; k<n; ++k) {
      t+=(static_cast<long long>(a[j*n+k])*
	  static_cast<long long>(b[k]))%m;
      if (t>=m)
	t-=m;
    }
    c[j]=static_cast<long>(t);
  }
}

// ---------------------------------------------------------------------

// for special functions calculation see
// Samuel S. M. Wong Computational Methods in Physics and Engineering

// ---------------------------------------------------------------------

//! \f$\ln\f$ of \f$\Gamma\f$-function
/*!
    Computes the \f$\Gamma\f$-function's logarithm
    \f$\ln\Gamma(x)=\ln\int\limits_0^\infty t^{x-1}\mathrm{e}^{-t}\,\mathrm{d}t\f$
    for positive arguments. 
    \latexonly Method is discribed in \cite{Lancos1964}.\endlatexonly
    \param x argument
    \exception error if \f$x\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::ln_Gamma(double x) {
  if (x<=0)
    throw TRNG::error("invalid (nonpositive) argument for TRNG::lnGamma");
  const double gamma=7.0;
  const int N=9;
  const double ln_sqrt_2_pi=.9189385332046725; // ln(sqrt(2 Pi))
  double t1=x-0.5;
  double t2=t1+gamma;
  /* coefficients for gamma=7, kmax=8  Lanczos method */
  const double lanczos_7_c[N]={0.99999999999980993227684700473478,
			       676.520368121885098567009190444019,
			       -1259.13921672240287047156078755283,
			       771.3234287776530788486528258894,
			       -176.61502916214059906584551354,
			       12.507343278686904814458936853,
			       -0.13857109526572011689554707,
			       9.984369578019570859563e-6,
			       1.50563273514931155834e-7};
  t1=ln_sqrt_2_pi+t1*std::log(t2)-t2;
  t2=lanczos_7_c[0];
  for (int i=1; i<N; ++i)
    t2+=lanczos_7_c[i]/x++;
  return t1+std::log(t2);
}

// ---------------------------------------------------------------------

//! \f$\Gamma\f$-function
/*!
    Computes the \f$\Gamma\f$-function 
    \f$\Gamma(x)=\int\limits_0^\infty t^{x-1}\mathrm{e}^{-t}\,\mathrm{d}t\f$
    for positive arguments.
    \param x argument
    \exception error if \f$x\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::Gamma(double x) {
  if (x<=0.0)
    throw TRNG::error("invalid (nonpositive) argument for TRNG::Gamma");
  // use a interpolating polynom for 1<=x<2
  if (x<15.0) {
    double f, z;
    // Gamma(x)=Gamma(x+1)/x
    if (x<1.0) {
      f=1.0/x;
      z=x;
    }
    if (1.0<=x && x<2.0) {
      z=x-1.0;
      f=1.0;
    }
    // Gamma(x+1)=x*Gamma(x)
    if (x>=2.0) {
      f=1.0;
      z=x-1.0;
      do {
	f=f*z;
	z--;
      } while (z>=1.0);
    }
    return f*(1.0+
	      (-0.5772152952998961+
	       (0.9890418621994901+
		(-0.9072503205615771+
		 (0.9796139097861202+
		  (-0.9693649942410396+
		   (0.9409502806615237+
		    (-0.8401327394958836+
		     (0.6506941079199484+
		      (-0.4028899587819087+
		       (0.1812400864733248+
			(-0.5156307405048575E-1
			 +0.6876141229E-2
			 *z)*z)*z)*z)*z)*z)*z)*z)*z)*z)*z)*z);
  }
  // Stirling's approximation
  double t=x*x;
  return std::exp(-x)*std::pow(x, x-0.5)*2.5066282746310*
    (1.0
     +1.0/12.0/x
     +1/288.0/t
     -139.0/51840.0/(t*x)
     -571.0/2488320.0/(t*t));
}

// ---------------------------------------------------------------------

//! incomplete \f$\Gamma\f$-function
/*!
    Computes the incomplete \f$\Gamma\f$-function 
    \f$P(a, x)=\frac{\displaystyle 1}{\displaystyle \Gamma(a)}
    \int\limits_0^x t^{a-1}\mathrm{e}^{-t}\,\mathrm{d}t\f$
    for positive arguments.
    \param a argument \f$a\f$
    \param x argument \f$x\f$
    \exception error if \f$x<0\f$ or \f$a\le0\f$
    \sa TRNG::error
    \author Heiko Bauke
 */
double TRNG::Gamma_P(double a, double x) {
  if (x<0.0 || a<=0.0)
    throw TRNG::error("invalid (nonpositive) argument for TRNG::GammaP");
  double t;
  t=TRNG::Gamma(a);
  if (x<a+1.0)
    return TRNG::Gamma_ser(a, x)/t;
  else
    return 1.0-TRNG::Gamma_cf(a, x)/t;
}

// ---------------------------------------------------------------------

//! incomplete \f$\Gamma\f$-function
/*!
    Computes the incomplete \f$\Gamma\f$-function 
    \f$Q(a, x)=\frac{\displaystyle 1}{\displaystyle \Gamma(a)}
    \int\limits_x^\infty t^{a-1}\mathrm{e}^{-t}\,\mathrm{d}t\f$
    for positive arguments.
    \param a argument \f$a\f$
    \param x argument \f$x\f$
    \exception error if \f$x<0\f$ or \f$a\le0\f$
    \sa TRNG::error
    \author Heiko Bauke
 */
double TRNG::Gamma_Q(double a, double x) {
  if (x<0.0 || a<=0.0)
    throw TRNG::error("invalid (nonpositive) argument for TRNG::GammaQ");
  double t;
  t=TRNG::Gamma(a);
  if (x<a+1.0)
    return 1.0-TRNG::Gamma_ser(a, x)/t;
  else
    return TRNG::Gamma_cf(a, x)/t;
}

// ---------------------------------------------------------------------

//! incomplete \f$\Gamma\f$-function
/*!
    Computes the incomplete \f$\Gamma\f$-function 
    \f$\gamma(a, x)=
    \int\limits_0^x t^{a-1}\mathrm{e}^{-t}\,\mathrm{d}t\f$
    for positive arguments.
    \param a argument \f$a\f$
    \param x argument \f$x\f$
    \exception error if \f$x<0\f$ or \f$a\le0\f$
    \sa TRNG::error
    \author Heiko Bauke
 */
double TRNG::incomp_Gamma(double a, double x) {
  if (x<0.0 || a<=0.0)
    throw TRNG::error("invalid (nonpositive) argument for TRNG::incomp_Gamma");
  if (x<a+1.0)
    return TRNG::Gamma_ser(a, x);
  else
    return TRNG::Gamma(a)-TRNG::Gamma_cf(a, x);
}

// ---------------------------------------------------------------------

//! incomplete \f$\Gamma\f$-function
/*!
    Computes the complementary incomplete \f$\Gamma\f$-function
    \f$\Gamma(a, x)=
    \int\limits_x^\infty t^{a-1}\mathrm{e}^{-t}\,\mathrm{d}t\f$
    for positive arguments.
    \param a argument \f$a\f$
    \param x argument \f$x\f$
    \exception error if \f$x<0\f$ or \f$a\le0\f$
    \sa TRNG::error
    \author Heiko Bauke
 */
double TRNG::comp_incomp_Gamma(double a, double x) {
  if (x<0.0 || a<=0.0)
    throw TRNG::error("invalid (nonpositive) argument for TRNG::comp_incomp_Gamma");
  if (x<a+1.0)
    return TRNG::Gamma(a)-TRNG::Gamma_ser(a, x);
  else
    return TRNG::Gamma_cf(a, x);
}

// ---------------------------------------------------------------------

//! incomplete \f$\Gamma\f$-function
/*!
    Computes incomplete Gamma function's (\f$P(a,x)\f$) power series 
    representation for \f$x\le a+1\f$.
    \param a argument \f$a\f$
    \param x argument \f$x\f$
    \exception error if \f$x<0\f$ or \f$a\le0\f$ or at convergence problems
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::Gamma_ser(double a, double x) {
  const int itmax=150;
  const double eps=1e-12;
  int i;
  double sum, xx, n; 
  if (x<0.0)
    throw TRNG::error("x less than 0 in routine TRNG::Gamma_ser");
  if (a<=0.0)
    throw TRNG::error("a less or equal than 0 in routine TRNG::Gamma_ser");
  if (x==0.0)
    return 0.0;
  xx=1.0/a;
  n=a;
  sum=xx;
  i=0;
  do {
    ++n;
    ++i;
    xx*=x/n;
    sum+=xx;
  } while (std::fabs(xx)>eps*std::fabs(sum) && i<itmax);
  if (i==itmax)
    throw TRNG::error("convergence problem in TRNG::Gamma_ser");
  return std::exp(-x+a*std::log(x))*sum;
}

// ---------------------------------------------------------------------

//! incomplete \f$\Gamma\f$-function
/*!
    Computes incomplete Gamma function's (\f$Q(a,x)\f$) continued fraction  
    representation for \f$x\ge a+1\f$.
    \param a argument \f$a\f$
    \param x argument \f$x\f$
    \exception error if \f$x<0\f$ or \f$a\le0\f$ or at convergence problems
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::Gamma_cf(double a, double x) {
  // complementary incomplete Gamma function's continued fraction  
  // representation for x>a+1
  const double itmax=75.0;
  const double eps=1.0e-12;
  const double min=1.0e-45;
  double ai, bi, ci, di, del, h, i; 
  if (x<0.0)
    throw TRNG::error("x less than 0 in routine TRNG::Gamma_cf");
  if (a<=0.0)
    throw TRNG::error("a less or equal than 0 in routine TRNG::Gamma_cf");
  // Set up for evaluating continued fraction by modied Lentz's method
  bi=x+1.0-a; 
  ci=1.0/min; 
  di=1.0/bi; 
  h=di;
  i=0.0;
  do { 
    // Iterate to convergence. 
    ++i;
    ai=-i*(i-a); 
    bi+=2.0;
    di=ai*di+bi;
    if (std::fabs(di)<min) 
      di=min; 
    ci=bi+ai/ci; 
    if (std::fabs(ci)<min)
      ci=min;
    di=1.0/di;
    del=di*ci;
    h*=del;
  } while ((std::fabs(del-1.0)>eps) && i<itmax);
  if (i==itmax)
    throw TRNG::error("a too large or convergence problem in TRNG::Gamma_cf"); 
  return std::exp(-x+a*std::log(x))*h;
}

// ---------------------------------------------------------------------

//! logarithm of the factorial function
/*!
    \return \f$\ln (n!)\f$
    \param n \f$n\f$
    \exception error if \f$n<0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::ln_factorial(long n) {
  if (n<0l)
    throw TRNG::error("parameter less than zero in TRNG::ln_factorial");
  const long n_ln_fac_tab=32l;
  static double ln_fac_tab[n_ln_fac_tab]={0.0, 0.0};
  static long in_tab_count=1l;
  if (n>=n_ln_fac_tab)
    return TRNG::ln_Gamma(static_cast<double>(n)+1.0);
  if (n>in_tab_count) {
    for (long i=in_tab_count+1; i<=n; ++i)
      ln_fac_tab[i]=ln_fac_tab[i-1l]+std::log(static_cast<double>(i));
    in_tab_count=n;
  }
  return ln_fac_tab[n];
}

// ---------------------------------------------------------------------

//! binomial coefficient
/*!
    \return binomial coefficient \f$\binom{n}{k}$, if $n<0\f$ 
    or \f$k<0\f$ or \f$k>n\f$ \f$0\f$ is returned
    \param n \f$n\f$
    \param k \f$k\f$
    \author Heiko Bauke
*/
long TRNG::binomial_coeff(long n, long k) {
  if (n<0 || k<0 || k>n) 
    return 0l;
  return static_cast<long>
    (std::exp(TRNG::ln_factorial(n)-TRNG::ln_factorial(k)-
	      TRNG::ln_factorial(n-k))+0.5);
}

// ---------------------------------------------------------------------

//! %error function
/*!
    Computes the %error function
    \f[
    \mathit{errf}(x)=\frac{\displaystyle 1}{\displaystyle\sqrt{2\pi}}
    \int\limits_{-\infty}^x \mathrm{e}^{-\frac{t^2}{2}}\,\mathrm{d}t\,.
    \f]
    \return \f$\mathit{errf}(x)\f$
    \author Heiko Bauke
*/
double TRNG::errf(double x) {
  return (x>0.0) ? 
    0.5*TRNG::Gamma_P(0.5, 0.5*x*x) : 
    -0.5*TRNG::Gamma_P(0.5, 0.5*x*x);
}

// ---------------------------------------------------------------------

//! Chisquare test
/*!
    Applies a \f$\chi^2\f$-test.
    \return \f$\chi^2\f$-value
    \param prob reference to a vector with some probabilities
    \param observ reference to a vector with numbers of actual observations
    \exception error if arguments are not equal sized or number of 
    observations is less then five
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::chi_square_test(const std::vector<double> &prob, 
			     const std::vector<double> &observ) {
  double chi2, n, t1, t2;
  if (prob.size()!=observ.size())
    throw TRNG::error("different sized vectors in TRNG::chi_square_test");
  n=0.0;
  chi2=0.0;
  for (unsigned int i=0; i<prob.size(); ++i) {
    n+=observ[i];
    if (observ[i]<5.0)
      throw TRNG::error("not enough observations in TRNG::chi_square_test");
  }
  for (unsigned int i=0; i<prob.size(); ++i) {
    t1=prob[i]*n;
    t2=observ[i]-t1;
    chi2+=t2*t2/t1;
  }
  return chi2;
}

// ---------------------------------------------------------------------

//! chisquare test
/*!
    Computes the probability corresponding to a \f$\chi^2\f$-value.
    \latexonly See \cite{Knuth1981} page 41 for details.\endlatexonly
    \param chi2 \f$\chi^2\f$-value
    \param df degrees of freedom
    \exception error if degrees of freedom less than one
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::chi_square_prob(double chi2, long df) {
  if (df<1l)
    throw TRNG::error("degrees of freedom less than one in TRNG::chi_square_prob");
  double ddf;
  ddf=static_cast<double>(df);
  if (chi2==0.0)
    return 1.0;
  if (df<256l)
    return TRNG::Gamma_Q(0.5*ddf, 0.5*chi2);
  // see Knuth, The Art of Computer Programming II, page 41
  return 0.5-TRNG::errf(-3.0/4.0*std::sqrt(2.0)*std::sqrt(ddf)+
			1.0/4.0*std::sqrt(-6.0*ddf+24.0*chi2+16.0));
}

// ---------------------------------------------------------------------

//! Stirling number
/*!
   Computes the Stirling number of the 2nd kind
   \f$\begin{Bmatrix}
   n \\ m 
   \end{Bmatrix}\f$\latexonly, see also \cite{Knuth1981I} pp. 65ff\endlatexonly. 
   \return Stirling number of the 2nd kind \f$\left\{{n \atop m}\right\}\f$
   \param n 1st parameter
   \param m 2nd parameter
   \author Heiko Bauke
*/
double TRNG::Stirling_num2(long n, long m) {
  // see Knuth, The Art of Computer Programming I
  if (n<0l || m<0l || m>n)
    return 0.0;
  if (m==n)
    return 1.0;
  if (m==0l)
    return 0.0;
  if (m==1l)
    return 1.0;
  if (m==2l)
    return static_cast<double>((1ll<<(n-1l))-1ll);
  return m*TRNG::Stirling_num2(n-1l, m)+TRNG::Stirling_num2(n-1l, m-1l);
}

// ---------------------------------------------------------------------

//! values for Student's \f$t\f$-distribution
/*!
    Computes \f$t_{p, \nu}\f$ of the \f$t\f$-distribution. \f$t_{p, \nu}\f$ 
    is defined in the symmetric case as
    \f[
    p=\dfrac{\Gamma((\nu+1)/2)}{\sqrt{\pi\nu}\,\Gamma(\nu/2)}
    \int_{-t_{p, \nu}}^{t_{p, \nu}} (1+x^2/\nu)^\frac{\nu+1}{2}\,
    \mathrm{d}x
    \f]
    and in the asymmetric case as
    \f[
    p=\dfrac{\Gamma((\nu+1)/2)}{\sqrt{\pi\nu}\,\Gamma(\nu/2)}
    \int_{-\infty}^{t_{p, \nu}} (1+x^2/\nu)^\frac{\nu+1}{2}\,
    \mathrm{d}x\,.
    \f]
   \param p probability \f$p\f$
   \param nu degrees of freedom \f$\nu\f$
   \param symmetric is true for the symmetric case
   \exception error if less than one degree of freedom or probability out of 
   range \f$0<p<1\f$
   \sa TRNG::error
   \author Heiko Bauke    
 */
double TRNG::Student_t(double p, long nu, bool symmetric) {
  const double pi=3.14159265358979324;
  const double sqrt2=1.41421356237309505;
  const double two_div_sqrt_Pi=1.12837916709551257;
  double t, dt, t2, g1, g2, g3, g4, m;
  int i;
  bool less_0_5;
  if (nu<1l)
    throw TRNG::error("less than one degree of freedom in TRNG::Student_t");
  if (p<=0.0 || p>=1.0)
    throw TRNG::error("probability out of range in TRNG::Student_t");
  m=static_cast<double>(nu);
  if (symmetric)
    p+=0.5*(1.0-p);
  // exact formulas from Statistik. Lehr- und Handbuch der angewandten 
  // Statistik., Joachim Hartung, Bärbel Elpelt, Karl-Heinz Klösener 
  // R. Oldenbourg Verlag, 1998, page 892
  if (nu==1l) {
    t=std::tan(pi*(p-0.5));
  }
  if (nu==2l) {
    t=(2.0*p-1.0);
    t*=sqrt2/std::sqrt(1.0-t*t); 
  }
  if (nu>2l) {
    // use symmetry t(1-p, m)=-t(p, m)
    less_0_5=(p<0.5);
    if (less_0_5)
      p=1.0-p;
    // claculate t(p, m) for m=oo with Newton's method 
    t=0.0;
    i=0;
    do {
      dt=-(2.0*TRNG::errf(sqrt2*t)+1.0-2.0*p)/(two_div_sqrt_Pi*std::exp(-t*t));
      t+=dt;
    } while (std::fabs(dt/t)>1e-14 && ++i<12);
    // use approximation form Handbook of Mathematical Functions, 
    // With Formulas, Graphs, and Mathematical Tables, Milton Abramowitz 
    // Dover Publications, Inc., page 949
    t*=sqrt2;
    t2=t*t;
    g1=1.0/4.0*(1.0+t2)*t;
    g2=1.0/96.0*(3.0+(16.0+5.0*t2)*t2)*t;
    g3=1.0/384.0*(-15.0+(17.0+.0*(19.0+3.0*t2)*t2)*t2)*t;
    g4=1.0/92160.0*(-945.0+(-1920.0+(1482.0+(776.0+79*t2)*t2)*t2)*t2)*t;
    t+=g1/m+g2/m/m+g3/m/m/m+g4/m/m/m/m;
    if (less_0_5)
      t*=-1.0;
  }
  return t;
}

// ---------------------------------------------------------------------

//! find interval
/*!
    This function searches for the interval which contains \f$x\f$ using an
    \f$O(\ln n)\f$ algorithm. The intervals are
    \f[
    I_0=(-\infty, b_0]\,,\quad
    I_1=(-b_0, b_1]\,,\quad
    I_2=(-b_1, b_2]\,,\quad\dots\quad
    I_{n-1}=(-b_{n-2}, \infty)
    \f]
    if \f$b\f$ is a sorted vector with elements \f$b_i\f$ with 
    \f$0\le i\le n-2\f$. 
    \return interval number
    \param borders reference to a sorted vector \f$b_i\f$ with interval edges
    \param x value that determs the interval
    \author Heiko Bauke
*/
long TRNG::find_interval(const std::vector<double> &borders, double x) {
  long num_classes=borders.size()+1;
  if (num_classes==1 || x<=borders[0])
    return 0;
  if (borders[num_classes-2]<x)
    return num_classes-1;
  long i1=0;
  long i2=num_classes-2;
  while (i2-i1>1) {
    long i3=(i2+i1)/2;
    if (x<=borders[i3])
      i2=i3;
    else
      i1=i3;
  }
  return i2;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    Calculates the probalility density function for in \f$[0,1)\f$ 
    equidistributed random variate.
    \return \f$1\f$ if \f$x\in[0,1)\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniform_pdf(double x) {
  return uniformco_pdf(x);
}

//! probalility density
/*!
    Calculates the probalility density function for in \f$[a,b)\f$ 
    equidistributed random variate.
    \return \f$\frac{1}{b-a}\f$ if \f$x\in[a,b)\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniform_pdf(double x, double a, double b) {
  return uniformco_pdf(x, a, b);
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    Calculates the probalility density function for in \f$[0,1)\f$ 
    equidistributed random variate.
    \return \f$1\f$ if \f$x\in[0,1)\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniformco_pdf(double x) {
  if (0.0<=x && x<1.0)
    return 1.0;
  else
    return 0.0;
}

//! probalility density
/*!
    Calculates the probalility density function for in \f$[a,b)\f$ 
    equidistributed random variate.
    \return \f$\frac{1}{b-a}\f$ if \f$x\in[a,b)\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniformco_pdf(double x, double a, double b) {
  if (a<=x && x<b)
    return 1.0/(b-a);
  else
    return 0.0;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    Calculates the probalility density function for in \f$[0,1]\f$ 
    equidistributed random variate.
    \return \f$1\f$ if \f$x\in[0,1]\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniformcc_pdf(double x) {
  if (0.0<=x && x<=1.0)
    return 1.0;
  else
    return 0.0;
}

//! probalility density
/*!
    Calculates the probalility density function for in \f$[a,b]\f$ 
    equidistributed random variate.
    \return \f$\frac{1}{b-a}\f$ if \f$x\in[a,b]\f$ else \f$0\f$
    \author Heiko Bauke
*/

double TRNG::uniformcc_pdf(double x, double a, double b) {
  if (a<=x && x<=b)
    return 1.0/(b-a);
  else
    return 0.0;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    Calculates the probalility density function for in \f$(0,1]\f$ 
    equidistributed random variate.
    \return \f$1\f$ if \f$x\in(0,1]\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniformoc_pdf(double x) {
  if (0.0<x && x<=1.0)
    return 1.0;
  else
    return 0.0;
}

//! probalility density
/*!
    Calculates the probalility density function for in \f$(a,b]\f$ 
    equidistributed random variate.
    \return \f$\frac{1}{b-a}\f$ if \f$x\in(a,b]\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniformoc_pdf(double x, double a, double b) {
  if (a<x && x<=b)
    return 1.0/(b-a);
  else
    return 0.0;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    Calculates the probalility density function for in \f$(0,1)\f$ 
    equidistributed random variate.
    \return \f$1\f$ if \f$x\in(0,1)\f$ else \f$0\f$
    \author Heiko Bauke
*/
double TRNG::uniformoo_pdf(double x) {
  if (0.0<x && x<1.0)
    return 1.0;
  else
    return 0.0;
}

//! probalility density
/*!
    Calculates the probalility density function for in \f$(a,b)\f$ 
    equidistributed random variate.
    \return \f$\frac{1}{b-a}$ if $x\in(a,b)$ else $0\f$
    \author Heiko Bauke
*/
double TRNG::uniformoo_pdf(double x, double a, double b) {
  if (a<x && x<b)
    return 1.0/(b-a);
  else
    return 0.0;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with normal distribution. This distribution is defined as
    \f[
    p(x)=\frac{1}{\sqrt{2\pi}}\mathrm{e}^{-\frac{(x-\mu)^2}{2\sigma^2}}
    \f]
    with mean \f$\mu$ and variance $\sigma\f$.
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param sigma variance \f$\sigma\f$
    \param mu mean \f$\mu\f$
    \exception error if \f$\sigma\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::normal_dist_pdf(double x, double sigma=1.0, double mu=0.0) {
  const double one_sqrt_2_pi=.3989422804014327; // 1/sqrt(2*Pi)
  if (sigma<=0.0)
    throw TRNG::error("negative or zero standard deviation in TRNG::normal_dist_pdf");
  return one_sqrt_2_pi/sigma*std::exp(-(x-mu)*(x-mu)/(2.0*sigma*sigma));
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with exponential distribution. This distribution is defined as
    \f[
    p(x)=
    \begin{cases}
    \dfrac{1}{\mu}\mathrm{e}^{-\frac{x}{\mu}} & x\ge 0 \\
    0 & \text{else}
    \end{cases}
    \f]
    with \f$\mu>0\f$. 
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param mu mean \f$\mu\f$, \f$\mu>0\f$
    \exception error if \f$\mu\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::exp_dist_pdf(double x, double mu=1.0) {
  if (mu<=0.0)
    throw TRNG::error("negative or zero parameter in TRNG::exp_dist_pdf");
  if (x>=0.0)
    return std::exp(-x/mu)/mu;
  else
    return 0.0;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with Laplace distribution. This distribution is defined as
    \f[
    p(x)=\frac{1}{2a}\mathrm{e}^{-|x|/a}
    \f]
    with \f$a>0\f$.
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param a parameter \f$a\f$, \f$a>0\f$
    \exception error if \f$a\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::laplace_dist_pdf(double x, double a=1.0) {
  if (a<=0.0)
    throw TRNG::error("negative or zero parameter in TRNG::exp_dist_pdf");
  return 0.5/a*std::exp(-std::fabs(x)/a);
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    The tent shaped probability distribution is defined as
    \f[
    p(x)=
    \begin{cases}
    \dfrac{x+a}{a^2} & -a\le x\le 0 \\[1.5ex]
    \dfrac{a-x}{a^2} & 0\le x\le a \\
    0 & \text{else}
    \end{cases}
    \f]
    with \f$a>0\f$.
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param a parameter \f$a\f$
    \exception error if \f$a\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::tent_dist_pdf(double x, double a=1.0) {
  if (a<=0.0)
    throw TRNG::error("negative or zero parameter in TRNG::exp_dist_pdf");
  if (-a<x && x<=0.0)
    return (x+a)/(a*a);
  if (0.0<x && x<=a)
    return (a-x)/(a*a);
  return 0.0;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with \f$\Gamma\f$-distribution. This distribution is defined as
    \f[
    p(x)=
    \begin{cases}
    \dfrac{1}{\Gamma(a)b^a} x^{a-1} \mathrm{e}^{-\frac{x}{b}} & x>0 \\
    0 & \text{else}
    \end{cases}\,.
    \f]
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param a parameter \f$a\f$, \f$a>0\f$
    \param b parameter \f$b\f$, \f$b>0\f$
    \exception error if \f$a\le0\f$ or \f$b\le0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::Gamma_dist_pdf(double x, double a, double b) {
  if (a<=0.0 || b<=0.0)
    throw TRNG::error("parameter less than one in TRNG::Gamma_dist_pdf");
  if (x<0.0) 
    return 0.0;
  if (x==0.0)
    if (a==1.0)
      return 1.0/b ;
    else
      return 0.0;
  if (a==1.0) 
    return std::exp(-x/b)/b ;
  return std::pow(x, a-1.0)*std::exp(-x/b)/TRNG::Gamma(a)/std::pow(b, a);
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with B-distribution. This distribution is defined as
    \f[
    p(x)=
    \begin{cases}
    \dfrac{\Gamma(a+b)}{\Gamma(a)\Gamma(b)}x^{a-1}(1-x)^{b-1} &
    0\le x\le 1 \\
    0 & \text{else}
    \end{cases}\,.
    \f]
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param a parameter \f$a\f$, \f$a>0\f$
    \param b parameter \f$b\f$, \f$b>0\f$
    \exception error if \f$a\le0\f$ or \f$b\le0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::Beta_dist_pdf(double x, double a, double b) {
  if (a<=0.0 || b<=0.0)
    throw TRNG::error("parameter less than one in TRNG::Gamma_dist");
  if (x<0.0 || x>1.0)
    return 0.0;
  return std::exp(TRNG::ln_Gamma(a+b)-TRNG::ln_Gamma(a)-TRNG::ln_Gamma(b))*
    std::pow(x, a-1.0)*std::pow(1.0-x, b-1.0);
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with \f$\chi^2\f$-distribution. This distribution is defined as
    \f[
    p(x)=
    \begin{cases}
    \dfrac{\left(\frac{x}{2}\right)^{\frac{\nu}{2}-1}}
    {2\Gamma(\frac{\nu}{2})}
    \mathrm{e}^{-\frac{x}{2}} & x\ge0 \\
    0 & \text{else}
    \end{cases}\,.
    \f]
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param nu parameter \f$\nu\f$, \f$\nu\ge1\f$
    \exception error if \f$\nu<1\f$
    \sa TRNG::error
    \author Heiko Bauke
*/

double TRNG::chi_square_dist_pdf(double x, double nu) {
  if (nu<1.0)
    throw TRNG::error("parameter less than one in TRNG::chi_square_dist_pdf");
  if (x<=0.0)
    return 0.0;
  return std::exp((0.5*nu-1.0)*std::log(0.5*x)-
		  0.5*x-TRNG::ln_Gamma(0.5*nu))*0.5;
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with Student's \f$t\f$ distribution. This distribution is defined as
    \f[
    p(x)=\dfrac{\Gamma\left(\frac{\nu-1}{2}\right)}
    {\sqrt{\nu\pi}\Gamma\left(\frac{\nu}{2}\right)}
    \left(1+\frac{x^2}{\nu}\right)^{-\frac{\nu+1}{2}}\,.
    \f]
    \return \f$p(x)\f$
    \param x \f$x\f$
    \param nu parameter \f$\nu\f$, \f$\nu\ge1\f$
    \exception error if \f$\nu\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::Student_t_dist_pdf(double x, double nu) {
  if (nu<=0.0)
    throw TRNG::error("parameter less than or equal zero in TRNG::Student_t_dist_pdf");
  const double pi=3.14159265358979324;
  double t=(nu+1.0)*0.5;
  return std::exp(TRNG::ln_Gamma(t)-TRNG::ln_Gamma(nu/2.0))/
    std::sqrt(pi*nu)*std::pow((1.0+x*x/nu), -t);
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with binomial distribution. This distribution is defined as
    \f[
    p(k)=\frac{n!}{k!(n-k)!}p^k(1-p)^{n-k}\,,\qquad 0\le k\le n\,.
    \f]
    \return \f$p(k)\f$
    \param k parameter \f$k\f$
    \param n number of trails \f$n\f$
    \param p probability \f$p\f$ in each trail
    \exception error if \f$p\le 0\f$ or \f$p>1\f$ or \f$n\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::binomial_dist_pdf(long k, long n, double p=0.5) {
  if (p<=0.0 || p>1.0)
    throw TRNG::error("probability <=0.0 or >1.0 in TRNG::binomial_dist_pdf");
  if (n<1l)
    throw TRNG::error("less than one trail TRNG::binomial_dist_pdf");
  if (k<0 || k>n)
    return 0.0;
  return TRNG::binomial_coeff(n, k)*
    std::pow(p, static_cast<double>(k))*
    std::pow(1.0-p, static_cast<double>(n-k));
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with poisson distribution. This distribution is defined as
    \f[
    p(k)=
    \begin{cases}
    \dfrac{\mu^k}{k!}\mathrm{e}^{-\mu} & k \ge 0 \\
    0 & \text{else}
    \end{cases}
    \f]
    with mean \f$\mu\ge 0\f$.
    \return \f$p(k)\f$
    \param k parameter \f$k\f$
    \param mu mean \f$\mu\f$
    \author Heiko Bauke
*/
double TRNG::poisson_dist_pdf(long k, double mu) {
  if (k<0l)
    return 0.0;
  return std::pow(mu, static_cast<double>(k))*
    std::exp(-mu-TRNG::ln_factorial(k));
}

// ---------------------------------------------------------------------

//! probalility density
/*!
    This function calculates the probalility density for a random variate 
    with geometric distribution. This distribution is defined as
    \f[
    p(k)=q(1-q)^{k-1}\,,\qquad k\ge 1\,.
    \f]
    \return \f$p(k)\f$
    \param q probability \f$q\f$ 
    \param k parameter \f$k\f$
    \exception error if \f$p\le 0\f$ or \f$p>1\f$ or \f$n\le 0\f$
    \sa TRNG::error
    \author Heiko Bauke
*/
double TRNG::geometric_dist_pdf(long k, double q) {
  if (q<=0.0 ||q>1.0)
    throw TRNG::error("parameter out of range in TRNG::RNG::geometric_dist_pdf");
  if (k<=0l)
    return 0.0;
  if (k==1l)
    return q;
  return q*std::pow(1.0-q, static_cast<double>(k)-1.0);
}
