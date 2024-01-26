//
// Created by pylinskyi.k on 25.01.2024.
//


#ifndef SDL_TETRIS_BLOCK_H
#define SDL_TETRIS_BLOCK_H

#include "SDL.h"
#include <vector>
#include <functional>
#include "entites/objects/Object.h"
#include "CoreTypes.h"

class Block :Object {
public:
    Block(int x, int y, BlockType type);

    void Initialize();
    void Render();
    void Update();

private:
    static constexpr Uint32 COOLDOWN_DURATION = 125;
    Uint32 _lastPressTime = 0;

    static constexpr std::uint8_t _tileSize = 16;
    static constexpr std::uint8_t _tilePadding = 2;

    BlockType _type;
    SDL_Color _color;
    std::uint8_t _width;
    std::uint8_t _height;

    std::vector<std::vector<bool>> _matrix;

    std::function<bool(std::uint8_t i, std::uint8_t j)> _defineMatrixCallback;
    void SetDefineMatrixCallback(const std::function<bool(std::uint8_t i, std::uint8_t j)>& callback) { _defineMatrixCallback = callback; }
};

#endif //SDL_TETRIS_BLOCK_H
