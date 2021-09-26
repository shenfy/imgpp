#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <imgpp/imgpp.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>
#include <imgpp/compositeimg.hpp>


// texture array, img on layer1 equals 255 - img on layer0
static const char *kRGBFn = "test.ktx";
static const char *kASTC8x8Fn = "astc8x8.ktx";
using namespace imgpp;
bool CheckKV(const std::unordered_map<std::string, std::string> &kv_data) {
  if (kv_data.size() != 1) {
    std::cerr << "kv data size error!" << std::endl;
    return false;
  }
  auto it = kv_data.find("KTXorientation");
  if (it == kv_data.end()) {
    std::cerr << "KTXorientation not found in kv data!" << std::endl;
    return false;
  }
  if (it->second.substr(0, 11) != std::string("S=r,T=d,R=i")) {
    std::cerr << "KTXorientation value error!" << std::endl;
    return false;
  }
  return true;
}

bool CheckRGB(const imgpp::CompositeImg &img, const std::unordered_map<std::string, std::string> &kv_data) {
  if (!CheckKV(kv_data)) {
    return false;
  }
  const auto &desc = img.TexDesc();
  if (desc.format != FORMAT_RGB8_UNORM_PACK8 || desc.target != TARGET_2D_ARRAY || desc.mipmap) {
    std::cerr << "RGB Texture desc error!" << std::endl;
    return false;
  }
  const auto &roi0 = img.ROI(0, 0, 0);
  const auto &roi1 = img.ROI(0, 1, 0);
  if (img.Levels() != 1 || img.Layers() != 2 || img.Faces() != 1 ||
    roi0.Width() != 980 || roi0.Height() != 240 || roi0.Depth() != 1 || roi0.Channel() != 3 ||
    roi1.Width() != 980 || roi1.Height() != 240 || roi1.Depth() != 1 || roi1.Channel() != 3) {
    std::cerr << "Dimensions error!" << std::endl;
    return false;
  }
  int xs[] = {262, 618, 750, 915};
  int ys[] = {43, 157, 223, 91};
  uint8_t expected_data[] = {255, 255, 255, 0};
  for (int i = 0; i < 4; ++i) {
    if (roi0.At<uint8_t>(xs[i], ys[i]) != expected_data[i] ||
      roi1.At<uint8_t>(xs[i], ys[i]) != 255 - expected_data[i]) {
      std::cerr << "Img data error!" << std::endl;
      return false;
    }
  }
  return true;
}

bool CheckASTC(const imgpp::CompositeImg &img, const std::unordered_map<std::string, std::string> &kv_data) {
  if (!CheckKV(kv_data)) {
    return false;
  }
  const auto &desc = img.TexDesc();
  if (!IsCompressedFormat(desc.format) || desc.format != FORMAT_RGBA_ASTC_8X8_SRGB_BLOCK16 || desc.target != TARGET_2D) {
    std::cerr << "ASTC Texture desc error!" << std::endl;
    return false;
  }
  auto block_size = GetBlockSize(desc.format);
  if (block_size.block_width != 8 || block_size.block_height != 8 || block_size.block_bytes != 16) {
    std::cerr << "Block size info error!" << std::endl;
    return false;
  }
  return true;
}

std::vector<char> LoadKTXData(const char *fn) {
  std::ifstream in(fn, std::ios::binary);
  in.seekg(0, std::ios::end);
  size_t length = in.tellg();
  in.seekg(0);
  std::vector<char> data(length);
  in.read((char*)data.data(), length);
  in.close();
  return data;
}

int main() {
  imgpp::CompositeImg img;
  std::unordered_map<std::string, std::string> kv_data;
  auto rgb_data = LoadKTXData(kRGBFn);
  if (!LoadKTX(rgb_data.data(), rgb_data.size(), img, kv_data, false)) {
    std::cerr << "Failed to load ktx from memory!" << std::endl;
    return 1;
  }
  if (!WriteKTX(kRGBFn, img, kv_data, false)) {
    std::cerr << "Failed to write ktx!" << std::endl;
    return 1;
  }
  if (!LoadKTX(kRGBFn, img, kv_data, false)) {
    std::cerr << "Failed to load ktx file!" << std::endl;
    return 1;
  }
  if (!CheckRGB(img, kv_data)) {
    return 1;
  }

  kv_data.clear();
  imgpp::CompositeImg astc_img;
  if (!LoadKTX(kASTC8x8Fn, astc_img, kv_data, false)) {
    std::cerr << "Failed to load ktx from memory!" << std::endl;
    return 1;
  }
  if (!CheckASTC(astc_img, kv_data)) {
    return 1;
  } else {
    return 0;
  }
}
