cmake_minimum_required(VERSION 3.10)
project(myDiscord C)

set(CMAKE_C_STANDARD 99)


find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQ REQUIRED libpq)

include_directories(${LIBPQ_INCLUDE_DIRS})
link_directories(${LIBPQ_LIBRARY_DIRS})

include_directories(include)

file(GLOB SOURCES "src/*.c")


add_executable(myDiscord ${SOURCES})


set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

target_link_libraries(myDiscord ${LIBPQ_LIBRARIES})


target_link_libraries(myDiscord ws2_32)
