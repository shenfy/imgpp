#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <imgpp/imgpp.hpp>
#include <imgpp/loaders.hpp>
#include <imgpp/loadersext.hpp>
#include <imgpp/compositeimg.hpp>

int main(int argc, char* argv[]) {
  imgpp::CompositeImg img;
  std::unordered_map<std::string, std::string> custom_data;
  std::ifstream in(argv[1], std::ios::binary);
  in.seekg(0, std::ios::end);
  size_t length = in.tellg();
  in.seekg(0);
  std::vector<char> data(length);
  in.read((char*)data.data(), length);
  in.close();
  if (!LoadKTX(data.data(), length, img, custom_data, false)) {
  // if (!LoadKTX(argv[1], img, custom_data, false)) {
    std::cout << "Failed to load ktx!" << std::endl;
    return 1;
  }
  if (!WriteKTX(argv[2], img, custom_data, false)) {
    std::cout << "Failed to write ktx!" << std::endl;
    return 1;
  }
}
