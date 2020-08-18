from conans import ConanFile, CMake, tools


class ImgppConan(ConanFile):
    name = "imgpp"
    version = "2.1.0"
    license = "MIT"
    author = "Fangyang Shen dev@shenfy.com"
    url = "https://github.com/shenfy/imgpp"
    description = "Basic C++ Image Wrapper"
    topics = ("Computer Graphics", "Computer Vision", "Image Processing")
    settings = "os", "compiler", "build_type", "arch"
    requires = "libjpeg-turbo/2.0.2", "libpng/1.6.37"
    options = {"shared": [False]}
    default_options = {"shared": False}
    generators = "cmake_paths"
    exports_sources = "src/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["imgpp"]

