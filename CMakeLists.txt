cmake_minimum_required(VERSION 3.10)
project(myDiscord C)

set(CMAKE_C_STANDARD 99)

# Utiliser pkg-config pour détecter libpq
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQ REQUIRED libpq)

include_directories(${LIBPQ_INCLUDE_DIRS})
link_directories(${LIBPQ_LIBRARY_DIRS})

# Inclure les headers personnalisés
include_directories(include)

# Chercher tous les fichiers .c dans src/
file(GLOB SOURCES "src/*.c")

# Créer l'exécutable
add_executable(myDiscord ${SOURCES})

# Lier avec libpq
target_link_libraries(myDiscord ${LIBPQ_LIBRARIES})
target_link_libraries(myDiscord pq ssl crypto)