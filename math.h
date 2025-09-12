#pragma once

#include <iostream>

struct float3
{
  float3() = default;
  float3(float f) : x(f), y(f), z(f) {}
  float3(float x, float y, float z) : x(x), y(y), z(z) {}
  const float &operator[](int i) const { return ((const float *)this)[i]; }
  float &operator[](int i) { return ((float *)this)[i]; }
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

inline float3 operator*(const float3& a, float b) {
  return { a.x*b, a.y*b, a.z*b };
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
  interval3(interval1 i) : x(i), y(i), z(i) {}
  interval3(interval1 x, interval1 y, interval1 z) : x(x), y(y), z(z) {}
  interval3(float3 f) : x(f.x), y(f.y), z(f.z) {}
  interval3(float3 lo, float3 hi)
    : x(lo.x,hi.x), y(lo.y,hi.y), z(lo.z,hi.z)
  {}
  float volume() const { return x.length()*y.length()*z.length(); }
  bool contains(float3 f) const {
    return x.contains(f.x) && y.contains(f.y) && z.contains(f.z);
  }
  const interval1 &operator[](int i) const {
    return i==0 ? x : i==1 ? y : z;
  }
  interval1 &operator[](int i) {
    return i==0 ? x : i==1 ? y : z;
  }
  interval1 x, y, z;
};

inline interval3 operator+(const interval3& a, const interval3& b) {
  return { a.x+b.x, a.y+b.y, a.z+b.z };
}

inline interval3 operator*(const interval3& a, const interval3& b) {
  return { a.x*b.x, a.y*b.y, a.z*b.z };
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
  out << '[' << float3(ival.x.lo,ival.y.lo,ival.z.lo) << ':'
    << float3(ival.x.hi,ival.y.hi,ival.z.hi) << ']';
  return out;
}

// promoting operations

inline interval3 operator*(const float3& a, const interval1& b) {
  return { interval1(a.x)*b, interval1(a.y)*b, interval1(a.z)*b };
}





