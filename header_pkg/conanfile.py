import os
from conans import ConanFile, CMake, tools

class ImgppHeaderConan(ConanFile):
    name = "imgpp_headers"
    version = "2.2.0"
    license = "MIT"
    author = "Fangyang Shen dev@shenfy.com"
    url = "https://github.com/shenfy/imgpp"
    description = "Basic C++ Image Wrapper Core Functionality Header Only Version"
    topics = ("Computer Graphics", "Computer Vision", "Image Processing")
    no_copy_source = True
    build_policy = "missing"
    exports_sources = "../src/include/*"

    def package(self):
        self.copy("imgpp/algorithms.hpp", dst="include/")
        self.copy("imgpp/bchelper.hpp", dst="include/")
        self.copy("imgpp/imgpp.hpp", dst="include/")
        self.copy("imgpp/bcdesc.hpp", dst="include/")
        self.copy("imgpp/imgbase.hpp", dst="include/")
        self.copy("imgpp/imgpp_bc.hpp", dst="include/")
        self.copy("imgpp/sampler.hpp", dst="include/")

    def package_id(self):
        self.info.header_only()
