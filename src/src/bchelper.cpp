#include <imgpp/bchelper.hpp>
#include <unordered_map>
#include <vector>

namespace imgpp {
BCFormat KHRToBCFormat(int internal_format) {
  static const std::unordered_map<int, BCFormat> khr_2_bc {
    { 0x93B0, RGBA_ASTC_4x4},
    { 0x93B1, RGBA_ASTC_5x4},
    { 0x93B2, RGBA_ASTC_5x5},
    { 0x93B3, RGBA_ASTC_6x5},
    { 0x93B4, RGBA_ASTC_6x6},
    { 0x93B5, RGBA_ASTC_8x5},
    { 0x93B6, RGBA_ASTC_8x6},
    { 0x93B7, RGBA_ASTC_8x8},
    { 0x93B8, RGBA_ASTC_10x5},
    { 0x93B9, RGBA_ASTC_10x6},
    { 0x93BA, RGBA_ASTC_10x8},
    { 0x93BB, RGBA_ASTC_10x10},
    { 0x93BC, RGBA_ASTC_12x10},
    { 0x93BD, RGBA_ASTC_12x12},
    { 0x93D0, SRGB8_ALPHA8_ASTC_4x4},
    { 0x93D1, SRGB8_ALPHA8_ASTC_5x4},
    { 0x93D2, SRGB8_ALPHA8_ASTC_5x5},
    { 0x93D3, SRGB8_ALPHA8_ASTC_6x5},
    { 0x93D4, SRGB8_ALPHA8_ASTC_6x6},
    { 0x93D5, SRGB8_ALPHA8_ASTC_8x5},
    { 0x93D6, SRGB8_ALPHA8_ASTC_8x6},
    { 0x93D7, SRGB8_ALPHA8_ASTC_8x8},
    { 0x93D8, SRGB8_ALPHA8_ASTC_10x5},
    { 0x93D9, SRGB8_ALPHA8_ASTC_10x6},
    { 0x93DA, SRGB8_ALPHA8_ASTC_10x8},
    { 0x93DB, SRGB8_ALPHA8_ASTC_10x10},
    { 0x93DC, SRGB8_ALPHA8_ASTC_12x10},
    { 0x93DD, SRGB8_ALPHA8_ASTC_12x12},
    { 0x8E8E, RGB_BPTC_SIGNED_FLOAT},
    { 0x8E8F, RGB_BPTC_UNSIGNED_FLOAT_ARB},
    { 0x8E8C, RGBA_BPTC_UNORM},
    { 0x8E8D, SRGB_ALPHA_BPTC_UNORM_ARB}
  };
  if (auto it = khr_2_bc.find(internal_format); it != khr_2_bc.end()) {
    return it->second;
  } else {
    return Unknown;
  }
}

int BCFormatToKHR(BCFormat format) {
  static const std::vector<int> bc_descs{
    0xffff,
    0x93B0,
    0x93B1,
    0x93B2,
    0x93B3,
    0x93B4,
    0x93B5,
    0x93B6,
    0x93B7,
    0x93B8,
    0x93B9,
    0x93BA,
    0x93BB,
    0x93BC,
    0x93BD,
    0x93D0,
    0x93D1,
    0x93D2,
    0x93D3,
    0x93D4,
    0x93D5,
    0x93D6,
    0x93D7,
    0x93D8,
    0x93D9,
    0x93DA,
    0x93DB,
    0x93DC,
    0x93DD,
    0x8E8E,
    0x8E8F,
    0x8E8C,
    0x8E8D
  };

  return bc_descs[(int)(format)];
}

}
