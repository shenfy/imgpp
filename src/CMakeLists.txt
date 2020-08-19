cmake_minimum_required(VERSION 3.16)
project(imgpp VERSION 2.2.0 LANGUAGES CXX)

if (EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

set(IMGPP_HEADER
  include/imgpp/algorithms.hpp
  include/imgpp/texturedesc.hpp
  include/imgpp/texturehelper.hpp
  include/imgpp/glhelper.hpp
  include/imgpp/imgbase.hpp
  include/imgpp/imgpp_block.hpp
  include/imgpp/imgpp.hpp
  include/imgpp/compositeimg.hpp
  include/imgpp/loadersext.hpp
  include/imgpp/loaders.hpp
  include/imgpp/opencvbinding.hpp
  include/imgpp/sampler.hpp)

# static library
add_library(imgpp STATIC)
target_sources(imgpp PRIVATE
  src/bmpimg.cpp src/pfmimg.cpp src/ppmimg.cpp src/pngimg.cpp
  src/jpegimg.cpp src/helper.cpp src/bson.cpp src/glhelper.cpp src/ktximage.cpp)
target_compile_features(imgpp PUBLIC cxx_std_17)
set_target_properties(imgpp PROPERTIES PUBLIC_HEADER
  "${IMGPP_HEADER}")
target_link_libraries(imgpp PUBLIC JPEG::JPEG PNG::PNG)
target_include_directories(imgpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
set_target_properties(imgpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(TARGETS imgpp EXPORT imgppTargets
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/imgpp)

# test excutable
enable_testing()
add_executable(basictest src/testcase.cpp)
add_custom_command(
  TARGET basictest
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resrc/test.png ${CMAKE_CURRENT_BINARY_DIR}/
  COMMENT "Copy test images to build folder")
target_link_libraries(basictest PRIVATE imgpp)
add_test(basic basictest)

add_executable(loadertest src/loadertest.cpp)
add_custom_command(
  TARGET basictest
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resrc/checkboard.ppm ${CMAKE_CURRENT_BINARY_DIR}/
  COMMENT "Copy test ppm image to build folder")
target_link_libraries(loadertest PRIVATE imgpp)
add_test(loaders loadertest)

add_executable(ktxloadertest src/ktxloadertest.cpp)
target_link_libraries(ktxloadertest PRIVATE imgpp)
add_test(ktxloaders ktxloadertest)
