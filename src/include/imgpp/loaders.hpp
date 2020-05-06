#ifndef IMGPP_LOADERS_HPP
#define IMGPP_LOADERS_HPP

#include <string>

/*! \file loaders.hpp */

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

  bool LoadBSON(const char *buffer, uint32_t length, Img &img);
  bool LoadBSON(const char *fn, Img &img);
  bool WriteBSON(const Img &img, std::string &bson);
  bool WriteBSON(const char *fn, const Img &img);
  bool WriteBSON(const ImgROI &roi, std::string &bson, bool flip_y = false);
  bool WriteBSON(const char *fn, const ImgROI &roi, bool flip_y = false);

}

#endif
