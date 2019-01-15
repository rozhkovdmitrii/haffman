cmake_minimum_required(VERSION 3.10)
project(haffman)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(libs/googletest)
include_directories(libs/googletest/googletest/include)
include_directories(libs/googletest/googlemock/include)

add_subdirectory(HaffmanLib)

