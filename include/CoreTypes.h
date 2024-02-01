//
// Created by pylinskyi.k on 25.01.2024.
//

#ifndef SDL_TETRIS_CORETYPES_H
#define SDL_TETRIS_CORETYPES_H

#include "SDL.h"
#include <cstdint>

enum BlockType : std::uint8_t {
    O = 0,
    I = 1,
    L = 2,
    J = 3,
    S = 4,
    Z = 5,
    T = 6
};

enum Side : std::uint8_t {
    LEFT,
    RIGHT
};

struct Tile {
    SDL_Color Color;
    bool Active;
};

#endif //SDL_TETRIS_CORETYPES_H
