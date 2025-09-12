#pragma once

// std
#include <vector>
// ours
#include "math.h"

static std::vector<float3> test01(int order) {
  int n = order;
  std::vector<float3> result;
  for (int z=0; z<n; ++z) {
    for (int y=0; y<n; ++y) {
      for (int x=0; x<n; ++x) {
        float3 offset(0.f);
        //float3 scale(1.f/(order-1));
        float3 scale(1.f);
        result.push_back(offset+float3(x,y,z)*scale);
      }
    }
  }
  return result;
}

static void testVTU() {
  std::vector<float3> pts;
  pts.push_back(float3(0,0,0));
  pts.push_back(float3(1,0,0));
  pts.push_back(float3(0,1,0));
  pts.push_back(float3(1,1,0));
  pts.push_back(float3(0,0,1));
  pts.push_back(float3(1,0,1));
  pts.push_back(float3(0,1,1));
  pts.push_back(float3(1,1,1));
  pts.push_back(float3(0.333333,0,0));
  pts.push_back(float3(0.666667,0,0));
  pts.push_back(float3(0,0.333333,0));
  pts.push_back(float3(0.333333,0.333333,0));
  pts.push_back(float3(0.666667,0.333333,0));
  pts.push_back(float3(1,0.333333,0));
  pts.push_back(float3(0,0.666667,0));
  pts.push_back(float3(0.333333,0.666667,0));
  pts.push_back(float3(0.666667,0.666667,0));
  pts.push_back(float3(1,0.666667,0));
  pts.push_back(float3(0.333333,1,0));
  pts.push_back(float3(0.666667,1,0));
  pts.push_back(float3(0,0,0.333333));
  pts.push_back(float3(0.333333,0,0.333333));
  pts.push_back(float3(0.666667,0,0.333333));
  pts.push_back(float3(1,0,0.333333));
  pts.push_back(float3(0,0.333333,0.333333));
  pts.push_back(float3(0.333333,0.333333,0.333333));
  pts.push_back(float3(0.666667,0.333333,0.333333));
  pts.push_back(float3(1,0.333333,0.333333));
  pts.push_back(float3(0,0.666667,0.333333));
  pts.push_back(float3(0.333333,0.666667,0.333333));
  pts.push_back(float3(0.666667,0.666667,0.333333));
  pts.push_back(float3(1,0.666667,0.333333));
  pts.push_back(float3(0,1,0.333333));
  pts.push_back(float3(0.333333,1,0.333333));
  pts.push_back(float3(0.666667,1,0.333333));
  pts.push_back(float3(1,1,0.333333));
  pts.push_back(float3(0,0,0.666667));
  pts.push_back(float3(0.333333,0,0.666667));
  pts.push_back(float3(0.666667,0,0.666667));
  pts.push_back(float3(1,0,0.666667));
  pts.push_back(float3(0,0.333333,0.666667));
  pts.push_back(float3(0.333333,0.333333,0.666667));
  pts.push_back(float3(0.666667,0.333333,0.666667));
  pts.push_back(float3(1,0.333333,0.666667));
  pts.push_back(float3(0,0.666667,0.666667));
  pts.push_back(float3(0.333333,0.666667,0.666667));
  pts.push_back(float3(0.666667,0.666667,0.666667));
  pts.push_back(float3(1,0.666667,0.666667));
  pts.push_back(float3(0,1,0.666667));
  pts.push_back(float3(0.333333,1,0.666667));
  pts.push_back(float3(0.666667,1,0.666667));
  pts.push_back(float3(1,1,0.666667));
  pts.push_back(float3(0.333333,0,1));
  pts.push_back(float3(0.666667,0,1));
  pts.push_back(float3(0,0.333333,1));
  pts.push_back(float3(0.333333,0.333333,1));
  pts.push_back(float3(0.666667,0.333333,1));
  pts.push_back(float3(1,0.333333,1));
  pts.push_back(float3(0,0.666667,1));
  pts.push_back(float3(0.333333,0.666667,1));
  pts.push_back(float3(0.666667,0.666667,1));
  pts.push_back(float3(1,0.666667,1));
  pts.push_back(float3(0.333333,1,1));
  pts.push_back(float3(0.666667,1,1));
}
