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

  //! Although http://netpbm.sourceforge.net/doc/pgm.html and http://netpbm.sourceforge.net/doc/ppm.html
  //! says "The most significant byte is first.", our loader doesn't respect that and reads whatever
  //! bytes into memory as it is for performance concern.
  bool LoadPPM(const char *fn, Img &img, bool bottom_first = false);
  bool LoadPPM(const char *src, uint32_t length, Img &img, bool bottom_first = false);
  //! As the loader, our writer also ignores the endianness specification and writes whatever bytes
  //! in the memory to the file.
  //! That implies that a pgm/ppm image written by our writer can only be read by our reader
  //! on systems with the same endianness.
  bool WritePPM(const char *fn, const ImgROI &roi, bool bottom_first = false);

  bool LoadBSON(const char *buffer, uint32_t length, Img &img);
  bool LoadBSON(const char *fn, Img &img);
  bool WriteBSON(const Img &img, std::string &bson);
  bool WriteBSON(const char *fn, const Img &img);
  bool WriteBSON(const ImgROI &roi, std::string &bson, bool flip_y = false);
  bool WriteBSON(const char *fn, const ImgROI &roi, bool flip_y = false);

}

#endif
