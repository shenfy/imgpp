#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <imgpp/compositeimg.hpp>
#include <imgpp/texturedesc.hpp>
#include <imgpp/glhelper.hpp>
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

uint32_t ComputeKTXStorageSize(const CompositeImg &composite_img,
  const std::unordered_map<std::string, std::string> &custom_data) {
  uint32_t total_size = sizeof(FOURCC_KTX10) + sizeof(KTXHeader);
  // KeyValue Data size
  for (const auto &kv_pair: custom_data) {
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
bool LoadKTX(const char *fn, CompositeImg &composite_img,
  std::unordered_map<std::string, std::string> &custom_data, bool bottom_first) {
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
  auto texture_format = gl::TranslateFromGL(
    ktx_header.gl_internal_format,
    ktx_header.gl_format,
    ktx_header.gl_base_internal_format,
    ktx_header.gl_type);
  auto texture_target = GetTarget(ktx_header);
  if(texture_format == FORMAT_UNDEFINED) {
    std::cerr << "Unknown texture format" << std::endl;
    return false;
  }
  TextureDesc desc;
  desc.format = texture_format;
  desc.target = texture_target;
  desc.mipmap = ktx_header.number_of_mipmap_levels != 1;
  composite_img.TexDesc() = desc;
  std::array<uint32_t, 3> original_extent = {
    std::max(ktx_header.pixel_width, 1u),
    std::max(ktx_header.pixel_height, 1u),
    std::max(ktx_header.pixel_depth, 1u)
  };
  // Parse user-defined key-value data
  custom_data.clear();
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
    custom_data.insert_or_assign(std::move(key), std::move(value));
    // read padding
    in.read((char*)kv_data.data(), 3 - (kv_size + 3) % 4);
    kv_left -= 4 + ((kv_size + 3) / 4) * 4;
  }
  uint32_t img_data_size = total_size - ktx_header.bytes_of_key_value_data - sizeof(KTXHeader);
  ImgBuffer img_buf(img_data_size);
  in.read((char*)img_buf.GetBuffer(), img_data_size);
  if (IsCompressedFormat(texture_format)) {
    composite_img.SetBCSize(std::max(ktx_header.number_of_mipmap_levels, 1u),
      std::max(ktx_header.number_of_array_elements, 1u), std::max(ktx_header.number_of_faces, 1u),
      original_extent[0], original_extent[1], original_extent[2]);
    int img_number = composite_img.Levels() * composite_img.Layers() * composite_img.Faces();
    uint32_t offset = 0;
    uint8_t *buffer = img_buf.GetBuffer();
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      uint32_t img_size = *((uint32_t*)(buffer + offset));
      offset += 4;
      for (uint32_t layer = 0; layer < composite_img.Layers(); ++layer) {
        for (uint32_t face = 0; face < composite_img.Faces(); ++face) {
          composite_img.SetData(buffer + offset, level, layer, face);
          const BCImgROI &bc_roi = composite_img.BCROI(level, layer, face);
          offset += bc_roi.SlicePitch() * bc_roi.Depth();
        }
      }
    }
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
    composite_img.SetSize(std::max(ktx_header.number_of_mipmap_levels, 1u),
      std::max(ktx_header.number_of_array_elements, 1u), std::max(ktx_header.number_of_faces, 1u),
      original_extent[0], original_extent[1], original_extent[2], channel, bpc,
      pixel_desc.is_float, pixel_desc.is_signed, 4);
    // Uncompressed texture data matches a GL_UNPACK_ALIGNMENT of 4
    uint32_t offset = 0;
    uint8_t *buffer = img_buf.GetBuffer();
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      uint32_t img_size = *((uint32_t*)(buffer + offset));
      offset += 4;
      for (uint32_t layer = 0; layer < composite_img.Layers(); ++layer) {
        for (uint32_t face = 0; face < composite_img.Faces(); ++face) {
          composite_img.SetData(buffer + offset, level, layer, face);
          const ImgROI &roi = composite_img.ROI(level, layer, face);
          offset += roi.SlicePitch() * roi.Depth();
        }
      }
    }
  }
  composite_img.AddBuffer(std::move(img_buf));
  return true;
}

bool WriteKTX(const char *fn, const CompositeImg &composite_img,
  const std::unordered_map<std::string, std::string> &custom_data, bool bottom_first) {
  if (bottom_first) {
    return false;
  }
  std::ofstream out(fn, std::ios::binary);
  if (!out.good()) {
    return false;
  }
  const TextureDesc desc = composite_img.TexDesc();
  if (desc.format == FORMAT_UNDEFINED) {
    return false;
  }
  uint32_t total_size = ComputeKTXStorageSize(composite_img, custom_data);
  std::vector<uint8_t> data(total_size, 0);
  uint32_t offset = 0;
  std::memcpy(data.data(), FOURCC_KTX10, sizeof(FOURCC_KTX10));
  offset += sizeof(FOURCC_KTX10);
  gl::GLFormatDesc gl_desc = gl::TranslateToGL(desc.format);
  const ImgROI &roi = composite_img.ROI(0, 0, 0);
  KTXHeader &header = *reinterpret_cast<KTXHeader*>(data.data() + offset);
  header.endianness = 0x04030201;
  header.gl_type = (uint32_t)(gl_desc.type);
  if (composite_img.IsCompressed()) {
    header.gl_type_size = 1;
  } else {
    header.gl_type_size = gl::GetTypeSize(gl_desc.type);
  }
  header.gl_format = gl_desc.external_format;
  header.gl_internal_format = gl_desc.internal_format;
  header.gl_base_internal_format = gl_desc.base_internal_format;
  const TextureTarget &target = desc.target;
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
  if (desc.mipmap && composite_img.Levels() == 1) {
    header.number_of_mipmap_levels = 0;
  } else {
    header.number_of_mipmap_levels = composite_img.Levels();
  }
  offset += sizeof(KTXHeader);

  for (const auto &item: custom_data) {
    uint32_t &kv_data_size = *reinterpret_cast<uint32_t*>(data.data() + offset);
    std::memcpy(data.data() + offset + sizeof(uint32_t), item.first.data(), item.first.size());
    // Jump key and null terminate
    std::memcpy(data.data() + offset + sizeof(uint32_t) + item.first.size() + 1, item.second.data(), item.second.size());
    kv_data_size = item.first.size() + 1 + item.second.size();
    offset += sizeof(uint32_t) + 4 * ((kv_data_size + 3) / 4);
    header.bytes_of_key_value_data += sizeof(uint32_t) + 4 * ((kv_data_size + 3) / 4);
  }

  if (composite_img.IsCompressed()) {
    for (uint32_t level = 0; level < composite_img.Levels(); ++level) {
      uint32_t &img_size = *reinterpret_cast<uint32_t*>(data.data() + offset);
      img_size = 0;
      offset += sizeof(uint32_t);
      uint32_t face_size = CalcFaceSize(composite_img, level);
      if (desc.target == TARGET_CUBE) {
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
      if (desc.target == TARGET_CUBE) {
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

  out.write((char*)data.data(), data.size());
  return out.good();
}
}
