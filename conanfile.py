from conans import ConanFile, CMake, tools
import os

class ImgppConan(ConanFile):
    name = "imgpp"
    version = "2.1.7"
    license = "MIT"
    author = "Fangyang Shen dev@shenfy.com"
    url = "https://github.com/shenfy/imgpp"
    description = "Basic C++ Image Wrapper"
    topics = ("Computer Graphics", "Computer Vision", "Image Processing")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [False], "no_ext_libs": [True, False]}
    default_options = {"shared": False, "no_ext_libs": False}
    generators = "cmake_paths"
    exports_sources = "src/*"

    def build(self):
        cmake = self._configure()
        cmake.build()

    def _configure(self):
        cmake = CMake(self)
        if self.options.no_ext_libs:
          cmake.definitions["IMGPP_NO_EXT_LIBS"] = True
        if self.settings.os == 'Android':
            if 'NDK' in os.environ:
                cmake.definitions['CMAKE_TOOLCHAIN_FILE'] = os.environ['NDK']
            cmake.definitions['ANDROID_STL'] = 'c++_static'
        cmake.configure(source_folder="src")
        return cmake

    def package(self):
        cmake = self._configure()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["imgpp"]

    def requirements(self):
        if not self.options.no_ext_libs:
          self.requires("libjpeg-turbo/2.0.2")
          self.requires("libpng/1.6.37")
