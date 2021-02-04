#include <imgpp/imgpp.hpp>

int main() {
  // Test clone
  {
    imgpp::Img image(2, 2, 1, 1, 32, true, true, 1);
    image.ROI().At<float>(0, 0) = 1.0f;
    image.ROI().At<float>(0, 1) = 2.0f;
    image.ROI().At<float>(1, 0) = 3.0f;
    image.ROI().At<float>(1, 1) = 4.0f;
    imgpp::Img clone = image.Clone();
    // Make sure this is a deep copy.
    if (image.Data().GetBuffer() == clone.Data().GetBuffer()) {
      return 1;
    }
    // Check data.
    if (std::memcmp(image.Data().GetBuffer(), clone.Data().GetBuffer(), image.Data().GetLength())) {
      return 1;
    }
  }
  return 0;
}
