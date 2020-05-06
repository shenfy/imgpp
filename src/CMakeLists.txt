cmake_minimum_required(VERSION 3.8)
project(imgpp VERSION 1.1.0 LANGUAGES CXX)

if (EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
  include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

# static library
add_library(imgpp STATIC)
target_sources(imgpp PRIVATE
  src/bmpimg.cpp src/pfmimg.cpp src/ppmimg.cpp src/pngimg.cpp
  src/jpegimg.cpp src/helper.cpp src/bson.cpp)
target_compile_features(imgpp PUBLIC cxx_std_17)
set_target_properties(imgpp PROPERTIES PUBLIC_HEADER
  "include/imgpp/imgpp.hpp;include/imgpp/algorithms.hpp;include/imgpp/loadersext.hpp;include/imgpp/loaders.hpp;include/imgpp/opencvbinding.hpp;include/imgpp/sampler.hpp")
target_link_libraries(imgpp PUBLIC JPEG::JPEG PNG::PNG)
target_include_directories(imgpp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
set_target_properties(imgpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(TARGETS imgpp EXPORT imgppTargets
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/imgpp)
install(EXPORT imgppTargets
  FILE imgppTargets.cmake
  NAMESPACE imgpp::
  DESTINATION lib/cmake/imgpp)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("imgppConfigVersion.cmake"
  VERSION ${imgpp_VERSION}
  COMPATIBILITY SameMajorVersion)
install(FILES
  "imgppConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/imgppConfigVersion.cmake"
  DESTINATION lib/cmake/imgpp)

# test excutable
enable_testing()
add_executable(basictest src/testcase.cpp)
add_custom_command(
  TARGET basictest
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/resrc/test.png ${CMAKE_CURRENT_BINARY_DIR}/test.png
  COMMENT "Copy test images to build folder")
target_link_libraries(basictest PRIVATE imgpp)
add_test(basic basictest)