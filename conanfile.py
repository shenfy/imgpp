from conans import ConanFile, CMake, tools


class ImgppConan(ConanFile):
    name = "imgpp"
    version = "2.1.4"
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
        cmake = CMake(self)
        if self.options.no_ext_libs:
          cmake.definitions["IMGPP_NO_EXT_LIBS"] = True
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        if self.options.no_ext_libs:
          cmake.definitions["IMGPP_NO_EXT_LIBS"] = True
        cmake.configure(source_folder="src")
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["imgpp"]

    def requirements(self):
        if not self.options.no_ext_libs:
          self.requires("libjpeg-turbo/2.0.2")
          self.requires("libpng/1.6.37")