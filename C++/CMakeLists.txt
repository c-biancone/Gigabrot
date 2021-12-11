cmake_minimum_required(VERSION 3.21)
project(C__)

set(CMAKE_CXX_STANDARD 17)

add_executable(C__ main.cpp PGM.cpp PGM.h Mandelbrot.cpp Colorization.cpp Colorization.h Shading.cpp Shading.h InsideColor.cpp InsideColor.h LineColor.cpp LineColor.h Striping.cpp Striping.h NormalMap.cpp NormalMap.h Neumorphic.cpp Neumorphic.h)
