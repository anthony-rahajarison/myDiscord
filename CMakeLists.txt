cmake_minimum_required(VERSION 3.10)
project(myDiscord C)

set(CMAKE_C_STANDARD 99)

# Utilise pkg-config depuis MSYS2 (attention à adapter si tu compiles depuis un autre terminal)
set(PKG_CONFIG_EXECUTABLE "C:/msys64/mingw64/bin/pkg-config.exe")
find_package(PkgConfig REQUIRED)

# PostgreSQL (libpq)
pkg_check_modules(LIBPQ REQUIRED libpq)
include_directories(${LIBPQ_INCLUDE_DIRS})
link_directories(${LIBPQ_LIBRARY_DIRS})

# GTK (à décommenter si tu utilises GTK plus tard)
# pkg_check_modules(GTK4 REQUIRED gtk4)
# include_directories(${GTK4_INCLUDE_DIRS})
# link_directories(${GTK4_LIBRARY_DIRS})

# Inclure tes headers si tu en as dans un dossier "include"
include_directories(include)

# Cherche tous les .c dans ton dossier src/
file(GLOB SOURCES "src/*.c")

# Crée l'exécutable
add_executable(myDiscord ${SOURCES})

# Lie les bibliothèques
target_link_libraries(myDiscord ${LIBPQ_LIBRARIES})

# Pour GTK (plus tard)
# target_link_libraries(myDiscord ${GTK4_LIBRARIES})
