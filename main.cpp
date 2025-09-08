#include <iostream>

struct float3 {
  float x, y, z;
};

struct LagrangePolynomial
{
  static constexpr int D=4;
  float p[D];
  float f[D];

  float evalBasis(float x, int j) const {
    float res=1.f;
    float xj = p[j];
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-p[i])/(xj-p[i]);
    }
    return res;
  }

  float eval(float x) const {
    float res=0.f;
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

struct LagrangeHex
{
  /*static constexpr int D=4;
  float3 p[D];
  float f[D];

  float evalBasis(float x, int j) const {
    float res=1.f;
    float xj = p[j];
    for (int i=0; i<D; ++i) {
      if (i != j) res *= (x-p[i])/(xj-p[i]);
    }
    return res;
  }

  float eval(float x) const {
    float res=0.f;
    for (int i=0; i<D; ++i) {
      res += f[i]*evalBasis(x,i);
    }
    return res;
  }*/
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

  float val = poly.eval(5.f);
  std::cout << val << '\n';

  float der = poly.evalDeriv(1.5f);
  std::cout << der << '\n';
}
