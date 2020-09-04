import os
from conans import ConanFile, CMake, tools

class ImgppHeaderConan(ConanFile):
    name = "imgpp_headers"
    version = "2.1.3"
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
        self.copy("imgpp/imgpp.hpp", dst="include/")
        self.copy("imgpp/imgbase.hpp", dst="include/")
        self.copy("imgpp/sampler.hpp", dst="include/")
        self.copy("imgpp/blockimg.hpp", dst="include/")
        self.copy("imgpp/compositeimg.hpp", dst="include/")
        self.copy("imgpp/texturedesc.hpp", dst="include/")
        self.copy("imgpp/texturehelper.hpp", dst="include/")
        self.copy("imgpp/typetraits.hpp", dst="include/")
        self.copy("imgpp/glmtraits.hpp", dst="include/")

    def package_id(self):
        self.info.header_only()
