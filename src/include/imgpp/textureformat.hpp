#ifndef IMGPP_TEXTUREFORMAT_HPP
#define IMGPP_TEXTUREFORMAT_HPP

#include <cstdint>
namespace imgpp {
enum TextureTarget: uint8_t {
  TARGET_1D = 0, TARGET_FIRST = TARGET_1D,
  TARGET_1D_ARRAY,
  TARGET_2D,
  TARGET_2D_ARRAY,
  TARGET_3D,
  TARGET_RECT,
  TARGET_RECT_ARRAY,
  TARGET_CUBE,
  TARGET_CUBE_ARRAY, TARGET_LAST = TARGET_CUBE_ARRAY
};

inline bool IsTarget1d(TextureTarget target)
{
  return target == TARGET_1D || target == TARGET_1D_ARRAY;
}

inline bool IsTarget3d(TextureTarget target)
{
  return target == TARGET_3D;
}

inline bool IsTargetArray(TextureTarget target)
{
  return target == TARGET_1D_ARRAY || target == TARGET_2D_ARRAY || target == TARGET_CUBE_ARRAY;
}

inline bool IsTargetCube(TextureTarget target)
{
  return target == TARGET_CUBE || target == TARGET_CUBE_ARRAY;
}

inline bool IsTargetRect(TextureTarget target)
{
  return target == TARGET_RECT || target == TARGET_RECT_ARRAY;
}

enum TextureFormat: uint16_t {
  FORMAT_UNDEFINED = 0,

  FORMAT_RG4_UNORM_PACK8, FORMAT_FIRST = FORMAT_RG4_UNORM_PACK8,
  FORMAT_RGBA4_UNORM_PACK16,
  FORMAT_R5G6B5_UNORM_PACK16,
  FORMAT_RGB5A1_UNORM_PACK16,

  FORMAT_R8_UNORM_PACK8,
  FORMAT_R8_SNORM_PACK8,
  FORMAT_R8_UINT_PACK8,
  FORMAT_R8_SINT_PACK8,
  FORMAT_R8_SRGB_PACK8,

  FORMAT_RG8_UNORM_PACK8,
  FORMAT_RG8_SNORM_PACK8,
  FORMAT_RG8_UINT_PACK8,
  FORMAT_RG8_SINT_PACK8,
  FORMAT_RG8_SRGB_PACK8,

  FORMAT_RGB8_UNORM_PACK8,
  FORMAT_RGB8_SNORM_PACK8,
  FORMAT_RGB8_UINT_PACK8,
  FORMAT_RGB8_SINT_PACK8,
  FORMAT_RGB8_SRGB_PACK8,

  FORMAT_RGBA8_UNORM_PACK8,
  FORMAT_RGBA8_SNORM_PACK8,
  FORMAT_RGBA8_UINT_PACK8,
  FORMAT_RGBA8_SINT_PACK8,
  FORMAT_RGBA8_SRGB_PACK8,

  FORMAT_RGB10A2_UNORM_PACK32,
  FORMAT_RGB10A2_SNORM_PACK32,
  FORMAT_RGB10A2_UINT_PACK32,
  FORMAT_RGB10A2_SINT_PACK32,

  FORMAT_R16_UNORM_PACK16,
  FORMAT_R16_SNORM_PACK16,
  FORMAT_R16_UINT_PACK16,
  FORMAT_R16_SINT_PACK16,
  FORMAT_R16_SFLOAT_PACK16,

  FORMAT_RG16_UNORM_PACK16,
  FORMAT_RG16_SNORM_PACK16,
  FORMAT_RG16_UINT_PACK16,
  FORMAT_RG16_SINT_PACK16,
  FORMAT_RG16_SFLOAT_PACK16,

  FORMAT_RGB16_UNORM_PACK16,
  FORMAT_RGB16_SNORM_PACK16,
  FORMAT_RGB16_UINT_PACK16,
  FORMAT_RGB16_SINT_PACK16,
  FORMAT_RGB16_SFLOAT_PACK16,

  FORMAT_RGBA16_UNORM_PACK16,
  FORMAT_RGBA16_SNORM_PACK16,
  FORMAT_RGBA16_UINT_PACK16,
  FORMAT_RGBA16_SINT_PACK16,
  FORMAT_RGBA16_SFLOAT_PACK16,

  FORMAT_R32_UINT_PACK32,
  FORMAT_R32_SINT_PACK32,
  FORMAT_R32_SFLOAT_PACK32,

  FORMAT_RG32_UINT_PACK32,
  FORMAT_RG32_SINT_PACK32,
  FORMAT_RG32_SFLOAT_PACK32,

  FORMAT_RGB32_UINT_PACK32,
  FORMAT_RGB32_SINT_PACK32,
  FORMAT_RGB32_SFLOAT_PACK32,

  FORMAT_RGBA32_UINT_PACK32,
  FORMAT_RGBA32_SINT_PACK32,
  FORMAT_RGBA32_SFLOAT_PACK32,

  FORMAT_R64_UINT_PACK64,
  FORMAT_R64_SINT_PACK64,
  FORMAT_R64_SFLOAT_PACK64,

  FORMAT_RG64_UINT_PACK64,
  FORMAT_RG64_SINT_PACK64,
  FORMAT_RG64_SFLOAT_PACK64,

  FORMAT_RGB64_UINT_PACK64,
  FORMAT_RGB64_SINT_PACK64,
  FORMAT_RGB64_SFLOAT_PACK64,

  FORMAT_RGBA64_UINT_PACK64,
  FORMAT_RGBA64_SINT_PACK64,
  FORMAT_RGBA64_SFLOAT_PACK64,

  FORMAT_RGB_DXT1_UNORM_BLOCK8,
  FORMAT_RGB_DXT1_SRGB_BLOCK8,
  FORMAT_RGBA_DXT1_UNORM_BLOCK8,
  FORMAT_RGBA_DXT1_SRGB_BLOCK8,
  FORMAT_RGBA_DXT3_UNORM_BLOCK16,
  FORMAT_RGBA_DXT3_SRGB_BLOCK16,
  FORMAT_RGBA_DXT5_UNORM_BLOCK16,
  FORMAT_RGBA_DXT5_SRGB_BLOCK16,
  FORMAT_R_ATI1N_UNORM_BLOCK8,
  FORMAT_R_ATI1N_SNORM_BLOCK8,
  FORMAT_RG_ATI2N_UNORM_BLOCK16,
  FORMAT_RG_ATI2N_SNORM_BLOCK16,
  FORMAT_RGB_BP_UFLOAT_BLOCK16,
  FORMAT_RGB_BP_SFLOAT_BLOCK16,
  FORMAT_RGBA_BP_UNORM_BLOCK16,
  FORMAT_RGBA_BP_SRGB_BLOCK16,

  FORMAT_RGB_ETC2_UNORM_BLOCK8, FORMAT_BLOCK_COMPRESSION_START = FORMAT_RGB_ETC2_UNORM_BLOCK8,
  FORMAT_RGB_ETC2_SRGB_BLOCK8,
  FORMAT_RGBA_ETC2_UNORM_BLOCK8,
  FORMAT_RGBA_ETC2_SRGB_BLOCK8,
  FORMAT_RGBA_ETC2_UNORM_BLOCK16,
  FORMAT_RGBA_ETC2_SRGB_BLOCK16,
  FORMAT_R_EAC_UNORM_BLOCK8,
  FORMAT_R_EAC_SNORM_BLOCK8,
  FORMAT_RG_EAC_UNORM_BLOCK16,
  FORMAT_RG_EAC_SNORM_BLOCK16,

  FORMAT_RGBA_ASTC_4X4_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_4X4_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_5X4_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_5X4_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_5X5_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_5X5_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_6X5_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_6X5_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_6X6_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_6X6_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_8X5_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_8X5_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_8X6_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_8X6_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_8X8_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_10X5_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_10X5_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_10X6_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_10X6_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_10X8_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_10X8_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_10X10_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_10X10_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_12X10_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_12X10_SRGB_BLOCK16,
  FORMAT_RGBA_ASTC_12X12_UNORM_BLOCK16,
  FORMAT_RGBA_ASTC_12X12_SRGB_BLOCK16,

  FORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32,
  FORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32,
  FORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32,
  FORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32,
  FORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32,
  FORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32,
  FORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32,
  FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32,
  FORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8,
  FORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8,
  FORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8,
  FORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8,

  FORMAT_RGB_ETC_UNORM_BLOCK8,
  FORMAT_RGB_ATC_UNORM_BLOCK8,
  FORMAT_RGBA_ATCA_UNORM_BLOCK16,
  FORMAT_RGBA_ATCI_UNORM_BLOCK16, FORMAT_BLOCK_COMPRESSION_LAST = FORMAT_RGBA_ATCI_UNORM_BLOCK16
};

struct PixelDesc {
  uint32_t components;
  // R, G, B, A
  uint32_t bpcs[4];
  bool is_signed;
  bool is_float;
};

// Make sure IsCompressed(format) == false
inline const PixelDesc &GetPixelDesc(TextureFormat format) {
  static const PixelDesc descs[] {
    { 2, {4, 4, 0, 0}, false, false },
    { 3, {4, 4, 4, 4}, false, false },
    { 4, {5, 6, 5, 0}, false, false },
    { 4, {5, 5, 5, 1}, false, false },

    { 1, {8, 0, 0, 0}, false, false },
    { 1, {8, 0, 0, 0}, true, false },
    { 1, {8, 0, 0, 0}, false, false },
    { 1, {8, 0, 0, 0}, true, false },
    { 1, {8, 0, 0, 0}, false, false },

    { 2, {8, 8, 0, 0}, false, false },
    { 2, {8, 8, 0, 0}, true, false },
    { 2, {8, 8, 0, 0}, false, false },
    { 2, {8, 8, 0, 0}, true, false },
    { 2, {8, 8, 0, 0}, false, false },

    { 3, {8, 8, 8, 0}, false, false },
    { 3, {8, 8, 8, 0}, true, false },
    { 3, {8, 8, 8, 0}, false, false },
    { 3, {8, 8, 8, 0}, true, false },
    { 3, {8, 8, 8, 0}, false, false },

    { 4, {8, 8, 8, 8}, false, false },
    { 4, {8, 8, 8, 8}, true, false },
    { 4, {8, 8, 8, 8}, false, false },
    { 4, {8, 8, 8, 8}, true, false },
    { 4, {8, 8, 8, 8}, false, false },

    { 4, {10, 10, 10, 2}, false, false },
    { 4, {10, 10, 10, 2}, true, false },
    { 4, {10, 10, 10, 2}, false, false },
    { 4, {10, 10, 10, 2}, true, false },

    { 1, {16, 0, 0, 0}, false, false },
    { 1, {16, 0, 0, 0}, true, false },
    { 1, {16, 0, 0, 0}, false, false },
    { 1, {16, 0, 0, 0}, true, false },
    { 1, {16, 0, 0, 0}, true, true },

    { 2, {16, 16, 0, 0}, false, false },
    { 2, {16, 16, 0, 0}, true, false },
    { 2, {16, 16, 0, 0}, false, false },
    { 2, {16, 16, 0, 0}, true, false },
    { 2, {16, 16, 0, 0}, true, true },

    { 3, {16, 16, 16, 0}, false, false },
    { 3, {16, 16, 16, 0}, true, false },
    { 3, {16, 16, 16, 0}, false, false },
    { 3, {16, 16, 16, 0}, true, false },
    { 3, {16, 16, 16, 0}, true, true },

    { 4, {16, 16, 16, 16}, false, false },
    { 4, {16, 16, 16, 16}, true, false },
    { 4, {16, 16, 16, 16}, false, false },
    { 4, {16, 16, 16, 16}, true, false },
    { 4, {16, 16, 16, 16}, true, true },

    { 1, {32, 0, 0, 0}, false, false },
    { 1, {32, 0, 0, 0}, true, false },
    { 1, {32, 0, 0, 0}, true, true },

    { 2, {32, 32, 0, 0}, false, false },
    { 2, {32, 32, 0, 0}, true, false },
    { 2, {32, 32, 0, 0}, true, true },

    { 3, {32, 32, 32, 0}, false, false },
    { 3, {32, 32, 32, 0}, true, false },
    { 3, {32, 32, 32, 0}, true, true },

    { 4, {32, 32, 32, 32}, false, false },
    { 4, {32, 32, 32, 32}, true, false },
    { 4, {32, 32, 32, 32}, true, true },

    { 1, {64, 0, 0, 0}, false, false },
    { 1, {64, 0, 0, 0}, true, false },
    { 1, {64, 0, 0, 0}, true, true },

    { 2, {64, 64, 0, 0}, false, false },
    { 2, {64, 64, 0, 0}, true, false },
    { 2, {64, 64, 0, 0}, true, true },

    { 3, {64, 64, 64, 0}, false, false },
    { 3, {64, 64, 64, 0}, true, false },
    { 3, {64, 64, 64, 0}, true, true },

    { 4, {64, 64, 64, 64}, false, false },
    { 4, {64, 64, 64, 64}, true, false },
    { 4, {64, 64, 64, 64}, true, true }
  };
  return descs[(int)(format - FORMAT_FIRST)];
}
// BlockCompression texture description
struct BCDesc {
  uint32_t block_width {0};
  uint32_t block_height {0};
  uint32_t block_bytes {0};
};

inline bool IsCompressedFormat(TextureFormat format) {
  return format >= FORMAT_BLOCK_COMPRESSION_START && format <= FORMAT_BLOCK_COMPRESSION_LAST;
}

// Make sure IsCompressed(format) == true
inline const BCDesc &GetBCDesc(TextureFormat format) {
  static const BCDesc descs[] = {
    {  8, 4, 4 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },

    {  8, 4, 4 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 },

    { 16, 4, 4 },
    { 16, 4, 4 },
    { 16, 5, 4 },
    { 16, 5, 4 },
    { 16, 5, 5 },
    { 16, 5, 5 },
    { 16, 6, 5 },
    { 16, 6, 5 },
    { 16, 6, 6 },
    { 16, 6, 6 },
    { 16, 8, 5 },
    { 16, 8, 5 },
    { 16, 8, 6 },
    { 16, 8, 6 },
    { 16, 8, 8 },
    { 16, 8, 8 },
    { 16, 10, 5 },
    { 16, 10, 5 },
    { 16, 10, 6 },
    { 16, 10, 6 },
    { 16, 10, 8 },
    { 16, 10, 8 },
    { 16, 10, 10 },
    { 16, 10, 10 },
    { 16, 12, 10 },
    { 16, 12, 10 },
    { 16, 12, 12 },
    { 16, 12, 12 },

    { 32, 8, 8 },
    { 32, 8, 8 },
    { 32, 16, 8 },
    { 32, 16, 8 },
    { 32, 8, 8 },
    { 32, 8, 8 },
    { 32, 16, 8 },
    { 32, 16, 8 },
    {  8, 4, 4 },
    {  8, 4, 4 },
    {  8, 8, 4 },
    {  8, 8, 4 },

    {  8, 4, 4 },
    {  8, 4, 4 },
    { 16, 4, 4 },
    { 16, 4, 4 }
  };
  return descs[(int)(format - FORMAT_BLOCK_COMPRESSION_START)];
}
}

#endif
