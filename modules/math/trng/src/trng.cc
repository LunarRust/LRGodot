// ---------------------------------------------------------------------
// Time-stamp: <Samstag, 20.09.2003, 23:04:38; edited by bauke>
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

#include <trng.h>


void TRNG::generic_MLCG::backward(void) {
  r=static_cast<long>
    ((static_cast<long long>(TRNG::modulo_invers(a, modulus))*
      static_cast<long long>(r))%modulus);
}

const char * TRNG::generic_MLCG::name(void) {
  return "generic_MLCG";
}

void TRNG::generic_MLCG::reset(void) {
  a=a_save;
  modulus=modulus_save;
  max_val=modulus-1l;;
  max_val2=max_val/2l;
}

void TRNG::generic_MLCG::seed(long s) {
  ++s;
  s%=modulus;
  if (s<=0l)
    throw error("bad seed for TRNG::generic_MLCG::seed");
  r=s;
}

void TRNG::generic_MLCG::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::generic_MLCG::split");
  if (s>1l) {
    long b=1l;
    long c=a;
    for (long i=0l; i<=n; ++i)
      rand();
    while (s>0) {
      if (s&1l==1l)
	b=static_cast<long>((static_cast<long long>(c)*
			     static_cast<long long>(b))%modulus);
      s>>=1l;
      c=static_cast<long>((static_cast<long long>(c)*
			   static_cast<long long>(c))%modulus);
    }
    a=b;
    backward();
  }
}
    
void TRNG::generic_MLCG::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::generic_MLCG::jump2");
  long t=a;
  for (long i=0; i<s; ++i)
    a=static_cast<long>((static_cast<long long>(a)*
			 static_cast<long long>(a))%modulus);
  rand();
  a=t;
}
    
void TRNG::generic_MLCG::save_status(std::vector<long> &s) {
  s.resize(6);
  s[0]=type;
  s[1]=a;
  s[2]=r;
  s[3]=modulus;
  s[4]=a_save;
  s[5]=modulus_save;
}
    
void TRNG::generic_MLCG::load_status(const std::vector<long> &s) {
  if ((s.size()!=6) || (s[0]!=type))
    throw error("TRNG::generic_MLCG::load_status wrong parameter");
  a=s[1];
  r=s[2];
  modulus=s[3];
  a_save=s[4];
  modulus_save=s[5];
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::generic_MLCG> & TRNG::generic_MLCG::operator=(TRNG::RNG<TRNG::generic_MLCG> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::generic_MLCG::generic_MLCG(long a_, long modulus_, long seed_) {
  a_save=a_;
  modulus_save=modulus_;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

const char * TRNG::ParkMiller::name(void) {
  return "ParkMiller";
}
    
void TRNG::ParkMiller::reset(void) {
  R.reset();
  max_val=R.max()-1l;
  max_val2=max_val/2l;
}

void TRNG::ParkMiller::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::ParkMiller::seed");
  R.seed(s);
}

void TRNG::ParkMiller::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::ParkMiller::split");
  R.split(s, n);
}

void TRNG::ParkMiller::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::ParkMiller::jump2");
  R.jump2(s);
}

void TRNG::ParkMiller::save_status(std::vector<long> &s) {
  R.save_status(s);
  s[0]=type;
}

void TRNG::ParkMiller::load_status(const std::vector<long> &s) {
  if ((s.size()!=6) || (s[0]!=type))
    throw error("TRNG::ParkMiller::load_status wrong parameter");
  std::vector<long> s_(s);
  if (s_.size()>0)
    s_[0]=generic_MLCG_t;
  R.load_status(s_);
  max_val=R.max()-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::ParkMiller> & TRNG::ParkMiller::operator=(TRNG::RNG<TRNG::ParkMiller> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::ParkMiller::ParkMiller(TRNG::ParkMiller_param param, long seed_) :
  R(param.a, param.modulus, seed_) {
  reset();
}

// ----------------------------------------------------------------------

void TRNG::LCG32::backward(void) {
  for (long i=0l; i<32l; ++i)
    jump2(i);
}

const char * TRNG::LCG32::name(void) {
  return "LCG32";
}

void TRNG::LCG32::reset(void) {
  a=a_save;
  b=b_save;
  max_val=0x7fffffffl;
  max_val2=max_val/2l;
}

void TRNG::LCG32::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::LCG32::seed");
  r=static_cast<unsigned long>(s)*2ul+1ul;
}
    
void TRNG::LCG32::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::LCG32::split");
  if (s>1l) {
    unsigned long t1=1ul;
    unsigned long t2=0ul;
    for (int i=0l; i<=n; ++i)
      rand();
    for (long i=0l; i<s; ++i) {
      t2+=t1;
      t1*=a;
    }
    b*=t2;
    a=t1;
    backward();
  }
}

void TRNG::LCG32::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::LCG32::jump2");
  unsigned long t1=a;
  for (long i=0l; i<s; ++i)
    t1*=t1;
  unsigned long t2=1ul;
  unsigned long t3=a;
  while (s>0l) {
    t2*=(1ul+t3);
    t3*=t3;
    --s;
  }
  r=r*t1+t2*b;
}

void TRNG::LCG32::save_status(std::vector<long> &s) {
  s.resize(6);
  s[0]=type;
  s[1]=a;  
  s[2]=b;  
  s[3]=r;
  s[4]=a_save;
  s[5]=b_save;
}

void TRNG::LCG32::load_status(const std::vector<long> &s) {
  if ((s.size()!=6) || (s[0]!=type))
    throw error("TRNG::LCG32::load_status wrong parameter");
  a=s[1];
  b=s[2];
  r=s[3];
  a_save=s[4];
  b_save=s[5];
}
    
TRNG::RNG<TRNG::LCG32> & TRNG::LCG32::operator=(TRNG::RNG<TRNG::LCG32> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::LCG32::LCG32(TRNG::LCG32_param param, long seed_) {
  a_save=param.a;
  b_save=param.b;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

void TRNG::LCG64::backward(void) {
  for (long i=0l; i<64l; ++i)
    jump2(i);
}
    
const char * TRNG::LCG64::name(void) {
  return "LCG64";
}

void TRNG::LCG64::reset(void) {
  a=a_save;
  b=b_save;
  max_val=0x7fffffffl;
  max_val2=max_val/2l;
}

void TRNG::LCG64::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::LCG64::seed");
  r=(static_cast<unsigned long long>(s)<<32)+1ull;
}

void TRNG::LCG64::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::LCG64::split");
  if (s>1l) {
    unsigned long long t1=1ull;
    unsigned long long t2=0ull;
    for (long i=0l; i<=n; ++i)
      rand();
    for (long i=0l; i<s; ++i) {
      t2+=t1;
      t1*=a;
    }
    b*=t2;
    a=t1;
    backward();
  }
}

void TRNG::LCG64::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::LCG64::jump2");
  unsigned long long t1=a;
  for (long i=0l; i<s; ++i)
    t1*=t1;
  unsigned long long t2=1ull;
  unsigned long long t3=a;
  while (s>0l) {
    t2*=(1ull+t3);
    t3*=t3;
    --s;
  }
  r=r*t1+t2*b;
}
    
void TRNG::LCG64::save_status(std::vector<long> &s) {
  s.resize(11);
  s[0]=type;
  s[1]=static_cast<long>(a>>32);
  s[2]=static_cast<long>(a&0xffffffffull);
  s[3]=static_cast<long>(b>>32);
  s[4]=static_cast<long>(b&0xffffffffull);
  s[5]=static_cast<long>(r>>32);
  s[6]=static_cast<long>(r&0xffffffffull);
  s[7]=static_cast<long>(a_save>>32);
  s[8]=static_cast<long>(a_save&0xffffffffull);
  s[9]=static_cast<long>(b_save>>32);
  s[10]=static_cast<long>(b_save&0xffffffffull);
}
    
void TRNG::LCG64::load_status(const std::vector<long> &s) {
  if ((s.size()!=11) || (s[0]!=type))
    throw error("TRNG::LCG64::load_status wrong parameter");
  a=(static_cast<unsigned long long>(s[1])<<32)|
    (static_cast<unsigned long long>
     (static_cast<unsigned long>(s[2])));
  b=(static_cast<unsigned long long>(s[3])<<32)|
    (static_cast<unsigned long long>
     (static_cast<unsigned long>(s[4])));
  r=(static_cast<unsigned long long>(s[5])<<32)|
    (static_cast<unsigned long long>
     (static_cast<unsigned long>(s[6])));
  a_save=(static_cast<unsigned long long>(s[7])<<32)|
    (static_cast<unsigned long long>
     (static_cast<unsigned long>(s[8])));
  b_save=(static_cast<unsigned long long>(s[9])<<32)|
    (static_cast<unsigned long long>
     (static_cast<unsigned long>(s[10])));
}

TRNG::RNG<TRNG::LCG64> & TRNG::LCG64::operator=(TRNG::RNG<TRNG::LCG64> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::LCG64::LCG64(TRNG::LCG64_param param, long seed_) {
  a_save=param.a;
  b_save=param.b;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

void TRNG::MRG2::backward(void) {
  long t;
  if (a2!=0l) {
    t=r1;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r2))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a2, modulus)))%modulus);
  } else if (a1!=0l) {
    t=r2;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a1, modulus)))%modulus);    
  } else
    t=0l;
  r1=r2;  r2=t;
}

const char * TRNG::MRG2::name(void) {
  return "MRG2";
}
    
void TRNG::MRG2::reset(void) {
  a1=a1_save;  
  a2=a2_save;
  modulus=modulus_save;
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

void TRNG::MRG2::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::MRG2::seed");
  r1=s%modulus;  r2=1l;
}
    
void TRNG::MRG2::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::MRG2::split");
  long q0, q1, q2, q3;
  if (s>1l) {
    for (long i=0l; i<=n; ++i) 
      q0=rand();
    for (long i=0l; i<s; ++i)
      q1=rand();
    for (long i=0l; i<s; ++i)
      q2=rand();
    for (long i=0l; i<s; ++i)
      q3=rand();
    std::vector<long> a(2), b(4);
    a[0]=q2;  b[0]=q1;  b[1]=q0;
    a[1]=q3;  b[2]=q2;  b[3]=q1;
    gauss(b, a, modulus);
    a1=a[0];  a2=a[1];
    r1=q1;    r2=q0;
    backward();
    backward();
  }
}

void TRNG::MRG2::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::MRG2::jump2");
  std::vector<long> b(4), c(4), d(2), r(2);
  long t1=a1, t2=a2;
  b[0]=a1;  b[1]=a2;
  b[2]=1l;  b[3]=0l;
  for (long i=0l; i<s; ++i)
    if ((i&1l)==0l)
      matrix_mult(b, b, c, modulus);
    else 
      matrix_mult(c, c, b, modulus);
  r[0]=r1;  r[1]=r2;
  if ((s&1l)==0l)
    matrix_vec_mult(b, r, d, modulus);
  else
    matrix_vec_mult(c, r, d, modulus);
  r1=d[0];  r2=d[1];
  a1=t1;    a2=t2;
}

void TRNG::MRG2::save_status(std::vector<long> &s) {
  s.resize(9);
  s[0]=type;
  s[1]=a1;  s[2]=a2;
  s[3]=r1;  s[4]=r2;
  s[5]=modulus;
  s[6]=a1_save;  s[7]=a2_save;
  s[8]=modulus_save;
}

void TRNG::MRG2::load_status(const std::vector<long> &s) {
  if ((s.size()!=9) || (s[0]!=type))
    throw error("TRNG::MRG2::load_status wrong parameter");
  a1=s[1];  a2=s[2];
  r1=s[3];  r2=s[4];
  modulus=s[5];
  a1_save=s[6];  a2_save=s[7];
  modulus_save=s[8];
  max_val=modulus-1l;
  max_val2=max_val/2l;
}
    
TRNG::RNG<TRNG::MRG2> & TRNG::MRG2::operator=(TRNG::RNG<TRNG::MRG2> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::MRG2::MRG2(TRNG::MRG2_param param, long seed_) {
  a1_save=param.a1;
  a2_save=param.a2;
  modulus_save=param.modulus;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

void TRNG::MRG3::backward(void) {
  long t;
  if (a3!=0l) {
    t=r1;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r2))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a2)*
			  static_cast<long long>(r3))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a3, modulus)))%modulus);
  } else if (a2!=0l) {
    t=r2;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r3))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a2, modulus)))%modulus);
  } else if (a1!=0l) {
    t=r3;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a1, modulus)))%modulus);    
  } else
    t=0l;
  r1=r2;  r2=r3;  r3=t;
}
    
const char * TRNG::MRG3::name(void) {
  return "MRG3";
}
    
void TRNG::MRG3::reset(void) {
  a1=a1_save;  
  a2=a2_save;
  a3=a3_save;
  modulus=modulus_save;
  max_val=modulus-1l;
  max_val2=max_val/2l;
}
    
void TRNG::MRG3::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::MRG3::seed");
  r1=s%modulus;  r2=1l;  r3=1l;
}
    
void TRNG::MRG3::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::MRG3::split");
  long q0, q1, q2, q3, q4, q5;
  if (s>1l) {
    for (long i=0l; i<=n; ++i) 
      q0=rand();
    for (long i=0l; i<s; ++i)
      q1=rand();
    for (long i=0l; i<s; ++i)
      q2=rand();
    for (long i=0l; i<s; ++i)
      q3=rand();
    for (long i=0l; i<s; ++i)
      q4=rand();
    for (long i=0l; i<s; ++i)
      q5=rand();
    std::vector<long> a(3), b(9);	
    a[0]=q3;  b[0]=q2;  b[1]=q1;  b[2]=q0;
    a[1]=q4;  b[3]=q3;  b[4]=q2;  b[5]=q1;
    a[2]=q5;  b[6]=q4;  b[7]=q3;  b[8]=q2;
    gauss(b, a, modulus);
    a1=a[0];  a2=a[1];  a3=a[2];
    r1=q2;    r2=q1;    r3=q0;
    backward();
    backward();
    backward();
  }
}
    
void TRNG::MRG3::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::MRG3::jump2");
  std::vector<long> b(9), c(9), d(3), r(3);
  long t1=a1, t2=a2, t3=a3;
  b[0]=a1;  b[1]=a2;  b[2]=a3;
  b[3]=1l;  b[4]=0l;  b[5]=0l;
  b[6]=0l;  b[7]=1l;  b[8]=0l;
  for (long i=0l; i<s; ++i)
    if ((i&1l)==0l)
      matrix_mult(b, b, c, modulus);
    else 
      matrix_mult(c, c, b, modulus);
  r[0]=r1;  r[1]=r2;  r[2]=r3;
  if ((s&1l)==0l)
    matrix_vec_mult(b, r, d, modulus);
  else
    matrix_vec_mult(c, r, d, modulus);
  r1=d[0];  r2=d[1];  r3=d[2];
  a1=t1;    a2=t2;    a3=t3;
}
    
void TRNG::MRG3::save_status(std::vector<long> &s) {
  s.resize(12);
  s[0]=type;
  s[1]=a1;  s[2]=a2;  s[3]=a3;
  s[4]=r1;  s[5]=r2;  s[6]=r3;
  s[7]=modulus;
  s[8]=a1_save;  s[9]=a2_save;  s[10]=a3_save;
  s[11]=modulus_save;
}

void TRNG::MRG3::load_status(const std::vector<long> &s) {
  if ((s.size()!=12) || (s[0]!=type))
    throw error("TRNG::MRG3::load_status wrong parameter");
  a1=s[1];  a2=s[2];  a3=s[3];
  r1=s[4];  r2=s[5];  r3=s[6];
  modulus=s[7];
  a1_save=s[8]; a2_save=s[9]; a3_save=s[10];
  modulus_save=s[11];
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::MRG3> & TRNG::MRG3::operator=(TRNG::RNG<TRNG::MRG3> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::MRG3::MRG3(TRNG::MRG3_param param, long seed_) {
  a1_save=param.a1;
  a2_save=param.a2;
  a3_save=param.a3;
  modulus_save=param.modulus;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

void TRNG::MRG4::backward(void) {
  long t;
  if (a4!=0l) {
    t=r1;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r2))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a2)*
			  static_cast<long long>(r3))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a3)*
			  static_cast<long long>(r4))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a4, modulus)))%modulus);
  } else if (a3!=0l) {
    t=r2;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r3))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a2)*
			  static_cast<long long>(r4))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a3, modulus)))%modulus);
  } else if (a2!=0l) {
    t=r3;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r4))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a2, modulus)))%modulus);
  } else if (a1!=0l) {
    t=r4;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a1, modulus)))%modulus);    
  } else
    t=0l;
  r1=r2;  r2=r3;  r3=r4;  r4=t;
}

const char * TRNG::MRG4::name(void) {
  return "MRG4";
}

void TRNG::MRG4::reset(void) {
  a1=a1_save;  
  a2=a2_save;
  a3=a3_save;
  a4=a4_save;
  modulus=modulus_save;
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

void TRNG::MRG4::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::MRG4::seed");
  r1=s%modulus;  r2=1l;  r3=1l;  r4=1l;
}

void TRNG::MRG4::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::MRG4::split");
  long q0, q1, q2, q3, q4, q5, q6, q7;
  if (s>1l) {
    for (long i=0l; i<=n; ++i) 
      q0=rand();
    for (long i=0l; i<s; ++i)
      q1=rand();
    for (long i=0l; i<s; ++i)
      q2=rand();
    for (long i=0l; i<s; ++i)
      q3=rand();
    for (long i=0l; i<s; ++i)
      q4=rand();
    for (long i=0l; i<s; ++i)
      q5=rand();
    for (long i=0l; i<s; ++i)
      q6=rand();
    for (long i=0l; i<s; ++i)
      q7=rand();
    std::vector<long> a(4), b(16);
    a[ 0]=q4;  b[ 0]=q3;  b[ 1]=q2;  b[ 2]=q1;  b[ 3]=q0;
    a[ 1]=q5;  b[ 4]=q4;  b[ 5]=q3;  b[ 6]=q2;  b[ 7]=q1;
    a[ 2]=q6;  b[ 8]=q5;  b[ 9]=q4;  b[10]=q3;  b[11]=q2;
    a[ 3]=q7;  b[12]=q6;  b[13]=q5;  b[14]=q4;  b[15]=q3;
    gauss(b, a, modulus);
    a1=a[0];   a2=a[1];   a3=a[2];   a4=a[3];   
    r1=q3;     r2=q2;     r3=q1;     r4=q0;
    backward();
    backward();
    backward();
    backward();
  }
}
    
void TRNG::MRG4::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::MRG4::jump2");
  std::vector<long> b(16), c(16), d(4), r(4);
  long t1=a1, t2=a2, t3=a3, t4=a4;
  b[ 0]=a1;  b[ 1]=a2;  b[ 2]=a3;  b[ 3]=a4;
  b[ 4]=1l;  b[ 5]=0l;  b[ 6]=0l;  b[ 7]=0l;
  b[ 8]=0l;  b[ 9]=1l;  b[10]=0l;  b[11]=0l;
  b[12]=0l;  b[13]=0l;  b[14]=1l;  b[15]=0l;
  for (long i=0l; i<s; ++i)
    if ((i&1l)==0l)
      matrix_mult(b, b, c, modulus);
    else 
      matrix_mult(c, c, b, modulus);
  r[0]=r1;  r[1]=r2;  r[2]=r3;  r[3]=r4;  
  if ((s&1l)==0l)
    matrix_vec_mult(b, r, d, modulus);
  else
    matrix_vec_mult(c, r, d, modulus);
  r1=d[0];  r2=d[1];  r3=d[2];  r4=d[3];
  a1=t1;    a2=t2;    a3=t3;    a4=t4;
}

void TRNG::MRG4::save_status(std::vector<long> &s) {
  s.resize(15);
  s[0]=type;
  s[1]=a1;  s[2]=a2;  s[3]=a3;  s[4]=a4;
  s[5]=r1;  s[6]=r2;  s[7]=r3;  s[8]=r4; 
  s[9]=modulus;
  s[10]=a1_save;  s[11]=a2_save;  s[12]=a3_save;  s[13]=a4_save;
  s[14]=modulus_save;
}

void TRNG::MRG4::load_status(const std::vector<long> &s) {
  if ((s.size()!=15) || (s[0]!=type))
    throw error("TRNG::MRG4::load_status wrong parameter");
  a1=s[1];  a2=s[2];  a3=s[3];  a4=s[4];
  r1=s[5];  r2=s[6];  r3=s[7];  r4=s[8];
  modulus=s[9];
  a1_save=s[10];  a2_save=s[11];  a3_save=s[12];  a4_save=s[13];
  modulus_save=s[14];
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::MRG4> & TRNG::MRG4::operator=(TRNG::RNG<TRNG::MRG4> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::MRG4::MRG4(TRNG::MRG4_param param, long seed_) {
  a1_save=param.a1;
  a2_save=param.a2;
  a3_save=param.a3;
  a4_save=param.a4;
  modulus_save=param.modulus;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

void TRNG::MRG5::backward(void) {
  long t;
  if (a5!=0l) {
    t=r1;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r2))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a2)*
			  static_cast<long long>(r3))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a3)*
			  static_cast<long long>(r4))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a4)*
			  static_cast<long long>(r5))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a5, modulus)))%modulus);
  } else if (a4!=0l) {
    t=r2;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r3))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a2)*
			  static_cast<long long>(r4))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a3)*
			  static_cast<long long>(r5))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a4, modulus)))%modulus);
  } else if (a3!=0l) {
    t=r3;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r4))%modulus);
    if (t<0l)
      t+=modulus;
    t-=static_cast<long>((static_cast<long long>(a2)*
			  static_cast<long long>(r5))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a3, modulus)))%modulus);
  } else if (a2!=0l) {
    t=r4;
    t-=static_cast<long>((static_cast<long long>(a1)*
			  static_cast<long long>(r5))%modulus);
    if (t<0l)
      t+=modulus;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a2, modulus)))%modulus);
  } else if (a1!=0l) {
    t=r5;
    t=static_cast<long>((static_cast<long long>(t)*
			 static_cast<long long>
			 (modulo_invers(a1, modulus)))%modulus);    
  } else
    t=0l;
  r1=r2;  r2=r3;  r3=r4;  r4=r5;  r5=t;
}

const char * TRNG::MRG5::name(void) {
  return "MRG5";
}

void TRNG::MRG5::reset(void) {
  a1=a1_save;  
  a2=a2_save;
  a3=a3_save;
  a4=a4_save;
  a5=a5_save;
  modulus=modulus_save;
  m2_2=2ull*static_cast<unsigned long long>(modulus)*
    static_cast<unsigned long long>(modulus);
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

void TRNG::MRG5::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::MRG5::seed");
  r1=s%modulus;  r2=1l;  r3=1l;  r4=1l; r5=1l;
}

void TRNG::MRG5::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::MRG5::split");
  long q0, q1, q2, q3, q4, q5, q6, q7, q8, q9;
  if (s>1l) {
    for (long i=0l; i<=n; ++i) 
      q0=rand();
    for (long i=0l; i<s; ++i)
      q1=rand();
    for (long i=0l; i<s; ++i)
      q2=rand();
    for (long i=0l; i<s; ++i)
      q3=rand();
    for (long i=0l; i<s; ++i)
      q4=rand();
    for (long i=0l; i<s; ++i)
      q5=rand();
    for (long i=0l; i<s; ++i)
      q6=rand();
    for (long i=0l; i<s; ++i)
      q7=rand();
    for (long i=0l; i<s; ++i)
      q8=rand();
    for (long i=0l; i<s; ++i)
      q9=rand();
    std::vector<long> a(5), b(25);
    a[ 0]=q5;  b[ 0]=q4;  b[ 1]=q3;  b[ 2]=q2;  b[ 3]=q1;  b[ 4]=q0;
    a[ 1]=q6;  b[ 5]=q5;  b[ 6]=q4;  b[ 7]=q3;  b[ 8]=q2;  b[ 9]=q1;
    a[ 2]=q7;  b[10]=q6;  b[11]=q5;  b[12]=q4;  b[13]=q3;  b[14]=q2;
    a[ 3]=q8;  b[15]=q7;  b[16]=q6;  b[17]=q5;  b[18]=q4;  b[19]=q3;
    a[ 4]=q9;  b[20]=q8;  b[21]=q7;  b[22]=q6;  b[23]=q5;  b[24]=q4;
    gauss(b, a, modulus);
    a1=a[0];   a2=a[1];   a3=a[2];   a4=a[3];   a5=a[4];   
    r1=q4;     r2=q3;     r3=q2;     r4=q1;     r5=q0;
    backward();
    backward();
    backward();
    backward();
    backward();
  }
}
    
void TRNG::MRG5::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::MRG5::jump2");
  std::vector<long> b(25), c(25), d(5), r(5);
  long t1=a1, t2=a2, t3=a3, t4=a4, t5=a5;
  b[ 0]=a1;  b[ 1]=a2;  b[ 2]=a3;  b[ 3]=a4;  b[ 4]=a5;
  b[ 5]=1l;  b[ 6]=0l;  b[ 7]=0l;  b[ 8]=0l;  b[ 9]=0l;
  b[10]=0l;  b[11]=1l;  b[12]=0l;  b[13]=0l;  b[14]=0l;
  b[15]=0l;  b[16]=0l;  b[17]=1l;  b[18]=0l;  b[19]=0l;
  b[20]=0l;  b[21]=0l;  b[22]=0l;  b[23]=1l;  b[24]=0l;
  for (long i=0l; i<s; ++i)
    if ((i&1l)==0l)
      matrix_mult(b, b, c, modulus);
    else 
      matrix_mult(c, c, b, modulus);
  r[0]=r1;  r[1]=r2;  r[2]=r3;  r[3]=r4;  r[4]=r5;
  if ((s&1l)==0l)
    matrix_vec_mult(b, r, d, modulus);
  else
    matrix_vec_mult(c, r, d, modulus);
  r1=d[0];  r2=d[1];  r3=d[2];  r4=d[3];  r5=d[4];
  a1=t1;    a2=t2;    a3=t3;    a4=t4;    a5=t5;
}

void TRNG::MRG5::save_status(std::vector<long> &s) {
  s.resize(18);
  s[0]=type;
  s[1]=a1;  s[2]=a2;  s[3]=a3;  s[4]=a4;  s[5]=a5;
  s[6]=r1;  s[7]=r2;  s[8]=r3;  s[9]=r4;  s[10]=r5; 
  s[11]=modulus;
  s[12]=a1_save;  s[13]=a2_save;  s[14]=a3_save;  s[15]=a4_save;  s[16]=a5_save;
  s[17]=modulus_save;
}

void TRNG::MRG5::load_status(const std::vector<long> &s) {
  if ((s.size()!=18) || (s[0]!=type))
    throw error("TRNG::MRG5::load_status wrong parameter");
  a1=s[1];  a2=s[2];  a3=s[3];  a4=s[4];  a5=s[5];
  r1=s[6];  r2=s[7];  r3=s[8];  r4=s[9];  r5=s[10];
  modulus=s[11];
  a1_save=s[12];  a2_save=s[13];  a3_save=s[14];  a4_save=s[15];  a5_save=s[16];
  modulus_save=s[17];
  m2_2=2ull*static_cast<unsigned long long>(modulus)*
    static_cast<unsigned long long>(modulus);
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::MRG5> & TRNG::MRG5::operator=(TRNG::RNG<TRNG::MRG5> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}


TRNG::MRG5::MRG5(TRNG::MRG5_param param, long seed_) {
  a1_save=param.a1;
  a2_save=param.a2;
  a3_save=param.a3;
  a4_save=param.a4;
  a5_save=param.a5;
  modulus_save=param.modulus;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

const char * TRNG::YARNLCG64::name(void) {
  return "YARNLCG64";
}

void TRNG::YARNLCG64::reset(void) {
  R.reset();
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(g_save, 2147483647l);
}

void TRNG::YARNLCG64::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::YARNLCG64::seed");
  R.seed(s);
}

void TRNG::YARNLCG64::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::YARNLCG64::split");
  R.split(s, n);
}

void TRNG::YARNLCG64::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::YARNLCG64::jump2");
  R.jump2(s);
}

void TRNG::YARNLCG64::save_status(std::vector<long> &s) {
  R.save_status(s);
  s[0]=type;
  s.push_back(g());
}

void TRNG::YARNLCG64::load_status(const std::vector<long> &s) {
  if ((s.size()!=12) || (s[0]!=type))
    throw error("TRNG::YARNLCG64::load_status wrong parameter");
  std::vector<long> s_(s);
  const long t=s_.back();
  s_.pop_back();
  if (s_.size()>0)
    s_[0]=LCG64_t;
  R.load_status(s_);
  g.set(t, 2147483647l);
}

TRNG::RNG<TRNG::YARNLCG64> & TRNG::YARNLCG64::operator=(TRNG::RNG<TRNG::YARNLCG64> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::YARNLCG64::YARNLCG64(TRNG::YARNLCG64_param param, long seed_) :
  R(TRNG::LCG64_param(param.a, param.b), seed_),
  g(param.g, 2147483647l),
  g_save(param.g) {
  reset();
}

// ----------------------------------------------------------------------

const char * TRNG::YARN2::name(void) {
  return "YARN2";
}

void TRNG::YARN2::reset(void) {
  R.reset();
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(g_save, max_val+1);
}

void TRNG::YARN2::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::YARN2::seed");
  R.seed(s);
}

void TRNG::YARN2::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::YARN2::split");
  R.split(s, n);
}

void TRNG::YARN2::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::YARN2::jump2");
  R.jump2(s);
}

void TRNG::YARN2::save_status(std::vector<long> &s) {
  R.save_status(s);
  s[0]=type;
  s.push_back(g());
}

void TRNG::YARN2::load_status(const std::vector<long> &s) {
  if ((s.size()!=10) || (s[0]!=type))
    throw error("TRNG::YARN2::load_status wrong parameter");
  std::vector<long> s_(s);
  const long t=s_.back();
  s_.pop_back();
  if (s_.size()>0)
    s_[0]=MRG2_t;
  R.load_status(s_);
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(t, s_[5]);
}

TRNG::RNG<TRNG::YARN2> & TRNG::YARN2::operator=(TRNG::RNG<TRNG::YARN2> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::YARN2::YARN2(TRNG::YARN2_param param, long seed_) :
  R(TRNG::MRG2_param(param.a1, param.a2, param.modulus), seed_),
  g(param.g, param.modulus),
  g_save(param.g) {
  reset();
}

// ----------------------------------------------------------------------

const char * TRNG::YARN3::name(void) {
  return "YARN3";
}

void TRNG::YARN3::reset(void) {
  R.reset();
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(g_save, max_val+1);
}

void TRNG::YARN3::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::YARN3::seed");
  R.seed(s);
}

void TRNG::YARN3::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::YARN3::split");
  R.split(s, n);
}

void TRNG::YARN3::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::YARN3::jump2");
  R.jump2(s);
}

void TRNG::YARN3::save_status(std::vector<long> &s) {
  R.save_status(s);
  s[0]=type;
  s.push_back(g());
}

void TRNG::YARN3::load_status(const std::vector<long> &s) {
  if ((s.size()!=13) || (s[0]!=type))
    throw error("TRNG::YARN3::load_status wrong parameter");
  std::vector<long> s_(s);
  const long t=s_.back();
  s_.pop_back();
  if (s_.size()>0)
    s_[0]=MRG3_t;
  R.load_status(s_);
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(t, s_[7]);
}

TRNG::RNG<TRNG::YARN3> & TRNG::YARN3::operator=(TRNG::RNG<TRNG::YARN3> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::YARN3::YARN3(TRNG::YARN3_param param, long seed_) :
  R(TRNG::MRG3_param(param.a1, param.a2, param.a3, param.modulus), seed_),
  g(param.g, param.modulus),
  g_save(param.g) {
  reset();
}

// ----------------------------------------------------------------------

const char * TRNG::YARN4::name(void) {
  return "YARN4";
}

void TRNG::YARN4::reset(void) {
  R.reset();
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(g_save, max_val+1);
}

void TRNG::YARN4::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::YARN4::seed");
  R.seed(s);
}

void TRNG::YARN4::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::YARN4::split");
  R.split(s, n);
}

void TRNG::YARN4::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::YARN4::jump2");
  R.jump2(s);
}

void TRNG::YARN4::save_status(std::vector<long> &s) {
  R.save_status(s);
  s[0]=type;
  s.push_back(g());
}

void TRNG::YARN4::load_status(const std::vector<long> &s) {
  if ((s.size()!=16) || (s[0]!=type))
    throw error("TRNG::YARN4::load_status wrong parameter");
  std::vector<long> s_(s);
  const long t=s_.back();
  s_.pop_back();
  if (s_.size()>0)
    s_[0]=MRG4_t;
  R.load_status(s_);
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(t, s_[9]);
}

TRNG::RNG<TRNG::YARN4> & TRNG::YARN4::operator=(TRNG::RNG<TRNG::YARN4> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::YARN4::YARN4(TRNG::YARN4_param param, long seed_) :
  R(TRNG::MRG4_param(param.a1, param.a2, param.a3, param.a4, param.modulus), seed_),
  g(param.g, param.modulus),
  g_save(param.g) {
  reset();
}

// ----------------------------------------------------------------------

const char * TRNG::YARN5::name(void) {
  return "YARN5";
}

void TRNG::YARN5::reset(void) {
  R.reset();
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(g_save, max_val+1);
}

void TRNG::YARN5::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::YARN5::seed");
  R.seed(s);
}

void TRNG::YARN5::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::YARN5::split");
  R.split(s, n);
}

void TRNG::YARN5::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::YARN5::jump2");
  R.jump2(s);
}

void TRNG::YARN5::save_status(std::vector<long> &s) {
  R.save_status(s);
  s[0]=type;
  s.push_back(g());
}

void TRNG::YARN5::load_status(const std::vector<long> &s) {
  if ((s.size()!=19) || (s[0]!=type))
    throw error("TRNG::YARN5::load_status wrong parameter");
  std::vector<long> s_(s);
  const long t=s_.back();
  s_.pop_back();
  if (s_.size()>0)
    s_[0]=MRG5_t;
  R.load_status(s_);
  max_val=R.max();
  max_val2=max_val/2l;
  g.set(t, s_[11]);
}

TRNG::RNG<TRNG::YARN5> & TRNG::YARN5::operator=(TRNG::RNG<TRNG::YARN5> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::YARN5::YARN5(TRNG::YARN5_param param, long seed_) :
  R(TRNG::MRG5_param(param.a1, param.a2, param.a3, param.a4, param.a5, param.modulus), seed_),
  g(param.g, param.modulus),
  g_save(param.g) {
  reset();
}

// ----------------------------------------------------------------------

const char * TRNG::CLCG2::name(void) {
  return "CLCG2";
}
    
void TRNG::CLCG2::reset(void) {
  R1.reset();
  R2.reset();
  modulus=R1.max();
  max_val=modulus-1l;
  max_val2=max_val/2l;
}
    
void TRNG::CLCG2::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::CLCG2::seed");
  R1.seed(s);
  R2.seed(s);
}

void TRNG::CLCG2::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::CLCG2::split");
  R1.split(s, n);
  R2.split(s, n);
}

void TRNG::CLCG2::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::CLCG2::jump2");
  R1.jump2(s);
  R2.jump2(s);
}

void TRNG::CLCG2::save_status(std::vector<long> &s) {
  std::vector<long> t;
  s.resize(11);
  R1.save_status(t);
  s[0]=type;
  s[1]=t[1];  s[2]=t[2];  s[3]=t[3];  s[4]=t[4];  s[5]=t[5];
  R2.save_status(t);
  s[6]=t[1];  s[7]=t[2];  s[8]=t[3];  s[9]=t[4];  s[10]=t[5];
}
    
void TRNG::CLCG2::load_status(const std::vector<long> &s) {
  std::vector<long> t(6);
  if ((s.size()!=11) || (s[0]!=type))
    throw error("TRNG::CLCG2::load_status wrong parameter");
  t[0]=generic_MLCG_t;
  t[1]=s[1];  t[2]=s[2];  t[3]=s[3];  t[4]=s[4];  t[5]=s[5];
  R1.load_status(t);
  t[1]=s[6];  t[2]=s[7];  t[3]=s[8];  t[4]=s[9];  t[5]=s[10];
  R2.load_status(t);    
  modulus=R1.max();
  max_val=modulus-1l;
  max_val2=max_val/2l;
}
    
TRNG::RNG<TRNG::CLCG2> & TRNG::CLCG2::operator=(TRNG::RNG<TRNG::CLCG2> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::CLCG2::CLCG2(TRNG::CLCG2_param param, long seed_) :
  R1(param.a1, param.modulus1, seed_), 
  R2(param.a2, param.modulus2, seed_) {
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

const char * TRNG::CLCG3::name(void) {
  return "CLCG3";
}

void TRNG::CLCG3::reset(void) {
  R1.reset();
  R2.reset();
  R3.reset();
  modulus=R1.max();
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

void TRNG::CLCG3::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::CLCG3::seed");
  R1.seed(s);
  R2.seed(s);
  R3.seed(s);
}

void TRNG::CLCG3::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::CLCG3::split");
  R1.split(s, n);
  R2.split(s, n);
  R3.split(s, n);
}

void TRNG::CLCG3::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::CLCG3::jump2");
  R1.jump2(s);
  R2.jump2(s);
  R3.jump2(s);
}
    
void TRNG::CLCG3::save_status(std::vector<long> &s) {
  std::vector<long> t;
  s.resize(16);
  R1.save_status(t);
  s[0]=type;
  s[1]=t[1];  s[2]=t[2];  s[3]=t[3];  s[4]=t[4];  s[5]=t[5];
  R2.save_status(t);
  s[6]=t[1];  s[7]=t[2];  s[8]=t[3];  s[9]=t[4];  s[10]=t[5];
  R3.save_status(t);
  s[11]=t[1];  s[12]=t[2];  s[13]=t[3];  s[14]=t[4];  s[15]=t[5];
}

void TRNG::CLCG3::load_status(const std::vector<long> &s) {
  std::vector<long> t(6);
  if ((s.size()!=16) || (s[0]!=type))
    throw error("TRNG::CLCG3::load_status wrong parameter");
  t[0]=generic_MLCG_t;
  t[1]=s[1];  t[2]=s[2];  t[3]=s[3];  t[4]=s[4];  t[5]=s[5];
  R1.load_status(t);
  t[1]=s[6];  t[2]=s[7];  t[3]=s[8];  t[4]=s[9];  t[5]=s[10];
  R2.load_status(t);    
  t[1]=s[11];  t[2]=s[12];  t[3]=s[13];  t[4]=s[14];  t[5]=s[15];
  R3.load_status(t);    
  modulus=R1.max();
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::CLCG3> & TRNG::CLCG3::operator=(TRNG::RNG<TRNG::CLCG3> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::CLCG3::CLCG3(TRNG::CLCG3_param param, long seed_) :
  R1(param.a1, param.modulus1, seed_), 
  R2(param.a2, param.modulus2, seed_), 
  R3(param.a3, param.modulus3, seed_) {
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

const char * TRNG::CLCG4::name(void) {
  return "CLCG4";
}

void TRNG::CLCG4::reset(void) {
  R1.reset();
  R2.reset();
  R3.reset();
  R4.reset();
  modulus=R1.max();
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

void TRNG::CLCG4::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::CLCG4::seed");
  R1.seed(s);
  R2.seed(s);
  R3.seed(s);
  R4.seed(s);
}

void TRNG::CLCG4::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::CLCG4::split");
  R1.split(s, n);
  R2.split(s, n);
  R3.split(s, n);
  R4.split(s, n);
}

void TRNG::CLCG4::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::CLCG4::jump2");
  R1.jump2(s);
  R2.jump2(s);
  R3.jump2(s);
  R4.jump2(s);
}

void TRNG::CLCG4::save_status(std::vector<long> &s) {
  std::vector<long> t;
  s.resize(21);
  R1.save_status(t);
  s[0]=type;
  s[1]=t[1];  s[2]=t[2];  s[3]=t[3];  s[4]=t[4];  s[5]=t[5];
  R2.save_status(t);
  s[6]=t[1];  s[7]=t[2];  s[8]=t[3];  s[9]=t[4];  s[10]=t[5];
  R3.save_status(t);
  s[11]=t[1];  s[12]=t[2];  s[13]=t[3];  s[14]=t[4];  s[15]=t[5];
  R4.save_status(t);
  s[16]=t[1];  s[17]=t[2];  s[18]=t[3];  s[19]=t[4];  s[20]=t[5];
}
    
void TRNG::CLCG4::load_status(const std::vector<long> &s) {
  std::vector<long> t(6);
  if ((s.size()!=21) || (s[0]!=type))
    throw error("TRNG::CLCG4::load_status wrong parameter");
  t[0]=generic_MLCG_t;
  t[1]=s[1];  t[2]=s[2];  t[3]=s[3];  t[4]=s[4];  t[5]=s[5];
  R1.load_status(t);
  t[1]=s[6];  t[2]=s[7];  t[3]=s[8];  t[4]=s[9];  t[5]=s[10];
  R2.load_status(t);    
  t[1]=s[11];  t[2]=s[12];  t[3]=s[13];  t[4]=s[14];  t[5]=s[15];
  R3.load_status(t);    
  t[1]=s[16];  t[2]=s[17];  t[3]=s[18];  t[4]=s[19];  t[5]=s[20];
  R4.load_status(t); 
  modulus=R1.max();
  max_val=modulus-1l;
  max_val2=max_val/2l;
}

TRNG::RNG<TRNG::CLCG4> & TRNG::CLCG4::operator=(TRNG::RNG<TRNG::CLCG4> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::CLCG4::CLCG4(TRNG::CLCG4_param param, long seed_) :
  R1(param.a1, param.modulus1, seed_), 
  R2(param.a2, param.modulus2, seed_), 
  R3(param.a3, param.modulus3, seed_), 
  R4(param.a4, param.modulus4, seed_) {
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

const char * TRNG::EINV::name(void) {
  return "EINV";
}
    
void TRNG::EINV::reset(void) {
  di=1l;
  a=a_save;
  b=b_save;
  modulus=modulus_save;
  max_val=modulus-1l;
  max_val2=max_val/2l;
}
    
void TRNG::EINV::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::EINV::seed");
  i=s%modulus;
}
    
void TRNG::EINV::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::EINV::split");
  if (s>1l) {
    di=static_cast<long>
      ((static_cast<long long>(s)*
	static_cast<long long>(di))%modulus);
    i=static_cast<long>
      ((static_cast<long long>(i)-
	static_cast<long long>(di)+
	static_cast<long long>(n)+1ll)%modulus);
  }  
}
    
void TRNG::EINV::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::EINV::jump2");
  long long t=1ll;
  for (long j=0l; j<s; ++j)
    t=(t+t)%modulus;
  t=t*di%modulus;
  i=static_cast<long>((t+static_cast<long long>(i))%modulus);
}

void TRNG::EINV::save_status(std::vector<long> &s) {
  s.resize(8);
  s[0]=type;
  s[1]=a;  s[2]=b;  s[3]=i;  s[4]=di;  
  s[5]=a_save;  s[6]=b_save;  s[7]=modulus_save;
}

void TRNG::EINV::load_status(const std::vector<long> &s) {
  if ((s.size()!=8) || (s[0]!=type))
    throw error("TRNG::EINV::load_status wrong parameter");
  a=s[1];  b=s[2];  i=s[3];  di=s[4];  
  a_save=s[5];  b_save=s[6];  modulus_save=s[7];
}

TRNG::RNG<TRNG::EINV> & TRNG::EINV::operator=(TRNG::RNG<TRNG::EINV> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::EINV::EINV(TRNG::EINV_param param, long seed_) {
  a_save=param.a;
  b_save=param.b;
  modulus_save=param.modulus;
  reset();
  seed(seed_);
}

// ----------------------------------------------------------------------

const char * TRNG::EINVLCG64::name(void) {
  return "EINVLCG64";
}

void TRNG::EINVLCG64::reset(void) {
  R1.reset();
  R2.reset();
  max_val=0x7fffffffl;
  max_val2=max_val/2l;
}

void TRNG::EINVLCG64::seed(long s) {
  if (s<0l)
    throw error("bad seed for TRNG::EINVLCG64::seed");
  R1.seed(s);
  R2.seed(s);
}

void TRNG::EINVLCG64::split(long s, long n) {
  if (s<1l || n>=s || n<0l)
    throw error("invalid argument for TRNG::EINVLCG64::split");
  R1.split(s, n);
  R2.split(s, n);
}

void TRNG::EINVLCG64::jump2(long s) {
  if (s<0l || s>63l)
    throw error("invalid argument for TRNG::EINVLCG64::jump2");
  R1.jump2(s);
  R2.jump2(s);
}

void TRNG::EINVLCG64::save_status(std::vector<long> &s) {
  std::vector<long> t;
  s.resize(18);
  R1.save_status(t);
  s[0]=type;
  s[1]=t[1];  s[2]=t[2];  s[3]=t[3];  s[4]=t[4];  
  s[5]=t[5];  s[6]=t[6];  s[7]=t[7];  s[8]=t[8];  
  s[9]=t[9];  s[10]=t[10];
  R2.save_status(t);
  s[11]=t[1];  s[12]=t[2];  s[13]=t[3];  s[14]=t[4]; 
  s[15]=t[5];  s[16]=t[6];  s[17]=t[7];
}

void TRNG::EINVLCG64::load_status(const std::vector<long> &s) {
  std::vector<long> t;
  if ((s.size()!=18) || (s[0]!=type))
    throw error("TRNG::EINVLCG64::load_status wrong parameter");
  t.resize(11);
  t[0]=LCG64_t;
  t[1]=s[1];  t[2]=s[2];  t[3]=s[3];  t[4]=s[4];
  t[5]=s[5];  t[6]=s[6];  t[7]=s[7];  t[8]=s[8];  
  t[9]=s[9];  t[10]=s[10];
  R1.load_status(t);
  t.resize(8);
  t[0]=EINV_t;
  t[1]=s[11];  t[2]=s[12];  t[3]=s[13];  t[4]=s[14]; 
  t[5]=s[15];  t[6]=s[16];  t[7]=s[17];
  R2.load_status(t);    
}
    
TRNG::RNG<TRNG::EINVLCG64> & TRNG::EINVLCG64::operator=(TRNG::RNG<TRNG::EINVLCG64> &other) {
  if (this!=&other) {
    std::vector<long> s;
    other.save_status(s);
    load_status(s);
  }
  return *this;
}

TRNG::EINVLCG64::EINVLCG64(TRNG::EINVLCG64_param param, long seed_) :
  R1(TRNG::LCG64_param(param.c, param.d)), 
  R2(TRNG::EINV_param(param.a, param.b, param.modulus))
{
  reset();
  seed(seed_);
}
