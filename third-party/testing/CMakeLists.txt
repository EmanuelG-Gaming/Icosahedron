cmake_minimum_required(VERSION 3.0.0)


project(testing)

add_library(testing)

target_sources(testing PRIVATE "src/Testing.cpp")
target_include_directories(testing PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")