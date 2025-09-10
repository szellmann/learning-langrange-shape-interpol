#include <iostream>

#include "data.h"
#include "math.h"

template <typename V>
struct LagrangeHex
{
  static constexpr int D=4;
  V value[D][D][D];

  template <typename T>
  T evalBasis(const T& x, int j) const {
    T res={1.f};
    T xj(j);
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-i)/(xj-i);
    }
    return res;
  }

  template <typename T>
  auto eval(const T& x, const T& y, const T& z) const {
    using R = decltype(value[0][0][0]*x); // TODO: make interval template over dim!
    R res=T{0.f};
    for (int i=0; i<D; ++i) {
      T lx = evalBasis(x,i);
      for (int j=0; j<D; ++j) {
        T my = evalBasis(y,j);
        for (int k=0; k<D; ++k) {
          T nz = evalBasis(z,k);
          res += value[i][j][k]*lx*my*nz;
        }
      }
    }
    return res;
  }
};

inline
bool sample(const LagrangeHex<float3> &hex, float3 P) {
  interval3 stack[32];
  int ptr = 0;
  stack[ptr++] = interval3(float3(0.f),float3(hex.D-1));

  interval3 ival;

  bool contained = false;

  const float threshold{1e-4f};

next:
  while (ptr > 0) {
    ival = stack[--ptr];

    while  (hex.eval(ival.x,ival.y,ival.z).volume() > threshold) {
      interval3 c0, c1;
      int splitAxis = 0;
      if (ival.y.length() > ival.x.length())
        splitAxis = 1;
      if (ival.z.length() > ival.x.length() && ival.z.length() > ival.y.length())
        splitAxis = 2;

      for (int a=0; a<3; ++a) {
        if (a == splitAxis) {
          c0[a] = interval1(ival[a].lo,ival[a].lo+ival[a].length()*0.5f);
          c1[a] = interval1(ival[a].lo+ival[a].length()*0.5f,ival[a].hi);
        } else {
          c0[a] = ival[a];
          c1[a] = ival[a];
        }
      }

      bool b0 = hex.eval(c0.x,c0.y,c0.z).contains(P);
      bool b1 = hex.eval(c1.x,c1.y,c1.z).contains(P);

      if (b0 && b1) {
        stack[ptr++] = c0;
        ival = c1;
      } else if (b0) {
        ival = c0;
      } else if (b1) {
        ival = c1;
      } else {
        goto next;
      }
    }

    auto leafBounds = hex.eval(ival.x,ival.y,ival.z);
    if (leafBounds.volume() <= threshold) {
      std::cout << leafBounds << '\n';
      if (leafBounds.contains(P))
        return true;
    }
  }

  return false;
}

int main() {
  LagrangeHex<float3> hex;
  int order=hex.D;
  auto data = test01(order);
  memcpy(hex.value,data.data(),sizeof(data[0])*data.size());

  std::cout << sample(hex,float3(3.1f,1.f,1.f)) << '\n';
}



