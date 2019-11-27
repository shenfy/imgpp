#ifndef IMGPP_LOADERS_HPP
#define IMGPP_LOADERS_HPP

#include <istream>

/*! \file imgpp_loaders_ext.hpp */

namespace imgpp {

  class Img;
  class ImgROI;

  bool LoadBMP(const char *fn, Img &img, bool flip_y = false);
  bool LoadBMP(const char *src, uint32_t length, Img &img, bool flip_y = false);
  bool WriteBMP(const char *fn, const ImgROI &roi, bool flip_y = false);

  bool LoadPFM(const char *fn, Img &img, bool flip_y = false);
  bool LoadPFM(const char *src, uint32_t length, Img &img, bool flip_y = false);
  bool WritePFM(const char *fn, const ImgROI &roi, bool flip_y = false);

  bool LoadPPM(const char *fn, Img &img, bool flip_y = false);
  bool LoadPPM(const char *src, uint32_t length, Img &img, bool flip_y = false);
  bool WritePPM(const char *fn, const ImgROI &roi, bool flip_y = false);

}

#endif
