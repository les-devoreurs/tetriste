cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(tetriste)

# Spécifie que le projet utilise le langage C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")



# Recherche de la bibliothèque SDL2
set(SDL2_DIR "/usr/local/include/SDL2")

find_package(SDL2 REQUIRED)

# Spécifie le répertoire des includes
include_directories(${PROJECT_SOURCE_DIR}/include)


# Liste des fichiers source
set(SRC_FILES
    src/main.c
    src/game.c
    src/input.c
    src/renderer.c
    src/tetromino.c
    src/grid.c
)


# Création de l'exécutable
add_executable(tetriste_game ${SRC_FILES})

# Lier la bibliothèque SDL2
target_link_libraries(tetriste_game SDL2::SDL2)
