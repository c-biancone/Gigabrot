cmake_minimum_required(VERSION 3.9)
project(Gigabrot)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)

add_executable(Gigabrot
        #stripedMan.c)
        #newMan.c)
        #parallelMan.c)
        parallelStripedMan.c)

target_link_libraries(Gigabrot PRIVATE OpenMP::OpenMP_CXX)