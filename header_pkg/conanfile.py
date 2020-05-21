import os
from conans import ConanFile, CMake, tools


class ImgppHeaderConan(ConanFile):
    name = "imgpp_headers"
    version = "2.0.1"
    license = "MIT"
    author = "Fangyang Shen dev@shenfy.com"
    url = "https://github.com/shenfy/imgpp"
    description = "Basic C++ Image Wrapper Core Functionality Header Only Version"
    topics = ("Computer Graphics", "Computer Vision", "Image Processing")
    no_copy_source = True
    build_policy = "missing"
    exports_sources = "../src/include/*"

    def package(self):
        self.copy("imgpp/algorithms.hpp")
        self.copy("imgpp/bchelper.hpp")
        self.copy("imgpp/imgpp.hpp")
        self.copy("imgpp/bcdesc.hpp")
        self.copy("imgpp/imgbase.hpp")
        self.copy("imgpp/imgpp_bc.hpp")
        self.copy("imgpp/sampler.hpp")

    def package_id(self):
        self.info.header_only()
