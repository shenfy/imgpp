#ifndef IMGPP_BCDESC_HPP
#define IMGPP_BCDESC_HPP

#include <cstdint>

namespace imgpp {

enum BCFormat: uint8_t {
  UNKNOWN_BC = 0,
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
  uint32_t block_width {0};
  uint32_t block_height {0};
  uint32_t block_bytes {0};
};

inline const BCDesc &GetBCDesc(BCFormat format) {
  static const BCDesc bc_descs[] = {
    { 1, 1, 0 }, // UNKNOWN_BC,
    { 4, 4, 16 }, // RGBA_ASTC_4x4,
    { 5, 4, 16 }, // RGBA_ASTC_5x4,
    { 5, 5, 16 }, // RGBA_ASTC_5x5,
    { 6, 5, 16 }, // RGBA_ASTC_6x5,
    { 6, 6, 16 }, // RGBA_ASTC_6x6,
    { 8, 5, 16 }, // RGBA_ASTC_8x5,
    { 8, 6, 16 }, // RGBA_ASTC_8x6,
    { 8, 8, 16 }, // RGBA_ASTC_8x8,
    { 10, 5, 16 }, // RGBA_ASTC_10x5,
    { 10, 6, 16 }, // RGBA_ASTC_10x6,
    { 10, 8, 16 }, // RGBA_ASTC_10x8,
    { 10, 10, 16 }, // RGBA_ASTC_10x10,
    { 12, 10, 16 }, // RGBA_ASTC_12x10,
    { 12, 12, 16 }, // RGBA_ASTC_12x12,
    { 4, 4, 16 }, // SRGB8_ALPHA8_ASTC_4x4,
    { 5, 4, 16 }, // SRGB8_ALPHA8_ASTC_5x4,
    { 5, 5, 16 }, // SRGB8_ALPHA8_ASTC_5x5,
    { 6, 5, 16 }, // SRGB8_ALPHA8_ASTC_6x5,
    { 6, 6, 16 }, // SRGB8_ALPHA8_ASTC_6x6,
    { 8, 5, 16 }, // SRGB8_ALPHA8_ASTC_8x5,
    { 8, 6, 16 }, // SRGB8_ALPHA8_ASTC_8x6,
    { 8, 8, 16 }, // SRGB8_ALPHA8_ASTC_8x8,
    { 10, 5, 16 }, // SRGB8_ALPHA8_ASTC_10x5,
    { 10, 6, 16 }, // SRGB8_ALPHA8_ASTC_10x6,
    { 10, 8, 16 }, // SRGB8_ALPHA8_ASTC_10x8,
    { 10, 10, 16 }, // SRGB8_ALPHA8_ASTC_10x10,
    { 12, 10, 16 }, // SRGB8_ALPHA8_ASTC_12x10,
    { 12, 12, 16 }, // SRGB8_ALPHA8_ASTC_12x12,
    { 4, 4, 16 }, // RGB_BPTC_SIGNED_FLOAT, BC6H signed float
    { 4, 4, 16 }, // RGB_BPTC_UNSIGNED_FLOAT_ARB, BC6H unsigned float
    { 4, 4, 16 }, // RGBA_BPTC_UNORM, BC7 rgba
    { 4, 4, 16 }  // SRGB_ALPHA_BPTC_UNORM_ARB, BC7 srgb_a
  };

  return bc_descs[(int)(format)];
}

}  // namespace

#endif // IMGPP_BCDESC_HPP
