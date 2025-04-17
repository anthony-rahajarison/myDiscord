cmake_minimum_required(VERSION 3.10)
project(myDiscord C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(
    ${GTK3_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    )

link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(myDiscord src/main.c)
target_link_libraries(myDiscord ${GTK3_LIBRARIES})