#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <imgpp/imgpp.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>
#include <imgpp/compositeimg.hpp>


// texture array, img on layer1 equals 255 - img on layer0
const char *fn = "test.ktx";
using namespace imgpp;
bool Check(const imgpp::CompositeImg &img, const std::unordered_map<std::string, std::string> &kv_data) {
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
  const auto &desc = img.TexDesc();
  if (desc.format != FORMAT_RGB8_UNORM_PACK8 || desc.target != TARGET_2D_ARRAY || desc.mipmap) {
    std::cerr << "Texture desc error!" << std::endl;
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

int main() {
  imgpp::CompositeImg img;
  std::unordered_map<std::string, std::string> kv_data;
  std::ifstream in(fn, std::ios::binary);
  in.seekg(0, std::ios::end);
  size_t length = in.tellg();
  in.seekg(0);
  std::vector<char> data(length);
  in.read((char*)data.data(), length);
  in.close();
  if (!LoadKTX(data.data(), length, img, kv_data, false)) {
    std::cerr << "Failed to load ktx from memory!" << std::endl;
    return 1;
  }
  if (!WriteKTX(fn, img, kv_data, false)) {
    std::cerr << "Failed to write ktx!" << std::endl;
    return 1;
  }
  if (!LoadKTX(fn, img, kv_data, false)) {
    std::cerr << "Failed to load ktx file!" << std::endl;
    return 1;
  }
  if (Check(img, kv_data)) {
    return 0;
  } else {
    return 1;
  }
}
