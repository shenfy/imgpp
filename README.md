# imgpp
Simple C++ Image Wrapper

## Installation
The following instructions are based on Ubuntu Linux 18.04 LTS.

### From Conan Bintray
The easiest way to use imgpp in your project is through [Conan](https://conan.io).

Please add the following bintray to your conan remote:
```bash
$ conan remote add bintray-shenfy https://api.bintray.com/conan/shenfy/oss
```
And ask for the latest imgpp in your project's conanfile.txt:
```
[requires]
imgpp/2.1.4@shenfy/testing
```
or conanfile.py:
```python
requires = "imgpp/2.1.4@shenfy/testing"
```

Please modify the string to request a specific version/channel of the library.

### Build from Source w/ Conan
Clone the repository, then install dependencies with conan:
```bash
$ mkdir build && cd build
$ conan install ..
```
You can then either build the library to be exported to your local conan cache:
```bash
$ conan build ..
$ conan create .. (user)/(channel)
```
or build it with cmake and install to your system library folder (default to /usr/local)
```bash
$ cmake ../src
$ make -j4
$ make install
```

### Create Header-Only Package
Clone the repository, and cd into the header_pkg directory:
```bash
$ cd header_pkg
$ conan create . (user)/(channel)
```

### Build from Source w/o Conan
This is strongly discouraged, but if you really want to, try something like this:
```bash
$ sudo apt install libjpeg-turbo8-dev libpng-dev
$ mkdir build
$ cd build
$ cmake ../src
$ make -j4
$ make install
```

### Build from source for iOS w/ Conan
1. `conan profile new ios`.
2. Edit `~/.conan/profiles/ios` as following.
```text
include(default)
[settings]
os=iOS
os.version=12.0
arch=armv8
[options]
[build_requires]
darwin-toolchain/1.0.8@theodelrieu/stable
[env]
```
3. `conan create --profile ios -o no_ext_libs=True . (user)/(channel)`.

## Usage
Include imgpp/imgpp.hpp if you need a wrapper for uncompressed regular images:
```c++
#include <imgpp/imgpp.hpp>
```
and imgpp/imgpp_bc.hpp for block-compressed images:
```c++
#include <imgpp/imgpp_bc.hpp>
```

WARNING: 3D compressed image (e.g. 4x4x4 blocks) are not supported yet!
