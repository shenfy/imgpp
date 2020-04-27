# imgpp
Simple C++ Image Wrapper

## Installation
### Ubuntu Linux 18.04
Install dependencies with conan, and build with conan + cmake:
```bash
$ mkdir build && cd build
$ conan install ..
$ cmake ..
$ make install
```

Manually installing dependencies, and build using CMake (without conan):
```bash
$ sudo apt install libjpeg-turbo8-dev libpng-dev
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make install
```
