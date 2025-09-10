#include <iostream>

struct float3
{
  float3() = default;
  float3(float f) : x(f), y(f), z(f) {}
  float3(float x, float y, float z) : x(x), y(y), z(z) {}
  float x, y, z;
};

inline float3 operator+(const float3& a, const float3 &b) {
  return { a.x+b.x, a.y+b.y, a.z+b.z };
}

inline float3 operator-(const float3& a, const float3 &b) {
  return { a.x-b.x, a.y-b.y, a.z-b.z };
}

inline float3 operator*(const float3& a, const float3 &b) {
  return { a.x*b.x, a.y*b.y, a.z*b.z };
}

inline float3 operator/(const float3& a, const float3 &b) {
  return { a.x/b.x, a.y/b.y, a.z/b.z };
}

inline float3& operator+=(float3& a, const float3& b) {
  a = a + b;
  return a;
}

inline float3& operator-=(float3& a, const float3& b) {
  a = a - b;
  return a;
}

inline float3& operator*=(float3& a, const float3& b) {
  a = a * b;
  return a;
}

inline float3& operator/=(float3& a, const float3& b) {
  a = a / b;
  return a;
}

inline std::ostream& operator<<(std::ostream& out, float3 v) {
  out << '(' << v.x << ',' << v.y << ',' << v.z << ')';
  return out;
}

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

struct interval3
{
  interval3() = default;
  interval3(float3 f) : lo(f), hi(f) {}
  interval3(float3 l, float3 h) : lo(l), hi(h) {}
  interval3(interval1 i) : lo(i.lo,i.lo,i.lo), hi(i.hi,i.hi,i.hi) {}
  interval3(interval1 i1, interval1 i2, interval1 i3)
    : lo(i1.lo,i2.lo,i3.lo),
      hi(i1.hi,i2.hi,i3.hi)
  {}
  //bool contains(float f) const { return lo <= f && f <= hi; }
  float3 lo, hi;
};

inline interval3 operator+(const interval3& a, const interval3& b) {
  return {
    interval1(a.lo.x,a.hi.x)+interval1(b.lo.x,b.hi.x),
    interval1(a.lo.y,a.hi.y)+interval1(b.lo.y,b.hi.y),
    interval1(a.lo.z,a.hi.z)+interval1(b.lo.z,b.hi.z)
  };
}

inline interval3 operator*(const interval3& a, const interval3& b) {
  return {
    interval1(a.lo.x,a.hi.x)*interval1(b.lo.x,b.hi.x),
    interval1(a.lo.y,a.hi.y)*interval1(b.lo.y,b.hi.y),
    interval1(a.lo.z,a.hi.z)*interval1(b.lo.z,b.hi.z)
  };
}

inline interval3& operator+=(interval3& a, const interval3& b) {
  a = a + b;
  return a;
}

// inline interval3& operator-=(interval3& a, const interval3& b) {
//   a = a - b;
//   return a;
// }

inline interval3& operator*=(interval3& a, const interval3& b) {
  a = a * b;
  return a;
}

// inline interval3& operator/=(interval3& a, const interval3& b) {
//   a = a / b;
//   return a;
// }

inline std::ostream& operator<<(std::ostream& out, interval3 ival) {
  out << '[' << ival.lo << ':' << ival.hi << ']';
  return out;
}

// promoting operations

inline interval3 operator*(const float3& a, const interval1& b) {
  return { interval1(a.x)*b, interval1(a.y)*b, interval1(a.z)*b };
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

template <typename V>
struct LagrangeQuad
{
  static constexpr int D=4;
  V value[D][D];

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
  auto eval(const T& x, const T& y) const {
    using R = decltype(value[0][0]*x); // TODO: make interval template over dim!
    R res=T{0.f};
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
#if 0
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
#endif

  LagrangeQuad<float3> quad;
  quad.value[0][0] = float3(-2.f, 0.f, -2.f);
  quad.value[1][0] = float3(-1.f, 0.f, -2.f);
  quad.value[2][0] = float3( 0.f, 0.f, -2.f);
  quad.value[3][0] = float3( 1.f, 0.f, -2.f);

  quad.value[0][1] = float3(-2.f, 0.f, -1.f);
  quad.value[1][1] = float3(-1.f, 0.f, -1.f);
  quad.value[2][1] = float3( 0.f, 0.f, -1.f);
  quad.value[3][1] = float3( 1.f, 0.f, -1.f);

  quad.value[0][2] = float3(-2.f, 0.f,  0.f);
  quad.value[1][2] = float3(-1.f, 0.f,  0.f);
  quad.value[2][2] = float3( 0.f, 0.f,  0.f);
  quad.value[3][2] = float3( 1.f, 0.f,  0.f);

  quad.value[0][3] = float3(-2.f, 0.f,  1.f);
  quad.value[1][3] = float3(-1.f, 0.f,  1.f);
  quad.value[2][3] = float3( 0.f, 0.f,  1.f);
  quad.value[3][3] = float3( 1.f, 0.f,  1.f);

  std::cout << quad.eval(interval1(0.0f,0.0001f), interval1(0.0f,0.0001f)) << '\n';
}



