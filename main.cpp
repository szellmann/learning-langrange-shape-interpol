#include <iostream>

struct interval1
{
  interval1() = default;
  interval1(float f) : lo(f), hi(f) {}
  interval1(float l, float h) : lo(l), hi(h) {}
  float length() const { return hi-lo; }
  bool contains(float f) const { return lo <= f && f <= hi; }
  float lo, hi;
};

inline interval1 operator+(interval1 a, interval1 b) {
  return { a.lo+b.lo, a.hi+b.hi };
}

inline interval1 operator-(interval1 a, interval1 b) {
  return { a.lo-b.lo, a.hi-b.hi };
}

inline interval1 operator*(interval1 a, interval1 b) {
  float ac = a.lo*b.lo;
  float ad = a.lo*b.hi;
  float bc = a.hi*b.lo;
  float bd = a.hi*b.hi;
  return {
    fminf(ac,fminf(ad,fminf(bc,bd))),
    fmaxf(ac,fmaxf(ad,fmaxf(bc,bd)))
  };
}

inline interval1 operator/(interval1 a, interval1 b) {
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

inline interval1& operator+=(interval1& a, const interval1& b) {
  a = a + b;
  return a;
}

inline interval1& operator-=(interval1& a, const interval1& b) {
  a = a - b;
  return a;
}

inline interval1& operator*=(interval1& a, const interval1& b) {
  a = a * b;
  return a;
}

inline interval1& operator/=(interval1& a, const interval1& b) {
  a = a / b;
  return a;
}

inline std::ostream& operator<<(std::ostream& out, interval1 ival) {
  out << '[' << ival.lo << ':' << ival.hi << ']';
  return out;
}

struct LagrangePolynomial
{
  static constexpr int D=4;
  float value[D];

  template <typename T>
  T evalBasis(const T& x, int j) const {
    T res={1.f};
    T xj((float)j);
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-i)/(xj-i);
    }
    return res;
  }

  template <typename T>
  T eval(const T& x) const {
    T res={0.f};
    for (int i=0; i<D; ++i) {
      res += value[i]*evalBasis(x,i);
    }
    return res;
  }
};

struct LagrangeQuad
{
  static constexpr int D=4;
  float value[D][D];

  template <typename T>
  T evalBasis(const T& x, int j) const {
    T res={1.f};
    T xj((float)j);
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-i)/(xj-i);
    }
    return res;
  }

  template <typename T>
  T eval(const T& x, const T& y) const {
    T res={0.f};
    for (int i=0; i<D; ++i) {
      T lx = evalBasis(x,i);
      for (int j=0; j<D; ++j) {
        T my = evalBasis(y,j);
        res += value[i][j]*lx*my;
      }
    }
    return res;
  }
};

int main() {
  LagrangePolynomial poly;
  poly.value[0] = -4;
  poly.value[1] =  4;
  poly.value[2] =  6;
  poly.value[3] =  8;

#if 1
  interval1 stack[32];
  int ptr = 0;
  stack[ptr++] = interval1(0.f, 3.f);

  interval1 ival = stack[--ptr];
  while (poly.eval(ival).contains(0.f)) {

    interval1 v = poly.eval(ival);
    if (v.length() < 0.00001f) {
      break;
    }

    interval1 a(ival.lo, ival.lo+ival.length()*0.5f);
    interval1 b(ival.lo+ival.length()*0.5f, ival.hi);

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
  interval1 iv = poly.eval(ival);
  std::cout << ival << iv << '\n';
#endif

  float val = poly.eval(0.36563f);
  std::cout << val << '\n';
}



