cmake_minimum_required(VERSION 3.23)

project(toon
  HOMEPAGE_URL https://github.com/xZepyx/toon.hpp
  LANGUAGES CXX)

include(GNUInstallDirs)

add_library(toon INTERFACE)
target_sources(toon INTERFACE FILE_SET HEADERS BASE_DIRS src FILES src/toon.hpp)
target_compile_features(toon INTERFACE cxx_std_17)

add_executable(basic)
target_sources(basic PRIVATE examples/basic.cpp)
target_link_libraries(basic PRIVATE toon)

install(EXPORT toon_export
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/toon
  NAMESPACE toon:: FILE toon-config.cmake)

install(TARGETS toon EXPORT toon_export FILE_SET HEADERS)
