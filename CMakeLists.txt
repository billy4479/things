cmake_minimum_required(VERSION 3.18)

project(
  usb-sneak-copy
  VERSION 0.0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Cache)
include(Sanitizers)
include(Warnings)

file(
  GLOB_RECURSE
  ${PROJECT_NAME}_SRC
  include/*.hpp
  include/*.h
  src/*.cpp
  src/*.hpp
  src/*.h)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_precompile_headers(${PROJECT_NAME} PRIVATE "include/Precompiled.hpp")

enable_sanitizers(${PROJECT_NAME})
set_project_warnings(${PROJECT_NAME})
