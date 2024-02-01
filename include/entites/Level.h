//
// Created by pylinskyi.k on 25.01.2024.
//

#ifndef SDL_TETRIS_LEVEL_H
#define SDL_TETRIS_LEVEL_H

#include <cstdint>
#include <vector>
#include <memory>
#include "Block.h"

class Level {
public:
    Level(std::uint8_t height, std::uint8_t width, std::uint8_t blockSize, std::uint8_t blockPadding);
    ~Level();

    void Render();
    void Update();

private:
    static constexpr Uint32 STEER_COOLDOWN = 100;
    static constexpr Uint32 FALL_NORMAL_COOLDOWN = 300;
    static constexpr Uint32 FALL_SPEED_UP_COOLDOWN = 60;

    Uint32 _currentFallCooldown;

    void InitializeGrid(std::uint8_t height, std::uint8_t width);

    bool CanFall();
    bool CanSteerRight();
    bool CanSteerLeft();
    void AddBlockToGrid();
    void RemoveBlockFromGrid();

    Uint32 _lastSteerTime = 0;
    Uint32 _lastFallTime = 0;

    std::vector<std::vector<Tile>> _grid;
    std::uint8_t GetGridHeight() const { return static_cast<std::uint8_t>(_grid.size()); }
    std::uint8_t GetGridWidth() const { return _grid.size() > 0 ? static_cast<std::uint8_t>(_grid[0].size()) : 0; }

    std::unique_ptr<Block> _fallingBlock;
    std::uint8_t _blockSize;
    std::uint8_t _blockPadding;
};

#endif //SDL_TETRIS_LEVEL_H
