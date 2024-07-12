cmake_minimum_required(VERSION 3.0.0...3.13)

project(STB)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "stb")