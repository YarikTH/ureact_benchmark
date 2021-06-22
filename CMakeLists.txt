cmake_minimum_required(VERSION 3.11)
project(cpp_react_benchmark LANGUAGES CXX)

add_subdirectory(libs)

enable_testing()
add_subdirectory(tests)
