cmake_minimum_required(VERSION 3.16)
project(imgpp VERSION 2.1.9 LANGUAGES CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(IMGPP_HEADER
  include/imgpp/algorithms.hpp
  include/imgpp/texturedesc.hpp
  include/imgpp/texturehelper.hpp
  include/imgpp/glhelper.hpp
  include/imgpp/imgbase.hpp
  include/imgpp/imgpp.hpp
  include/imgpp/blockimg.hpp
  include/imgpp/compositeimg.hpp
  include/imgpp/loaders.hpp
  include/imgpp/sampler.hpp
  include/imgpp/typetraits.hpp
  include/imgpp/glmtraits.hpp)

if (NOT DEFINED IMGPP_NO_EXT_LIBS)
  set(IMGPP_HEADER ${IMGPP_HEADER}
    include/imgpp/loadersext.hpp
    include/imgpp/opencvbinding.hpp)
endif()

# static library
add_library(imgpp STATIC)
target_sources(imgpp PRIVATE
  src/bmpimg.cpp src/pfmimg.cpp src/ppmimg.cpp
  src/helper.cpp src/bson.cpp src/glhelper.cpp src/ktximage.cpp)

if (NOT DEFINED IMGPP_NO_EXT_LIBS)
target_sources(imgpp PRIVATE
  src/pngimg.cpp src/jpegimg.cpp)
target_link_libraries(imgpp PUBLIC CONAN_PKG::libjpeg-turbo CONAN_PKG::libpng)
endif()

target_compile_features(imgpp PUBLIC cxx_std_17)
set_target_properties(imgpp PROPERTIES PUBLIC_HEADER
  "${IMGPP_HEADER}")
target_include_directories(imgpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
set_target_properties(imgpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(TARGETS imgpp EXPORT imgppTargets
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/imgpp)

# test excutable
enable_testing()

if (NOT DEFINED IMGPP_NO_EXT_LIBS)
  add_executable(basictest src/testcase.cpp)
  add_custom_command(
    TARGET basictest
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resrc/test.png ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copy test images to build folder")
  target_link_libraries(basictest PRIVATE imgpp)
  add_test(basic bin/basictest)

  add_executable(loadertest src/loadertest.cpp)
  add_custom_command(
    TARGET basictest
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resrc/checkboard.ppm ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Copy test ppm image to build folder")
  target_link_libraries(loadertest PRIVATE imgpp)
  add_test(loaders bin/loadertest)
endif()

add_executable(clonetest src/clonetest.cpp)
target_link_libraries(clonetest PRIVATE imgpp)
add_test(clone bin/clonetest)

add_executable(ktxloadertest src/ktxloadertest.cpp)
add_custom_command(
  TARGET ktxloadertest
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resrc/test.ktx ${CMAKE_CURRENT_SOURCE_DIR}/resrc/astc8x8.ktx ${CMAKE_CURRENT_BINARY_DIR}/
  COMMENT "Copy test ktx image to build folder")
target_link_libraries(ktxloadertest PRIVATE imgpp)
add_test(ktxloaders bin/ktxloadertest)
