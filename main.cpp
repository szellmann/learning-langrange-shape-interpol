#include <iostream>

struct interval
{
  interval() = default;
  interval(float f) : lo(f), hi(f) {}
  interval(float l, float h) : lo(l), hi(h) {}
  float length() const { return hi-lo; }
  bool contains(float f) const { return lo <= f && f <= hi; }
  float lo, hi;
};

inline interval operator+(interval a, interval b) {
  return { a.lo+b.lo, a.hi+b.hi };
}

inline interval operator-(interval a, interval b) {
  return { a.lo-b.lo, a.hi-b.hi };
}

inline interval operator*(interval a, interval b) {
  float ac = a.lo*b.lo;
  float ad = a.lo*b.hi;
  float bc = a.hi*b.lo;
  float bd = a.hi*b.hi;
  return {
    fminf(ac,fminf(ad,fminf(bc,bd))),
    fmaxf(ac,fmaxf(ad,fmaxf(bc,bd)))
  };
}

inline interval operator/(interval a, interval b) {
  // special handling for "division by zero" (eqvl. 0 in b)
  if (b.lo <= 0.f && 0.f <= b.hi) {
    return { -INFINITY, INFINITY };
  }

  float ac = a.lo/b.lo;
  float ad = a.lo/b.hi;
  float bc = a.hi/b.lo;
  float bd = a.hi/b.hi;
  return {
    fminf(ac,fminf(ad,fminf(bc,bd))),
    fmaxf(ac,fmaxf(ad,fmaxf(bc,bd)))
  };
}

inline interval& operator+=(interval& a, const interval& b) {
  a = a + b;
  return a;
}

inline interval& operator-=(interval& a, const interval& b) {
  a = a - b;
  return a;
}

inline interval& operator*=(interval& a, const interval& b) {
  a = a * b;
  return a;
}

inline interval& operator/=(interval& a, const interval& b) {
  a = a / b;
  return a;
}

inline std::ostream& operator<<(std::ostream& out, interval ival) {
  out << '[' << ival.lo << ':' << ival.hi << ']';
  return out;
}

struct LagrangePolynomial
{
  static constexpr int D=4;
  float p[D];
  float f[D];

  template <typename T>
  T evalBasis(const T& x, int j) const {
    T res={1.f};
    T xj(p[j]);
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-p[i])/(xj-p[i]);
    }
    return res;
  }

  template <typename T>
  T eval(const T& x) const {
    T res={0.f};
    for (int i=0; i<D; ++i) {
      res += f[i]*evalBasis(x,i);
    }
    return res;
  }

  float evalBasisDeriv(float x, int j) const {
    float a=0.f;
    for (int i=0; i<D; ++i) {
      float b = 1.f;
      if (i != j) {
        for (int k=0; k<D; ++k) {
          if (k != i && k != j) {
            b *= x-p[k];
          }
        }
        a += b;
      }
    }

    float c=1.f;
    float xj = p[j];
    for (int i=0; i<D; ++i) {
      if (i != j) c *= xj-p[i];
    }
    return a/c;
  }

  float evalDeriv(float x) const {
    float res=0.f;
    for (int i=0; i<D; ++i) {
      res += f[i]*evalBasisDeriv(x,i);
    }
    return res;
  }
};

//https://www.researchgate.net/profile/Ali-Yazici-2/publication/221434767_2d_Polynomial_Interpolation_A_Symbolic_Approach_with_Mathematica/links/579f35e308ae80bf6ea792f9/2d-Polynomial-Interpolation-A-Symbolic-Approach-with-Mathematica.pdf
struct LagrangeQuad
{
  static constexpr int D=4;
  float px[D];
  float py[D];
  float f[D][D];

  template <typename T>
  T evalBasis(float *p, const T& x, int j) const {
    T res={1.f};
    T xj(p[j]);
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-p[i])/(xj-p[i]);
    }
    return res;
  }

  template <typename T>
  T eval(const T& x, const T& y) const {
    T res={0.f};
    for (int i=0; i<D; ++i) {
      T lx = evalBasis(px,x,i);
      for (int j=0; j<D; ++j) {
        T my = evalBasis(py,y,j);
        res += f[i][j]*lx*my;
      }
    }
    return res;
  }
};

int main() {
  LagrangePolynomial poly;
  poly.p[0] = -9;
  poly.p[1] = -4;
  poly.p[2] = -1;
  poly.p[3] =  7;

  poly.f[0] =  5;
  poly.f[1] =  2;
  poly.f[2] = -2;
  poly.f[3] =  9;

  interval stack[32];
  int ptr = 0;
  stack[ptr++] = interval(-9.f, 7.f);

  interval ival = stack[--ptr];
  int steps=0;
  while (poly.eval(ival).contains(0.f)) {
    std::cout << steps++ << '\n';

    interval v = poly.eval(ival);
    if (v.length() < 0.00001f) {
      break;
    }

    interval a(ival.lo, ival.lo+ival.length()*0.5f);
    interval b(ival.lo+ival.length()*0.5f, ival.hi);

    bool b1 = poly.eval(a).contains(0.f);
    bool b2 = poly.eval(b).contains(0.f);

    if (b1 && b2) {
      ival = a;
      stack[ptr++] = b;
    } else if (b1 && !b2) {
      ival = a;
    } else if (!b1 && b2) {
      ival = b;
    } else if (ptr > 0) {
      ival = stack[--ptr];
    } else {
      break;
    }
  }
  interval iv = poly.eval(ival);
  std::cout << ival << iv << '\n';

  float val = poly.eval(-2.57413f);
  std::cout << val << '\n';
}



