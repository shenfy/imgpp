#include <imgpp/imgpp.hpp>
#include <imgpp/blockimg.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>
#include <imgpp/sampler.hpp>
#include <string>
#include <iostream>


int main(int argc, char *argv[]) {
  imgpp::Img img;
  if (!imgpp::Load("test.png", img, false)) {
    std::cerr << "failed to load png" << std::endl;
    return 1;
  }

  // sample the CC logo images
  enum {kWhite = 255, kBlack = 0};
  enum {kNumPts = 3};
  static const uint32_t pos[] = {100, 40, 260, 40, 270, 190};
  static const uint8_t vals[] = {kWhite, kWhite, kBlack};

  for (size_t idx = 0; idx < kNumPts; idx++) {
    for (uint8_t ch = 0; ch < 3; ch++) {
      if (img.ROI().At<uint8_t>(pos[idx * 2], pos[idx * 2 + 1], ch) != vals[idx]) {
        std::cerr << "checking png value failed!" << std::endl;
        return 1;
      }

      auto color = imgpp::Tex2DBilinear<uint8_t>(
        img.ROI(), pos[idx * 2] + .5f, pos[idx * 2 + 1] + .5f);
    }
  }

  // bson serialization/deserialization test
  if (!imgpp::Write("test.bson", img.ROI(), false)) {
    std::cerr << "failed to write bson" << std::endl;
    return 1;
  }

  imgpp::Img img_bson;
  if (!imgpp::Load("test.bson", img_bson, false)) {
    std::cerr << "failed to load bson" << std::endl;
    return 1;  // test failed
  }

  for (uint32_t y = 0; y < img.ROI().Height(); ++y) {
    for (uint32_t x = 0; x < img.ROI().Width(); ++x) {
      if (img.ROI().At<uint8_t>(x, y, 0) != img_bson.ROI().At<uint8_t>(x, y, 0)) {
        std::cerr << "checking bson value failed!" << std::endl;
        return 1;
      }
    }
  }

  return 0;
}
