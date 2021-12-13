cmake_minimum_required(VERSION 3.21)
project(C__)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-std=c++17 -march=native -lstdc++)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(C__
        #executables
        main.cpp
        #ThreadPoolTest.cpp

        #headers
        PPM.cpp PPM.h Mandelbrot.cpp Colorization.cpp Colorization.h Shading.cpp Shading.h InsideColor.cpp InsideColor.h LineColor.cpp LineColor.h Striping.cpp Striping.h NormalMap.cpp NormalMap.h Neumorphic.cpp Neumorphic.h ThreadPool.cpp ThreadPool.h UnboundedQueue.h)
