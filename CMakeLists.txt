cmake_minimum_required(VERSION 3.16)
project(myDiscord C)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find GTK4 using pkg-config
set(PKG_CONFIG_EXECUTABLE "C:/msys64/mingw64/bin/pkg-config.exe")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Add GTK4 include directories
include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})

# Add your source files
file(GLOB SOURCES "src/*.c")
add_executable(myDiscord ${SOURCES})

# Link with GTK4
target_link_libraries(myDiscord ${GTK4_LIBRARIES})