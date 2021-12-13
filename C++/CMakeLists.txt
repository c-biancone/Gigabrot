cmake_minimum_required(VERSION 3.21)
project(C__)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-std=c++17 -march=native -lstdc++)

add_executable(C__
        #executables
        #main.cpp
        ThreadPoolTest.cpp

        #headers
        PPM.cpp PPM.h Mandelbrot.cpp Colorization.cpp Colorization.h Shading.cpp Shading.h InsideColor.cpp InsideColor.h LineColor.cpp LineColor.h Striping.cpp Striping.h NormalMap.cpp NormalMap.h Neumorphic.cpp Neumorphic.h ThreadPool.cpp ThreadPool.h UnboundedQueue.h)
