#ifndef IMGPP_SAMPLE_HPP
#define IMGPP_SAMPLE_HPP

#include <cmath>
#include "imgpp.hpp"

namespace imgpp {

template<typename T, typename TR, int dim>
bool Tex2DNN(const ImgROI &roi, float x, float y, TR &result) {
  if (x < 0 || x > roi.Width() || y < 0 || y >= roi.Height()) {
    return false;
  }
  uint32_t ui = (uint32_t)(x + 0.5f);
  uint32_t vi = (uint32_t)(y + 0.5f);

  for (int i = 0; i < dim; i++) {
    result[i] = roi.At<T>(ui, vi, (uint32_t)i);
  }
  return true;
}

template<typename T, typename TR, int dim>
bool Tex2DBilinear(const ImgROI &roi, float x, float y, TR &result) {
  if (x < 0 || x > roi.Width() || y < 0 || y >= roi.Height()) {
    return false;
  }
  uint32_t u0 = (uint32_t)(floor(x));
  uint32_t v0 = (uint32_t)(floor(y));
  uint32_t u1 = (uint32_t)(ceil(x));
  uint32_t v1 = (uint32_t)(ceil(y));
  float u = x - u0;
  float v = y = v0;

  for (int i = 0; i < dim; i++) {
    T val_u0 = roi.At<T>(u0, v0, (uint32_t)i);
    T val_u1 = roi.At<T>(u1, v0, (uint32_t)i);
    T val_u2 = roi.At<T>(u0, v1, (uint32_t)i);
    T val_u3 = roi.At<T>(u1, v1, (uint32_t)i);
    T val_uv0 = val_u0 * u + val_u1 * (1 - u);
    T val_uv1 = val_u2 * u + val_u3 * (1 - u);
    result[i] = val_uv0 * v + val_uv1 * (1 - v);
  }
  return true;
}

} //namespace imgpp

#endif //IMGPP_SAMPLE_HPP
