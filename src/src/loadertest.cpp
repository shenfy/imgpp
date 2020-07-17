#include <iostream>
#include <vector>
#include <string>
#include <imgpp/imgpp.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>

// this image has four pixels in 2x2 grid:
// black white
// white black
const char *input_fn = "checkboard";

bool CheckImg(const imgpp::ImgROI &roi, bool bottom_first) {
  // check metadata
  if (roi.Width() != 2 || roi.Height() != 2 || roi.Depth() != 1
    || roi.Channel() != 3 || roi.BPC() != 8) {
    std::cerr << "Wrong meta data" << std::endl;
    return false;
  }

  // check content
  std::vector<uint8_t> expected_data;
  if (bottom_first) {
    expected_data = {255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255};
  } else {
    expected_data = {0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0};
  }
  if (memcmp(expected_data.data(), roi.GetData(), expected_data.size()) != 0) {
    std::cerr << "Wrong content" << std::endl;
    return false;
  }

  return true;
}

bool CheckImgLossy(const imgpp::ImgROI &roi, bool bottom_first) {
  // check metadata
  if (roi.Width() != 2 || roi.Height() != 2 || roi.Depth() != 1
    || roi.Channel() != 3 || roi.BPC() != 8) {
    std::cerr << "Wrong meta data" << std::endl;
    return false;
  }

  // check content
  std::vector<uint8_t> expected_data;
  if (bottom_first) {
    expected_data = {255, 255, 255, 0, 0, 0, 0, 0, 0, 255, 255, 255};
  } else {
    expected_data = {0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0};
  }
  for (size_t idx = 0; idx < expected_data.size(); ++idx) {
    auto val = *((uint8_t*)roi.GetData() + idx);
    if (expected_data[idx] == 0 && val > 20
      || expected_data[idx] == 255 && val < 230) {
      std::cerr << "Wrong lossy content" << std::endl;
      return false;
    }
  }

  return true;
}

bool TestImg(bool bottom_first) {
  imgpp::Img src_img;
  imgpp::Img img;

  std::string fn = input_fn;
  std::string out_fn = "loadertest";

  // first load it
  if (!imgpp::LoadPPM((fn + ".ppm").c_str(), src_img, bottom_first)) {
    std::cerr << "Faied to load ppm" << std::endl;
    return false;
  }

  if (!CheckImg(src_img.ROI(), bottom_first)) {
    return false;
  }

  // write to disk and read back
  if (!imgpp::WritePPM((out_fn + ".ppm").c_str(), src_img.ROI(), bottom_first)) {
    std::cerr << "Faied to write ppm" << std::endl;
    return false;
  }
  if (!imgpp::LoadPPM((out_fn + ".ppm").c_str(), img, bottom_first)) {
    std::cerr << "Faied to load written ppm" << std::endl;
    return false;
  }
  if (!CheckImg(img.ROI(), bottom_first)) {
    return false;
  }

  // bmp
  if (!imgpp::WriteBMP((out_fn + ".bmp").c_str(), src_img.ROI(), bottom_first)) {
    std::cerr << "Faied to write bmp" << std::endl;
    return false;
  }
  if (!imgpp::LoadBMP((out_fn + ".bmp").c_str(), img, bottom_first)) {
    std::cerr << "Faied to load written bmp" << std::endl;
    return false;
  }
  if (!CheckImg(img.ROI(), bottom_first)) {
    return false;
  }

  // png
  if (!imgpp::WritePNG((out_fn + ".png").c_str(), src_img.ROI(), bottom_first)) {
    std::cerr << "Faied to write png" << std::endl;
    return false;
  }
  if (!imgpp::LoadPNG((out_fn + ".png").c_str(), img, bottom_first)) {
    std::cerr << "Faied to load written png" << std::endl;
    return false;
  }
  if (!CheckImg(img.ROI(), bottom_first)) {
    return false;
  }

  // jpeg
  if (!imgpp::WriteJPEG((out_fn + ".jpg").c_str(), src_img.ROI(), bottom_first)) {
    std::cerr << "Faied to write jpg" << std::endl;
    return false;
  }
  if (!imgpp::LoadJPEG((out_fn + ".jpg").c_str(), img, bottom_first)) {
    std::cerr << "Faied to load written jpg" << std::endl;
    return false;
  }
  if (!CheckImgLossy(img.ROI(), bottom_first)) {
    return false;
  }

  return true;
}

int main() {
  // test both bottom_first and top_first reading and writing
  if (!TestImg(false)) {
    return 1;
  }
  if (!TestImg(true)) {
    return 1;
  }
  return 0;
}
