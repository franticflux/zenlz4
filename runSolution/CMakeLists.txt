cmake_minimum_required(VERSION 3.28)

project(runSolution)

add_executable(zenlz4 main.cpp)

find_package(lz4 CONFIG REQUIRED)

target_link_libraries(zenlz4 PRIVATE 
    lz4::lz4
   )
target_compile_features(zenlz4 PUBLIC cxx_std_23)
