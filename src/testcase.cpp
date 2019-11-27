#include "../include/imgpp.hpp"
#include "../include/loaders.hpp"
#include "../include/loadersext.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "testcase (input filename) (output filename)" << endl;
    return -1;
  }

  std::string input_filename(argv[1]);
  std::string output_filename(argv[2]);

  cout << input_filename << " " << output_filename << endl;
  imgpp::Img img;
  imgpp::Load(input_filename.c_str(), img, false);
  imgpp::Write(output_filename.c_str(), img.ROI());

  // imgpp::Img resized;
  // resized.SetSize(480, 480, 1, 3, 8);
  // imgpp::ipp_binding::ResizeInfo<imgpp::ipp_binding::INTERP_CUBIC> resize_info(
  //  img.ROI(), resized.ROI(), 0, 0.0f, 0.5f);
  // resize_info.Resize(img.ROI().data_, resized.ROI().data_);
  // imgpp::Write("resized.png", resized.ROI());

  // imgpp::Img mem_img;
  // ifstream infile(input_filename.c_str(), std::ios::binary);
  // infile.seekg(0, std::ios::end);
  // auto pos = infile.tellg();
  // size_t length = (size_t)pos;
  // char *buffer = new char[length];
  // infile.seekg(0, std::ios::beg);
  // infile.read(buffer, length);
  // infile.close();

  // // imgpp::DecompressPNG(buffer, length, mem_img, true);
  // // imgpp::DecompressJPEG(buffer, length, mem_img);
  // imgpp::LoadPFM(buffer, length, mem_img, true);
  // delete[] buffer;
  // imgpp::Write(output_filename.c_str(), mem_img.ROI(), true);

  return 0;
}
