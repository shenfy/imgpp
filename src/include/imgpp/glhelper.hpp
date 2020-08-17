#ifndef IMGPP_GL_HPP
#define IMGPP_GL_HPP

#include <imgpp/texturedesc.hpp>

namespace imgpp { namespace gl {
enum Profile: uint8_t {
  ES20, // no sRGB
  ES30, // has swizzle
  GL32,
  GL33, // has swizzle
  KTX
};

struct GLFormatDesc {
  uint16_t internal_format;
  uint16_t external_format;
  uint16_t base_internal_format;
  uint16_t type;
};

TextureFormat TranslateFromGL(uint16_t internal, uint16_t external,
  uint16_t base_internal, uint16_t type);
GLFormatDesc TranslateToGL(TextureFormat format, Profile profile);
GLFormatDesc TranslateToGL(TextureFormat format);
uint32_t GetTypeSize(uint16_t type);
}}

#endif
