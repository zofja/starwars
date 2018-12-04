cmake_minimum_required(VERSION 3.12)
project(starwars)

set(CMAKE_CXX_STANDARD 17)
set(-DCMAKE_C_COMPILER=clang-6.0 -DCMAKE_CXX_COMPILER=clang++6.0)

add_executable(starwars starwars_example.cc battle.h rebelfleet.h imperialfleet.h)