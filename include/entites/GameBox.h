#ifndef SDL_TETRIS_GAMEBOX_H
#define SDL_TETRIS_GAMEBOX_H

#include <cstdint>
#include <vector>
#include <memory>
#include "Block.h"

class GameBox {
public:
    GameBox(std::uint8_t height, std::uint8_t width, int paddingLeft, int paddingUp, std::uint8_t blockSize, std::uint8_t blockPadding);
    ~GameBox();

    void Render();
    void Update();

private:
    static constexpr Uint32 ACTION_COOLDOWN = 60;
    static constexpr Uint32 FALL_NORMAL_COOLDOWN = 300;
    static constexpr Uint32 FALL_SPEED_UP_COOLDOWN = 60;

    int GetGridHeight() const { return _grid.size(); }
    int GetGridWidth() const { return _grid.size() > 0 ? _grid[0].size() : 0; }

    void InitializeGrid(std::uint8_t height, std::uint8_t width);
    void HandleSpeedUp();
    void HandleSteer();
    void HandleFall();
    void HandleRotate();
    void HandleSpawn();
    void HandleLineClear();

    bool CanFall() const;
    bool CanSteerRight() const;
    bool CanSteerLeft() const;
    bool CanRotate() const;
    bool CanClearLine(std::uint8_t row) const;

    void AddBlockToGrid();
    void RemoveBlockFromGrid();

    std::uint32_t _currentFallCooldown;
    std::uint32_t _currentTime;
    std::uint32_t _lastSteerTime;
    std::uint32_t _lastRotateTime;
    std::uint32_t _lastFallTime;

    std::vector<std::vector<Tile>> _grid;
    std::unique_ptr<Block> _fallingBlock;

    std::uint8_t _blockSize;
    std::uint8_t _blockPadding;
    int _paddingX;
    int _paddingY;

    int _score = 0;
    bool _gameOver;
};

#endif // SDL_TETRIS_GAMEBOX_H
