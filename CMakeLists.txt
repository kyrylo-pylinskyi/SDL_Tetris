cmake_minimum_required(VERSION 3.22)
project(sdl_tetris)

set(CMAKE_CXX_STANDARD 23)

set(SOURCES
        src/Game.cpp
        src/systems/input/InputSystem.cpp
        src/entities/objects/Object.cpp
        src/entities/objects/Block.cpp
)

include_directories(vendor/pugi)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(sdl_tetris_bin main.cpp)

set_target_properties(sdl_tetris_bin PROPERTIES CXX_STANDARD 23)

target_link_libraries(sdl_tetris_bin PRIVATE ${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)
set(SDL2_PATH "vendor/SDL")
set(SDL2_IMAGE_PATH "vendor/SDL")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})