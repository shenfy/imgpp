#ifndef IMGPP_BCDESC_HPP
#define IMGPP_BCDESC_HPP

#include <vector>
#include <cstdint>

namespace imgpp {
enum BCFormat: uint8_t {
  UNKNOWN = 0,
  RGBA_ASTC_4x4,
  RGBA_ASTC_5x4,
  RGBA_ASTC_5x5,
  RGBA_ASTC_6x5,
  RGBA_ASTC_6x6,
  RGBA_ASTC_8x5,
  RGBA_ASTC_8x6,
  RGBA_ASTC_8x8,
  RGBA_ASTC_10x5,
  RGBA_ASTC_10x6,
  RGBA_ASTC_10x8,
  RGBA_ASTC_10x10,
  RGBA_ASTC_12x10,
  RGBA_ASTC_12x12,
  SRGB8_ALPHA8_ASTC_4x4,
  SRGB8_ALPHA8_ASTC_5x4,
  SRGB8_ALPHA8_ASTC_5x5,
  SRGB8_ALPHA8_ASTC_6x5,
  SRGB8_ALPHA8_ASTC_6x6,
  SRGB8_ALPHA8_ASTC_8x5,
  SRGB8_ALPHA8_ASTC_8x6,
  SRGB8_ALPHA8_ASTC_8x8,

  SRGB8_ALPHA8_ASTC_10x5,
  SRGB8_ALPHA8_ASTC_10x6,
  SRGB8_ALPHA8_ASTC_10x8,
  SRGB8_ALPHA8_ASTC_10x10,
  SRGB8_ALPHA8_ASTC_12x10,
  SRGB8_ALPHA8_ASTC_12x12,

  RGB_BPTC_SIGNED_FLOAT,
  RGB_BPTC_UNSIGNED_FLOAT_ARB,
  RGBA_BPTC_UNORM,
  SRGB_ALPHA_BPTC_UNORM_ARB
};

// Block based compression texture info
struct BCDesc {
  BCFormat format;
  uint32_t block_width;
  uint32_t block_height;
  uint32_t block_bytes;
};

inline const BCDesc &GetBCDesc(BCFormat format) {
  static const std::vector<BCDesc> bc_descs{
    { UNKNOWN, 1, 1, 0},
    { RGBA_ASTC_4x4, 4, 4, 16 },
    { RGBA_ASTC_5x4, 5, 4, 16 },
    { RGBA_ASTC_5x5, 5, 5, 16 },
    { RGBA_ASTC_6x5, 6, 5, 16 },
    { RGBA_ASTC_6x6, 6, 6, 16 },
    { RGBA_ASTC_8x5, 8, 5, 16 },
    { RGBA_ASTC_8x6, 8, 6, 16 },
    { RGBA_ASTC_8x8, 8, 8, 16 },
    { RGBA_ASTC_10x5, 10, 5, 16 },
    { RGBA_ASTC_10x6, 10, 6, 16 },
    { RGBA_ASTC_10x8, 10, 8, 16 },
    { RGBA_ASTC_10x10, 10, 10, 16 },
    { RGBA_ASTC_12x10, 12, 10, 16 },
    { RGBA_ASTC_12x12, 12, 12, 16 },
    { SRGB8_ALPHA8_ASTC_4x4, 4, 4, 16 },
    { SRGB8_ALPHA8_ASTC_5x4, 5, 4, 16 },
    { SRGB8_ALPHA8_ASTC_5x5, 5, 5, 16 },
    { SRGB8_ALPHA8_ASTC_6x5, 6, 5, 16 },
    { SRGB8_ALPHA8_ASTC_6x6, 6, 6, 16 },
    { SRGB8_ALPHA8_ASTC_8x5, 8, 5, 16 },
    { SRGB8_ALPHA8_ASTC_8x6, 8, 6, 16 },
    { SRGB8_ALPHA8_ASTC_8x8, 8, 8, 16 },
    { SRGB8_ALPHA8_ASTC_10x5, 10, 5, 16 },
    { SRGB8_ALPHA8_ASTC_10x6, 10, 6, 16 },
    { SRGB8_ALPHA8_ASTC_10x8, 10, 8, 16 },
    { SRGB8_ALPHA8_ASTC_10x10, 10, 10, 16 },
    { SRGB8_ALPHA8_ASTC_12x10, 12, 10, 16 },
    { SRGB8_ALPHA8_ASTC_12x12, 12, 12, 16 },
    { RGB_BPTC_SIGNED_FLOAT, 4, 4, 16 }, // BC6H signed float
    { RGB_BPTC_UNSIGNED_FLOAT_ARB, 4, 4, 16 }, // BC6H unsigned float
    { RGBA_BPTC_UNORM, 4, 4, 16 }, // BC7 rgba
    { SRGB_ALPHA_BPTC_UNORM_ARB, 4, 4, 16 } // BC7 srgb_a
  };

  return bc_descs[(int)(format)];
}
}
#endif // IMGPP_BCDESC_HPP
