cmake_minimum_required(VERSION 3.10)

project(tetriste)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

set(SRC_FILES
    src/main.c
    src/game.c
    src/input.c
    src/renderer.c
    src/tetromino.c
    src/grid.c
    src/menu.c
    src/scores.c
)

add_executable(tetriste_game ${SRC_FILES})

target_link_libraries(tetriste_game
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
)
