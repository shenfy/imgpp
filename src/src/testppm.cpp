#include <iostream>
#include <vector>
#include <imgpp/imgpp.hpp>
#include <imgpp/loaders.hpp>

// this image has four pixels in 2x2 grid:
// black white
// white black
const char *test_file_name = "checkboard.ppm";

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

bool TestImg(bool bottom_first) {
  imgpp::Img img;

  // first load it
  if (!imgpp::LoadPPM(test_file_name, img, bottom_first)) {
    std::cerr << "Faied to load ppm" << std::endl;
    return false;
  }

  if (!CheckImg(img.ROI(), bottom_first)) {
    return false;
  }

  // write to disk and read back
  if (!imgpp::WritePPM(test_file_name, img.ROI(), bottom_first)) {
    std::cerr << "Faied to write ppm" << std::endl;
    return false;
  }
  if (!imgpp::LoadPPM(test_file_name, img, bottom_first)) {
    std::cerr << "Faied to load written ppm" << std::endl;
    return false;
  }
  if (!CheckImg(img.ROI(), bottom_first)) {
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
}
