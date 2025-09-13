#include <iostream>

#include "data.h"
#include "math.h"

using float3 = math::vec3f;
using float2 = math::vec2f;
using interval1 = math::interval1f;
using interval3 = math::interval3f;

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
      int splitAxis = 0;
      if (ival.y.length() > ival.x.length())
        splitAxis = 1;
      if (ival.z.length() > ival.x.length() && ival.z.length() > ival.y.length())
        splitAxis = 2;

      constexpr int N=2; // num children to split into
      interval3 c[N];
      for (int a=0; a<3; ++a) {
        if (a == splitAxis) {
          for (int i=0; i<N; ++i) {
            c[i][a] = interval1(ival[a].lo+ival[a].length()*(i/float(N)),
                                ival[a].lo+ival[a].length()*((i+1)/float(N)));
          }
          // set outer bounds exactly to avoid floating point error:
          c[0][a].lo = ival[a].lo;
          c[N-1][a].hi = ival[a].hi;
        } else {
          for (int i=0; i<N; ++i) {
            c[i][a] = ival[a];
          }
        }
      }

      bool b[N];
      for (int i=0; i<N; ++i) {
        b[i] = hex.eval(c[i].x,c[i].y,c[i].z).contains(P);
      }

      bool assigned=false;
      for (int i=0; i<N; ++i) {
        if (!b[i]) continue;
        if (assigned) {
          stack[ptr++] = c[i];
        } else {
          ival = c[i];
          assigned = true;
        }
      }

      if (!assigned) {
        goto next;
      }
    }

    auto leafBounds = hex.eval(ival.x,ival.y,ival.z);
    if (leafBounds.volume() <= threshold) {
      if (leafBounds.contains(P))
        return true;
    }
  }

  return false;
}

// https://www.uni-ulm.de/fileadmin/website_uni_ulm/iui.inst.100/institut/Papers/PrecisionRayTracing.pdf
template <typename V, int Deg>
struct BezierCurve
{
  enum { D=Deg, };
  V cp[D];

  void subdivide(BezierCurve &a, BezierCurve &b) const {
    V dst[D*2];
    V tmp[D*2];

    memcpy(dst,cp,sizeof(cp));

    int n=D;
    for (int n=D; n<D*2; ++n) {
      memcpy(tmp,dst,sizeof(dst));
      int ptr=0;
      dst[ptr++] = tmp[0];
      for (int j=0; j<n-1; ++j) {
        V v1 = tmp[j];
        V v2 = tmp[j+1];
        dst[ptr++] = v1*0.5f+v2*0.5f;
      }
      dst[ptr++] = tmp[n-1];
    }

    memcpy(a.cp,&dst[0],sizeof(a.cp));
    memcpy(b.cp,&dst[D],sizeof(b.cp));
  }
};

template <typename V, int Deg>
struct BezierQuad
{
  enum { D=Deg, };
  BezierCurve cv[D];

  // TODO:
  void subdivide(BezierQuad &a, BezierQuad &b) const {
    // dim-1
    for (int i=0; i<D; ++i) {
      BezierCurve ca,cb;
      cv[i].subdivide(ca,cb);
      a.cv[i] = ca;
      b.cv[i] = cb;
    }
    // dim-2
    for (int i=0; i<D; ++i) {
      BezierCurve c;
      for (int j=0; j<D; ++j) {
        c[j] = a.cv[j].cp[i];
      }
    }
  }
};

template <typename V>
struct BezierHex
{
  static constexpr int D=4;
  V cp[D][D][D];

  V eval(float x, float y, float z) const {
  }
};

int main() {
  LagrangeHex<float3> hex;
  int order=hex.D;
  auto data = test01(order);
  memcpy(hex.value,data.data(),sizeof(data[0])*data.size());

  std::cout << sample(hex,float3(1.f,1.f,1.f)) << '\n';

  BezierCurve<float2,4> bc;
  bc.cp[0] = float2(0.0f,0.0f);
  bc.cp[1] = float2(1.0f,1.0f);
  bc.cp[2] = float2(2.0f,1.0f);
  bc.cp[3] = float2(3.0f,0.0f);

  BezierCurve<float2,4> a, b;
  bc.subdivide(a,b);

  for (int i=0; i<a.D; ++i) {
    std::cout << a.cp[i] << ' ';
  }
  std::cout << '\n';

  for (int i=0; i<b.D; ++i) {
    std::cout << b.cp[i] << ' ';
  }
  std::cout << '\n';
}



