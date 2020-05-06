#ifndef IMGPP_LOADERSEXT_HPP
#define IMGPP_LOADERSEXT_HPP

/*! \file loadersext.hpp */

#include <memory>

namespace imgpp {

  class Img;
  class ImgROI;

  bool Load(const char *fn, Img &img, bool flip_y = false);
  bool Load(const char *src, uint32_t length, Img &img, bool flip_y = false);
  bool Write(const char *fn, const ImgROI &roi, bool flip_y = false);

  bool LoadPNG(const char *fn, Img &img, bool flip_y = false);
  bool LoadPNG(void *src, uint32_t length, Img &img, bool flip_y = false);
  bool WritePNG(const char *fn, const ImgROI &roi, bool flip_y = false);
  size_t CompressPNG(const ImgROI& roi, void *dst, size_t length);

  bool LoadJPEG(const char *fn, Img &img, bool flip_y = false);
  bool LoadJPEG(const void *src, uint32_t length, Img &img, bool flip_y = false);
  bool WriteJPEG(const char *fn, const ImgROI &roi, bool flip_y = false);
  uint32_t CompressJPEG(const ImgROI &roi, void *dst, uint32_t length);

}

#endif