# imgpp
Simple C++ Image Wrapper

## Installation
### Ubuntu Linux 18.04
Install dependencies:

```bash
$ sudo apt install libjpeg-turbo8-dev libpng-dev
```

Build using conan and cmake
```bash
$ mkdir build && cd build
$ conan install ..
$ cmake ..
$ make install
```


Build using CMake (without conan):
```bash
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make install
```
