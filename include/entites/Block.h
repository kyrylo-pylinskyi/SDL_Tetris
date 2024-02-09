#ifndef SDL_TETRIS_BLOCK_H
#define SDL_TETRIS_BLOCK_H

#include "CoreTypes.h"
#include "components/RigidBody.h"
#include <functional>
#include <vector>
#include <SDL.h>
#include <memory>

class Block {
public:
    Block(BlockType type);

    void Respawn(std::uint8_t gridWidth);
    void Fall();
    void Rotate();
    void SteerRight();
    void SteerLeft();

    SDL_Color GetColor() const { return _color; };
    Position GetPosition() const { return _rigidBody->GetPosition(); }
    std::vector<std::vector<bool>> GetMatrix() const { return _matrix; };

    std::uint8_t GetMatrixHeight() const { return _matrix.size(); }
    std::uint8_t GetMatrixWidth() const { return _matrix.empty() ? 0 : _matrix[0].size(); }
    bool IsTileActive(std::uint8_t i, std::uint8_t j) const { return _matrix[i][j]; }

private:
    static constexpr std::uint8_t MAX_BLOCK_TYPES = 7;

    void Initialize();
    void InitializeMatrix(int height, int width, std::function<bool(int, int)> defineMatrixCallback, const SDL_Color& color);
    void ClearMatrix();

    BlockType _type;
    SDL_Color _color;

    std::unique_ptr<RigidBody> _rigidBody;
    std::vector<std::vector<bool>> _matrix;
    std::function<bool(std::uint8_t, std::uint8_t)> _defineMatrixCallback;
};

#endif // SDL_TETRIS_BLOCK_H
