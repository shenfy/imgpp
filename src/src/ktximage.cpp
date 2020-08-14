#include <algorithm>
#include <iostream>
#include <fstream>
#include <imgpp/compositeimg.hpp>
#include <imgpp/textureformat.hpp>
#include <imgpp/gl.hpp>
#include <imgpp/loaders.hpp>

namespace {
using namespace imgpp;
// KTX 1.1 specification
// https://www.khronos.org/opengles/sdk/tools/KTX/file_format_spec/
static unsigned char const FOURCC_KTX10[] = {
  0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A};
struct KTXHeader {
  uint32_t endianness;
  uint32_t gl_type;
  uint32_t gl_type_size;
  uint32_t gl_format;
  uint32_t gl_internal_format;
  uint32_t gl_base_internal_format;
  uint32_t pixel_width;
  uint32_t pixel_height;
  uint32_t pixel_depth;
  uint32_t number_of_array_elements;
  uint32_t number_of_faces;
  uint32_t number_of_mipmap_levels;
  uint32_t bytes_of_key_value_data;
};

TextureTarget GetTarget(KTXHeader header) {
  if(header.number_of_faces > 1) {
    if(header.number_of_array_elements > 0)
      return TARGET_CUBE_ARRAY;
    else
      return TARGET_CUBE;
  }
  else if(header.number_of_array_elements > 0)
  {
    if(header.pixel_height == 0)
      return TARGET_1D_ARRAY;
    else
      return TARGET_2D_ARRAY;
  }
  else if(header.pixel_height == 0)
    return TARGET_1D;
  else if(header.pixel_depth > 0)
    return TARGET_3D;
  else
    return TARGET_2D;
}

uint32_t CalcFaceSize(const CompositeImg &composite_img, uint32_t level) {
  uint32_t face_size = 0;
  if (composite_img.IsCompressed()) {
    const BCImgROI &bc_roi = composite_img.BCROI(level, 0, 0);
    face_size = bc_roi.SlicePitch() * bc_roi.Depth();
  } else {
    const ImgROI &roi = composite_img.ROI(level, 0, 0);
    face_size = roi.SlicePitch() * roi.Depth();
  }
  return face_size;
}

uint32_t ComputeKTXStorageSize(const CompositeImg &composite_img) {
  uint32_t total_size = sizeof(FOURCC_KTX10) + sizeof(KTXHeader);
  // KeyValue Data size
  const auto &kv_map = composite_img.Info();
  for (const auto &kv_pair: kv_map) {
    total_size += sizeof(uint32_t) +
      4 * ((kv_pair.first.size() + 1 + kv_pair.second.size() + 3) / 4);
  }
  for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
    total_size += sizeof(uint32_t);
    total_size += composite_img.Layers() * composite_img.Faces() * CalcFaceSize(composite_img, level);
  }
  return total_size;
}

inline std::array<uint32_t, 3> CalcExtent(const std::array<uint32_t, 3> &original_extent,
  TextureTarget target, uint32_t level) {
  std::array<uint32_t, 3> extent = original_extent;
  extent[0] >>= level;
  if (!IsTarget1d(target)) {
    extent[1] >>= level;
  }
  if (IsTarget3d(target)) {
    extent[2] >>= level;
  }
  return extent;
}
}

namespace imgpp {
bool LoadKTX(const char *fn, CompositeImg &composite_img, bool bottom_first) {
  if (bottom_first) {
    std::cerr << "Bottom first not support yet!" << std::endl;
    return false;
  }
  std::ifstream in(fn, std::ios::binary);
  if(!in.good()) {
    return false;
  }

  in.seekg(0, std::ios::end);
  uint64_t total_size = in.tellg();
  in.seekg(0);
  std::vector<char> four_cc(sizeof(FOURCC_KTX10));
  in.read((char*)four_cc.data(), four_cc.size());
  if (memcmp(four_cc.data(), FOURCC_KTX10, four_cc.size()) != 0) {
    std::cerr << "Unknown file format!" << std::endl;
    return false;
  }
  KTXHeader ktx_header;
  in.read((char *)(&ktx_header), sizeof(KTXHeader));
  uint32_t img_data_size = total_size - ktx_header.bytes_of_key_value_data - sizeof(KTXHeader);
  auto texture_format = gl::Find(
    static_cast<gl::InternalFormat>(ktx_header.gl_internal_format),
    static_cast<gl::ExternalFormat>(ktx_header.gl_format),
    static_cast<gl::TypeFormat>(ktx_header.gl_type));
  auto texture_target = GetTarget(ktx_header);
  if(texture_format == FORMAT_UNDEFINED) {
    std::cerr << "Unknown texture format" << std::endl;
    return false;
  }
  composite_img.SetFormat(texture_format);
  composite_img.SetTarget(texture_target);
  composite_img.SetSize(
    std::max(ktx_header.number_of_mipmap_levels, 1u),
    std::max(ktx_header.number_of_array_elements, 1u),
    std::max(ktx_header.number_of_faces, 1u), img_data_size);
  if (ktx_header.number_of_mipmap_levels == 0) {
    composite_img.AutoGenerateMipmaps();
  }
  std::array<uint32_t, 3> original_extent = {
    std::max(ktx_header.pixel_width, 1u),
    std::max(ktx_header.pixel_height, 1u),
    std::max(ktx_header.pixel_depth, 1u)
  };
  // Parse user-defined key-value data
  int64_t kv_left = ktx_header.bytes_of_key_value_data;
  while (kv_left > 0) {
    uint32_t kv_size = 0;
    std::string kv_data;
    in.read((char*)(&kv_size), sizeof(uint32_t));
    kv_data.resize(kv_size);
    in.read((char*)kv_data.data(), kv_size);
    auto null_char_pos = std::find(kv_data.begin(), kv_data.end(), 0) - kv_data.begin();

    std::string key = kv_data.substr(0, null_char_pos);
    std::string value = kv_data.substr(null_char_pos + 1);
    composite_img.SetInfo(std::move(key), std::move(value));
    // read padding
    in.read((char*)kv_data.data(), 3 - (kv_size + 3) % 4);
    kv_left -= 4 + ((kv_size + 3) / 4) * 4;
  }

  in.read((char*)composite_img.Data().GetBuffer(), img_data_size);
  if (IsCompressedFormat(texture_format)) {
    const BCDesc& bc_desc = GetBCDesc(texture_format);
    int img_number = composite_img.Levels() * composite_img.Layers() * composite_img.Faces();
    std::vector<BCImgROI> bc_rois(img_number);
    uint32_t offset = 0;
    uint8_t *buffer = composite_img.Data().GetBuffer();
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      auto extent = CalcExtent(original_extent, texture_target, level);
      uint32_t img_size = *((uint32_t*)(buffer + offset));
      offset += 4;
      uint32_t pitch = BCImgROI::CalcPitch(texture_format, extent[0]);
      uint32_t slice_pitch = pitch * extent[1];
      for (uint32_t layer = 0; layer < composite_img.Layers(); ++layer) {
        for (uint32_t face = 0; face < composite_img.Faces(); ++face) {
          uint32_t flatten_id = level * composite_img.Layers() * composite_img.Faces() +
            layer * composite_img.Faces() + face;
          BCImgROI bc_roi(buffer + offset, texture_format,
            extent[0], extent[1], extent[2]);
          bc_rois[flatten_id] = bc_roi;
          offset += slice_pitch * extent[2];
        }
      }
    }
    composite_img.AddBCROI(std::move(bc_rois));
  } else {
    const PixelDesc& pixel_desc = GetPixelDesc(texture_format);
    uint32_t bpp = 0;
    uint32_t bpc = pixel_desc.bpcs[0];
    bool is_norm = true;
    for (uint32_t c = 0; c < pixel_desc.components; ++c) {
      bpp += pixel_desc.bpcs[c];
      if (bpc != pixel_desc.bpcs[c]) {
        is_norm = false;
      }
    }
    uint32_t channel = pixel_desc.components;
    if (!is_norm) {
      channel = 1;
      bpc = bpp;
    }
    // Uncompressed texture data matches a GL_UNPACK_ALIGNMENT of 4
    int img_number = composite_img.Levels() * composite_img.Layers() * composite_img.Faces();
    std::vector<ImgROI> rois(img_number);
    uint32_t offset = 0;
    uint8_t *buffer = composite_img.Data().GetBuffer();
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      auto extent = CalcExtent(original_extent, texture_target, level);
      uint32_t img_size = *((uint32_t*)(buffer + offset));
      offset += 4;
      uint32_t pitch = ImgROI::CalcPitch(extent[0], channel, bpc, 4);
      uint32_t slice_pitch = pitch * extent[1];
      for (uint32_t layer = 0; layer < composite_img.Layers(); ++layer) {
        for (uint32_t face = 0; face < composite_img.Faces(); ++face) {
          uint32_t flatten_id = level * composite_img.Layers() * composite_img.Faces() +
            layer * composite_img.Faces() + face;
          ImgROI roi(buffer + offset, extent[0], extent[1], extent[2],
            channel, bpc, pitch, slice_pitch, pixel_desc.is_float, pixel_desc.is_signed);
          rois[flatten_id] = roi;
          offset += slice_pitch * extent[2];
        }
      }
    }
    composite_img.AddROI(std::move(rois));
  }
  return true;
}

bool WriteKTX(const char *fn, const CompositeImg &composite_img, bool bottom_first) {
  if (bottom_first) {
    return false;
  }
  if (composite_img.Format() == FORMAT_UNDEFINED) {
    return false;
  }
  uint32_t total_size = ComputeKTXStorageSize(composite_img);
  std::vector<uint8_t> data(total_size, 0);
  uint32_t offset = 0;
  std::memcpy(data.data(), FOURCC_KTX10, sizeof(FOURCC_KTX10));
  offset += sizeof(FOURCC_KTX10);
  gl::FormatDesc gl_desc = gl::Translate(composite_img.Format());
  const ImgROI &roi = composite_img.ROI(0, 0, 0);
  KTXHeader &header = *reinterpret_cast<KTXHeader*>(data.data() + offset);
  header.endianness = 0x04030201;
  header.gl_type = (uint32_t)(gl_desc.type);
  if (composite_img.IsCompressed()) {
    const auto &bc_desc = GetBCDesc(composite_img.Format());
    header.gl_type_size = bc_desc.block_bytes;
  } else {
    header.gl_type_size = 1;
  }
  header.gl_format = gl_desc.external;
  header.gl_internal_format = gl_desc.internal;
  header.gl_base_internal_format = gl_desc.external;
  const TextureTarget &target = composite_img.Target();
  if (composite_img.IsCompressed()) {
    const BCImgROI &bc_roi = composite_img.BCROI(0, 0, 0);
    header.pixel_width = bc_roi.Width();
    header.pixel_height = IsTarget1d(target) ? 0 : bc_roi.Height();
    header.pixel_depth = IsTarget3d(target) ? bc_roi.Depth() : 0;
  } else {
    const ImgROI &roi = composite_img.ROI(0, 0, 0);
    header.pixel_width = roi.Width();
    header.pixel_height = IsTarget1d(target) ? 0 : roi.Height();
    header.pixel_depth = IsTarget3d(target) ? roi.Depth() : 0;
  }
  header.number_of_array_elements = IsTargetArray(target) ? composite_img.Layers() : 0;
  header.number_of_faces = IsTargetCube(target) ? 6 : 1;
  header.number_of_mipmap_levels = composite_img.NeedGenerateMipmaps() ? 0 : composite_img.Levels();
  offset += sizeof(KTXHeader);

  const auto &kv_map = composite_img.Info();
  if (!kv_map.empty()) {
    for (const auto &item: kv_map) {
      uint32_t &kv_data_size = *reinterpret_cast<uint32_t*>(data.data() + offset);
      std::memcpy(data.data() + offset + sizeof(uint32_t), item.first.data(), item.first.size());
      // Jump key and null terminate
      std::memcpy(data.data() + offset + sizeof(uint32_t) + item.first.size() + 1, item.second.data(), item.second.size());
      kv_data_size = item.first.size() + 1 + item.second.size();
      offset += sizeof(uint32_t) + 4 * ((kv_data_size + 3) / 4);
      header.bytes_of_key_value_data += sizeof(uint32_t) + 4 * ((kv_data_size + 3) / 4);
    }
  }
  if (composite_img.IsCompressed()) {
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      uint32_t &img_size = *reinterpret_cast<uint32_t*>(data.data() + offset);
      img_size = 0;
      offset += sizeof(uint32_t);
      uint32_t face_size = CalcFaceSize(composite_img, level);
      if (composite_img.Target() == TARGET_CUBE) {
        img_size = face_size;
      } else {
        img_size = composite_img.Layers() * composite_img.Faces() * face_size;
      }
      for (uint32_t layer = 0; layer < composite_img.Layers(); ++layer) {
        for (uint32_t face = 0; face < composite_img.Faces(); ++face) {
          std::memcpy(data.data() + offset, composite_img.BCROI(level, layer, face).GetData(), face_size);
          offset += face_size;
        }
      }
    }
  } else {
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      uint32_t &img_size = *reinterpret_cast<uint32_t*>(data.data() + offset);
      img_size = 0;
      offset += sizeof(uint32_t);
      uint32_t face_size = CalcFaceSize(composite_img, level);
      if (composite_img.Target() == TARGET_CUBE) {
        img_size = face_size;
      } else {
        img_size = composite_img.Layers() * composite_img.Faces() * face_size;
      }
      for (uint32_t layer = 0; layer < composite_img.Layers(); ++layer) {
        for (uint32_t face = 0; face < composite_img.Faces(); ++face) {
          std::memcpy(data.data() + offset, composite_img.ROI(level, layer, face).GetData(), face_size);
          offset += face_size;
        }
      }
    }
  }
  std::ofstream out(fn, std::ios::binary);
  if (!out.good()) {
    return false;
  }
  out.write((char*)data.data(), data.size());
  return true;
}
}
