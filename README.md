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
imgpp/2.0.0@shenfy/testing
```
or conanfile.py:
```python
requires = "imgpp/2.0.0@shenfy/testing"
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
