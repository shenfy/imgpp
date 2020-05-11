#include <imgpp/imgpp.hpp>
#include <imgpp/bcimg.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>
#include <string>

int main(int argc, char *argv[]) {
  imgpp::Img img;
  if (!imgpp::Load("test.png", img, false)) {
    return 1;
  }

  if (!imgpp::Write("test.bson", img.ROI(), false)) {
    return 1;
  }

  imgpp::Img img_bson;
  if (!imgpp::Load("test.bson", img_bson)) {
    return 1;  // test failed
  }

  for (uint32_t y = 0; y < img.ROI().Height(); ++y) {
    for (uint32_t x = 0; x < img.ROI().Width(); ++x) {
      if (img.ROI().At<uint8_t>(x, y, 0) != img_bson.ROI().At<uint8_t>(x, y, 0)) {
        return 1;
      }
    }
  }

  return 0;
}
