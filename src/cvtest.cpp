#include "../include/imgpp.hpp"
#include "../include/loaders.hpp"
#include "../include/loadersext.hpp"
#include "../include/opencvbinding.hpp"
#include <string>
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    cout << "testcase (input filename) (output prefix)" << endl;
    return 1;
  }

  imgpp::Img img;
  if (!imgpp::Load(argv[1], img)) {
    cerr << "Failed to load file " << argv[1] << endl;
  }

  cv::Mat mat = RefCVMat(img.ROI());

  cv::Mat gray_mat;
  cv::cvtColor(mat, gray_mat, cv::COLOR_RGB2GRAY);

  std::string out_fn = argv[2];

  imgpp::ImgROI out_roi = imgpp::RefROI(gray_mat);
  imgpp::Write((out_fn + ".bmp").c_str(), out_roi);

  imgpp::Img out_img;
  out_img.CopyFrom(out_roi);
  imgpp::Write((out_fn + ".jpg").c_str(), out_img.ROI());

  return 0;
}