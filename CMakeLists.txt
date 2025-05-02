cmake_minimum_required(VERSION 3.10)

project(myDiscord
    LANGUAGES C
    DESCRIPTION "MyDiscord - Chat Application"
    VERSION 1.0
)

# Configuration du compilateur
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Configuration spécifique Windows
if(WIN32)
    # Configuration des compilateurs MinGW
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
    set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
    
    # Définitions pour Windows
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
    
endif()

# Recherche des dépendances
find_package(PkgConfig REQUIRED)

# Configuration PostgreSQL
pkg_check_modules(LIBPQ REQUIRED libpq)
if(NOT LIBPQ_FOUND)
    message(FATAL_ERROR "Bibliothèque PostgreSQL non trouvée")
endif()

# Configuration des répertoires d'inclusion
include_directories(
    ${LIBPQ_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Fichiers sources
set(DATABASE_SOURCES src/database.c)
set(SERVER_SOURCES src/server.c)
set(CLIENT_SOURCES src/client.c)
set(CONNEXION_MODULE_SOURCES src/connexion_module.c)
set(USER_SOURCES src/user.c)
set(DATABASE_SOURCES src/database.c)
set(MAIN_SOURCES src/main.c)


# Création de l'exécutable
add_executable(myDiscord
    ${MAIN_SOURCES}
    ${COMMAND_SOURCES}
    ${DATABASE_SOURCES}
    ${SERVER_SOURCES}
    ${CLIENT_SOURCES}
    ${CONEXION_MODULE_SOURCES}
    ${USER_SOURCES}
)

# Liaison des bibliothèques
target_link_libraries(myDiscord PRIVATE
    ${LIBPQ_LIBRARIES}
    ws2_32
)

# Gestion des DLL sous Windows
if(WIN32)
    #Copie automatique de libpq.dll
    find_file(PQ_DLL libpq.dll
        PATHS ${LIBPQ_LIBRARY_DIRS} "C:/msys64/mingw64/bin"
        NO_DEFAULT_PATH
    )

endif()

