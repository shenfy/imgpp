#include <iostream>
#include <vector>
#include <string>
#include <imgpp/imgpp.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>
#include <imgpp/compositeimg.hpp>

int main(int argc, char* argv[]) {
  imgpp::CompositeImg img;
  if (!LoadKTX(argv[1], img, false)) {
    std::cout << "Failed to load ktx!" << std::endl;
    return 1;
  }
  if (!WriteKTX(argv[2], img, false)) {
    std::cout << "Failed to write ktx!" << std::endl;
    return 1;
  }
}
